// monitor.cpp - patched to remove unused-variable warnings
// Simple system monitor (top-like) in C++ using /proc and ncurses.
// Build: g++ -std=c++17 monitor.cpp -o monitor -lncurses

#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <signal.h>
#include <pwd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std::chrono;
using namespace std;

struct ProcSnapshot {
    long long utime = 0;
    long long stime = 0;
    long long total_time() const { return utime + stime; }
};

struct Process {
    int pid;
    string user;
    string cmd;
    string state;
    double cpu = 0.0;
    double mem = 0.0;
    long long utime = 0;
    long long stime = 0;
    long long rss = 0; // resident set size in pages
};

static long long last_total_cpu = 0;
static map<int, ProcSnapshot> last_proc_snap;

// --- Use PROC_ROOT environment variable (defaults to /proc) ---
static std::string get_proc_root() {
    const char *env = std::getenv("PROC_ROOT");
    if (env && env[0] != '\0') return std::string(env);
    return std::string("/proc");
}
static std::string PROC_ROOT = get_proc_root();
// -----------------------------------------------------------

long long parse_total_cpu() {
    std::ifstream f(PROC_ROOT + std::string("/stat"));
    string line;
    if (!getline(f, line)) return 0;
    std::istringstream iss(line);
    string cpulabel;
    iss >> cpulabel;
    long long val, total = 0;
    while (iss >> val) total += val;
    return total;
}

bool read_mem_info(long long &mem_total_kb, long long &mem_available_kb) {
    std::ifstream f(PROC_ROOT + std::string("/meminfo"));
    if (!f) return false;
    string line;
    mem_total_kb = mem_available_kb = 0;
    while (getline(f, line)) {
        if (line.rfind("MemTotal:", 0) == 0) {
            std::istringstream iss(line);
            string label; long long val; string unit;
            iss >> label >> val >> unit;
            mem_total_kb = val;
        } else if (line.rfind("MemAvailable:", 0) == 0) {
            std::istringstream iss(line);
            string label; long long val; string unit;
            iss >> label >> val >> unit;
            mem_available_kb = val;
        }
        if (mem_total_kb && mem_available_kb) break;
    }
    return mem_total_kb > 0;
}

string uid_to_user(uid_t uid) {
    struct passwd *pw = getpwuid(uid);
    if (pw) return string(pw->pw_name);
    return to_string((long)uid);
}

bool read_proc_stat(int pid, ProcSnapshot &snap, long long &starttime) {
    string path = PROC_ROOT + "/" + to_string(pid) + "/stat";
    std::ifstream f(path);
    if (!f) return false;
    string content;
    getline(f, content);
    // /proc/[pid]/stat: many fields. We will parse carefully.
    // Field indices: 1 pid, 2 comm (may have spaces inside parentheses), 3 state, ... utime at 14, stime 15, starttime 22, rss at 24 (in pages)
    // Approach: find the parentheses-enclosed comm, then split remainder.
    size_t lparen = content.find('(');
    size_t rparen = content.rfind(')');
    if (lparen == string::npos || rparen == string::npos || rparen <= lparen) return false;
    string before = content.substr(0, lparen);
    string comm = content.substr(lparen+1, rparen - lparen - 1);
    string after = content.substr(rparen+1);
    std::istringstream iss(after);
    string state;
    // fields after comm: state (1), then many fields. We need to count to utime (14th overall)
    // We'll read into a vector of tokens for simplicity:
    vector<string> tokens;
    string tok;
    while (iss >> tok) tokens.push_back(tok);
    // tokens[0] is state
    if (tokens.size() < 22) return false; // need at least up to starttime
    state = tokens[0];
    long long utime = atoll(tokens[13].c_str()); // token index 13 (0-based) corresponds to field 14
    long long stime = atoll(tokens[14].c_str()); // field 15
    long long _starttime = atoll(tokens[21].c_str()); // field 22
    snap.utime = utime;
    snap.stime = stime;
    starttime = _starttime;
    return true;
}

bool read_cmd_and_user(int pid, string &cmd, string &state, uid_t &uid_out, long long &rss_pages) {
    // Read /proc/<pid>/cmdline for cmd; if empty, use comm.
    string base = PROC_ROOT + "/" + to_string(pid);
    string cmdline_path = base + "/cmdline";
    std::ifstream f(cmdline_path, ios::in | ios::binary);
    if (f) {
        // read entire file and replace NULs with spaces
        std::string all;
        char c;
        while (f.get(c)) {
            if (c == '\0') all += ' ';
            else all += c;
        }
        if (!all.empty()) cmd = all;
    }
    if (cmd.empty()) {
        // fallback to comm
        string comm_path = base + "/comm";
        std::ifstream fc(comm_path);
        if (fc) getline(fc, cmd);
    }
    // read status for uid and state and VmRSS
    string status_path = base + "/status";
    std::ifstream fs(status_path);
    uid_out = 0;
    state = "";
    rss_pages = 0;
    if (!fs) return false;
    string line;
    while (getline(fs, line)) {
        if (line.rfind("Uid:", 0) == 0) {
            std::istringstream iss(line);
            string label; unsigned long uid; iss >> label >> uid;
            uid_out = (uid_t)uid;
        } else if (line.rfind("State:", 0) == 0) {
            std::istringstream iss(line);
            string label; string st; iss >> label >> st;
            state = st;
        } else if (line.rfind("VmRSS:", 0) == 0) {
            // VmRSS: value kB. We'll pick this if present.
            std::istringstream iss(line);
            string label; long long val; string unit;
            iss >> label >> val >> unit;
            // convert kB to pages later using page size.
            long long kbytes = val;
            long long page_size_kb = sysconf(_SC_PAGESIZE) / 1024;
            if (page_size_kb > 0) rss_pages = (kbytes + page_size_kb - 1) / page_size_kb;
        }
    }
    // If VmRSS not found, read statm
    if (rss_pages == 0) {
        string statm_path = base + "/statm";
        std::ifstream fm(statm_path);
        if (fm) {
            long long size, resident;
            fm >> size >> resident;
            rss_pages = resident;
        }
    }
    return true;
}

vector<int> list_pids() {
    vector<int> pids;
    DIR *d = opendir(PROC_ROOT.c_str());
    if (!d) return pids;
    struct dirent *entry;
    while ((entry = readdir(d)) != nullptr) {
        if (entry->d_type != DT_DIR) continue;
        string name = entry->d_name;
        bool digits = !name.empty() && all_of(name.begin(), name.end(), ::isdigit);
        if (digits) pids.push_back(stoi(name));
    }
    closedir(d);
    return pids;
}

vector<Process> gather_processes(long long total_cpu_delta, long long mem_total_kb, long long mem_available_kb, long long clk_tck, long long page_size) {
    // silence intentionally unused parameters to avoid compiler warnings
    (void)mem_available_kb;
    (void)clk_tck;

    vector<Process> procs;
    vector<int> pids = list_pids();
    for (int pid : pids) {
        Process p;
        p.pid = pid;
        string cmd;
        string state;
        uid_t uid;
        long long rss_pages;
        if (!read_cmd_and_user(pid, cmd, state, uid, rss_pages)) continue;
        p.cmd = cmd;
        p.state = state;
        p.user = uid_to_user(uid);
        // read stat snapshot
        ProcSnapshot snap;
        long long starttime;
        if (!read_proc_stat(pid, snap, starttime)) continue;
        p.utime = snap.utime;
        p.stime = snap.stime;
        p.rss = rss_pages;
        // compute cpu percent using last_proc_snap
        double cpu_pct = 0.0;
        auto it = last_proc_snap.find(pid);
        if (it != last_proc_snap.end() && total_cpu_delta > 0) {
            long long prev_total = it->second.total_time();
            long long curr_total = snap.total_time();
            long long proc_delta = curr_total - prev_total;
            // CPU% = (proc_delta / total_cpu_delta) * 100
            cpu_pct = (double)proc_delta * 100.0 / (double)total_cpu_delta;
        }
        p.cpu = cpu_pct;
        // memory percent: rss_pages * page_size / mem_total_bytes
        long long rss_bytes = rss_pages * page_size;
        long long mem_total_bytes = mem_total_kb * 1024LL;
        double mem_pct = (mem_total_bytes > 0) ? (double)rss_bytes * 100.0 / (double)mem_total_bytes : 0.0;
        p.mem = mem_pct;
        procs.push_back(p);
    }
    return procs;
}

void draw_header(WINDOW *w, int width, const string &sort_by, bool descending, int interval) {
    // width currently unused; mark explicitly to silence warnings (could also be used to center text)
    (void)width;

    werase(w);
    mvwprintw(w, 0, 0, "SimpleSysMon - refresh: %d s | sort: %s %s | PROC_ROOT: %s | q:quit  s:toggle sort  k:kill PID",
              interval,
              sort_by.c_str(),
              descending ? "DESC" : "ASC",
              PROC_ROOT.c_str());
    // column header
    mvwprintw(w, 1, 0, "%5s %-8s %6s %6s %-7s %s", "PID", "USER", "CPU%", "MEM%", "STATE", "COMMAND");
    wrefresh(w);
}

void draw_processes(WINDOW *w, const vector<Process> &procs, int start_row, int rows, int cols) {
    int line = start_row;
    int displayed = 0;
    for (const auto &p : procs) {
        if (displayed >= rows) break;
        // sanitize cmd length
        string cmd = p.cmd;
        if ((int)cmd.size() > cols - 40) cmd = cmd.substr(0, cols - 43) + "...";
        mvwprintw(w, line, 0, "%5d %-8s %6.2f %6.2f %-7s %s",
                  p.pid,
                  p.user.c_str(),
                  p.cpu,
                  p.mem,
                  p.state.c_str(),
                  cmd.c_str());
        line++;
        displayed++;
    }
    wrefresh(w);
}

int main() {
    // config
    int interval_seconds = 2;
    string sort_by = "CPU"; // or "MEM"
    bool descending = true;

    // setup ncurses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // non-blocking getch
    keypad(stdscr, TRUE);
    curs_set(0);

    long long clk_tck = sysconf(_SC_CLK_TCK);
    long long page_size = sysconf(_SC_PAGESIZE);

    // initial total cpu
    last_total_cpu = parse_total_cpu();

    // initial process snapshots
    vector<int> pids_init = list_pids();
    for (int pid : pids_init) {
        ProcSnapshot snap;
        long long starttime;
        if (read_proc_stat(pid, snap, starttime)) {
            last_proc_snap[pid] = snap;
        }
    }

    bool running = true;
    while (running) {
        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        erase();
        // read total cpu now, compute delta since last
        long long total_cpu_now = parse_total_cpu();
        long long total_cpu_delta = total_cpu_now - last_total_cpu;
        // read mem
        long long mem_total_kb=0, mem_avail_kb=0;
        read_mem_info(mem_total_kb, mem_avail_kb);
        // gather processes (reads current stat and cmd/status)
        vector<Process> procs = gather_processes(total_cpu_delta, mem_total_kb, mem_avail_kb, clk_tck, page_size);
        // update last_proc_snap for next iteration
        last_proc_snap.clear();
        for (const auto &p : procs) {
            ProcSnapshot snap;
            snap.utime = p.utime;
            snap.stime = p.stime;
            last_proc_snap[p.pid] = snap;
        }
        last_total_cpu = total_cpu_now;

        // sorting
        if (sort_by == "CPU") {
            sort(procs.begin(), procs.end(), [&](const Process &a, const Process &b) {
                if (descending) return a.cpu > b.cpu;
                return a.cpu < b.cpu;
            });
        } else {
            sort(procs.begin(), procs.end(), [&](const Process &a, const Process &b) {
                if (descending) return a.mem > b.mem;
                return a.mem < b.mem;
            });
        }

        // draw header and processes
        draw_header(stdscr, cols, sort_by, descending, interval_seconds);
        draw_processes(stdscr, procs, 2, rows - 3, cols);

        // handle input (non-blocking)
        int ch;
        int sleep_ms = interval_seconds * 1000 / 10; // we'll loop 10 times checking input for more responsiveness
        for (int i = 0; i < 10; ++i) {
            ch = getch();
            if (ch != ERR) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
        }
        if (ch != ERR) {
            if (ch == 'q' || ch == 'Q') {
                running = false;
            } else if (ch == 's' || ch == 'S') {
                // toggle sort
                if (sort_by == "CPU") sort_by = "MEM";
                else sort_by = "CPU";
            } else if (ch == 'o' || ch == 'O') {
                descending = !descending;
            } else if (ch == 'k' || ch == 'K') {
                nodelay(stdscr, FALSE);
                echo();
                curs_set(1);
                mvprintw(rows - 1, 0, "Enter PID to kill: ");
                char buf[32];
                getnstr(buf, sizeof(buf)-1);
                int pid_to_kill = atoi(buf);
                if (pid_to_kill > 0) {
                    int res = kill(pid_to_kill, SIGTERM);
                    if (res == 0) {
                        mvprintw(rows - 1, 0, "Sent SIGTERM to %d. Press any key to continue.     ", pid_to_kill);
                    } else {
                        mvprintw(rows - 1, 0, "Failed to kill %d (permission or not exist). Press any key. ", pid_to_kill);
                    }
                    getch();
                }
                // restore
                nodelay(stdscr, TRUE);
                noecho();
                curs_set(0);
            } else if (ch == '+') {
                if (interval_seconds < 10) interval_seconds++;
            } else if (ch == '-') {
                if (interval_seconds > 1) interval_seconds--;
            }
        }
    }

    endwin();
    return 0;
}

