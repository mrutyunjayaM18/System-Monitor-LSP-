<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1" />
  <title>System Monitor (C++17 + ncurses + Docker)</title>
  <style>
    :root{
      --bg:#0f1724;
      --card:#0b1220;
      --muted:#9aa4b2;
      --accent:#38bdf8;
      --accent-2:#60a5fa;
      --mono: "SFMono-Regular",Monaco,Consolas,"Liberation Mono","Courier New",monospace;
      --maxw:980px;
    }
    body{
      background: #f7fafc;
      color:#0b1724;
      font-family: Inter, ui-sans-serif, system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial;
      margin:0;
      padding:30px 16px;
      line-height:1.55;
    }
    .container{max-width:var(--maxw);margin:0 auto;background:#fff;padding:28px;border-radius:12px;box-shadow:0 8px 30px rgba(11,17,36,0.06);}
    header{text-align:center;margin-bottom:18px}
    h1{font-size:28px;margin:6px 0}
    p.lead{color:var(--muted);margin-top:6px}
    .badges{display:flex;gap:8px;justify-content:center;margin:12px 0 22px}
    .badges img{height:34px;border-radius:6px}
    hr{border:none;border-top:1px solid #e6edf3;margin:20px 0}
    section{margin:18px 0}
    h2{font-size:18px;margin:8px 0 12px}
    ul{padding-left:20px;margin:8px 0}
    pre{background:#0b1220;color:#e6eef6;padding:14px;border-radius:8px;overflow:auto;font-family:var(--mono);font-size:13px}
    code{background:#eef2f7;padding:2px 6px;border-radius:6px;font-family:var(--mono);font-size:13px}
    .project-struct{background:#fafbfd;border:1px dashed #e7eef7;padding:12px;border-radius:8px;font-family:var(--mono)}
    table{width:100%;border-collapse:collapse;margin:8px 0}
    table th, table td{padding:8px 10px;border:1px solid #eef3f6;text-align:left}
    .controls{display:grid;grid-template-columns:repeat(auto-fit,minmax(140px,1fr));gap:8px}
    .muted{color:var(--muted)}
    footer{font-size:13px;color:var(--muted);margin-top:22px;text-align:center}
    .one-line{display:flex;gap:8px;align-items:center;flex-wrap:wrap}
    a.link{color:var(--accent);text-decoration:none;font-weight:600}
    .kbd{background:#f1f5f9;padding:4px 8px;border-radius:6px;border:1px solid #e6edf3;font-family:var(--mono)}
  </style>
</head>
<body>
  <div class="container">
    <header>
      <h1>🖥️ System Monitor (C++17 + ncurses + Docker)</h1>
      <p class="lead"><strong>Interactive Linux system monitor built in C++17 with ncurses.</strong><br>
        Features CPU &amp; memory tracking, process management, and host/container monitoring via custom <code>/proc</code> mapping.
      </p>

      <div class="badges" aria-hidden="true">
        <a href="https://github.com/mrutyunjayaM18/System-Monitor-LSP-/actions" title="Build Status">
          <img src="https://img.shields.io/badge/build-passing-brightgreen?style=for-the-badge" alt="Build Status">
        </a>
        <a href="https://hub.docker.com/_/ubuntu" title="Docker Ready">
          <img src="https://img.shields.io/badge/docker-ready-blue?style=for-the-badge&logo=docker" alt="Docker Ready">
        </a>
        <a href="https://github.com/mrutyunjayaM18/System-Monitor-LSP-/blob/main/LICENSE" title="License: MIT">
          <img src="https://img.shields.io/badge/license-MIT-yellow?style=for-the-badge" alt="License: MIT">
        </a>
        <a href="https://github.com/mrutyunjayaM18/System-Monitor-LSP-/stargazers" title="GitHub Stars">
          <img src="https://img.shields.io/github/stars/mrutyunjayaM18/System-Monitor-LSP-?style=for-the-badge&logo=github" alt="GitHub Stars">
        </a>
        <a href="https://github.com/mrutyunjayaM18/System-Monitor-LSP-/" title="Top Language">
          <img src="https://img.shields.io/github/languages/top/mrutyunjayaM18/System-Monitor-LSP-?style=for-the-badge" alt="Top Language">
        </a>
      </div>
    </header>

    <hr>

    <section id="overview">
      <h2>📖 Overview</h2>
      <p><strong>System Monitor</strong> is a lightweight, interactive Linux process viewer built in <strong>C++17</strong> using <strong>ncurses</strong>.
      It displays live process information — CPU, memory usage, and process state — similar to <code>top</code>.
      The monitor can also run inside <strong>Docker</strong>, reading process data from a configurable <code>/proc</code> path, making it ideal for <strong>host or container monitoring</strong>.</p>
    </section>

    <section id="features">
      <h2>✨ Features</h2>
      <ul>
        <li>🧠 Real-time <strong>CPU &amp; memory</strong> usage per process</li>
        <li>🔄 <strong>Sorting</strong> by CPU% or MEM% (toggle with <span class="kbd">s</span>)</li>
        <li>📉 <strong>Adjustable refresh interval</strong> (<span class="kbd">+</span> and <span class="kbd">-</span>)</li>
        <li>❌ <strong>Kill process</strong> by entering PID (<span class="kbd">k</span>)</li>
        <li>🐳 <strong>Docker integration</strong> — monitor host or container processes</li>
        <li>⚙️ Configurable <code>/proc</code> root using the <code>PROC_ROOT</code> environment variable</li>
        <li>💻 Simple terminal interface using <strong>ncurses</strong></li>
      </ul>
    </section>

    <section id="structure">
      <h2>📁 Project Structure</h2>
      <div class="project-struct">
<pre>
.
├── monitor.cpp     # C++17 source code
├── Dockerfile      # Multi-stage Docker build
├── Makefile        # Local and Docker build automation
└── README.md       # Documentation (this file)
</pre>
      </div>
    </section>

    <section id="build">
      <h2>⚙️ Build Process</h2>
      <p>Below is the <strong>complete build and run process</strong> for both local and Docker environments.</p>

      <h3>🧩 Prerequisites</h3>
      <p>Install dependencies on <strong>Ubuntu/Debian</strong>:</p>
      <pre>sudo apt update
sudo apt install -y build-essential g++ make libncurses5-dev libncursesw5-dev docker.io</pre>

      <h3>💻 Local Build and Run</h3>
      <p><strong>Build:</strong></p>
      <pre>make build</pre>
      <p>This compiles <code>monitor.cpp</code> using <code>g++</code> with <code>ncurses</code> and outputs the executable <code>monitor</code>.</p>

      <p><strong>Run:</strong></p>
      <pre>make run
# or
./monitor</pre>

      <p><strong>Use a custom <code>/proc</code> root:</strong></p>
      <pre>PROC_ROOT=/path/to/proc ./monitor</pre>

      <h3>🐳 Docker Build and Run</h3>
      <p>The project includes a multi-stage <code>Dockerfile</code>.</p>

      <p><strong>Build Docker image:</strong></p>
      <pre>make docker-build</pre>

      <p><strong>Run inside container (container processes):</strong></p>
      <pre>make docker-run
# or
docker run --rm -it simple-sysmon:latest</pre>

      <p><strong>Run against host’s <code>/proc</code> (host monitoring):</strong></p>
      <pre>make docker-run-host</pre>
      <p>Equivalent to:</p>
      <pre>docker run --rm -it \
  --pid=host \
  -v /proc:/hostproc:ro \
  -e PROC_ROOT=/hostproc \
  simple-sysmon:latest</pre>

      <p>This shares your host’s PID namespace and mounts <code>/proc</code> read-only, allowing full host monitoring from inside Docker.</p>

      <h3>🧰 Makefile Commands Summary</h3>
      <table>
        <thead>
          <tr><th>Command</th><th>Description</th></tr>
        </thead>
        <tbody>
          <tr><td><code>make build</code></td><td>Build the local binary</td></tr>
          <tr><td><code>make run</code></td><td>Run locally</td></tr>
          <tr><td><code>make docker-build</code></td><td>Build Docker image</td></tr>
          <tr><td><code>make docker-run</code></td><td>Run in container (container processes only)</td></tr>
          <tr><td><code>make docker-run-host</code></td><td>Run container monitoring host’s <code>/proc</code></td></tr>
          <tr><td><code>make clean</code></td><td>Remove local binary</td></tr>
          <tr><td><code>make docker-clean</code></td><td>Remove Docker image</td></tr>
          <tr><td><code>make help</code></td><td>Display available make targets</td></tr>
        </tbody>
      </table>
    </section>

    <section id="controls">
      <h2>⌨️ Keyboard Controls</h2>
      <div class="controls">
        <div><strong><span class="kbd">q</span></strong><div class="muted">Quit</div></div>
        <div><strong><span class="kbd">s</span></strong><div class="muted">Toggle sorting between CPU% and MEM%</div></div>
        <div><strong><span class="kbd">o</span></strong><div class="muted">Toggle ascending/descending order</div></div>
        <div><strong><span class="kbd">+</span></strong><div class="muted">Increase refresh interval</div></div>
        <div><strong><span class="kbd">-</span></strong><div class="muted">Decrease refresh interval</div></div>
        <div><strong><span class="kbd">k</span></strong><div class="muted">Enter PID to kill (sends SIGTERM)</div></div>
      </div>
    </section>

    <section id="env">
      <h2>🧩 Environment Variables</h2>
      <table>
        <thead><tr><th>Variable</th><th>Default</th><th>Description</th></tr></thead>
        <tbody>
          <tr><td><code>PROC_ROOT</code></td><td><code>/proc</code></td><td>Alternate <code>/proc</code> path (used for host monitoring inside Docker)</td></tr>
        </tbody>
      </table>
    </section>

    <section id="cleanup">
      <h2>🧼 Cleanup</h2>
      <pre>make clean          # Remove compiled binary
make docker-clean   # Remove Docker image</pre>
    </section>

    <section id="example">
      <h2>🧾 Example Output</h2>
      <pre>SimpleSysMon - refresh: 2 s | sort: CPU DESC | PROC_ROOT: /proc | q:quit  s:toggle sort  k:kill PID
  PID USER       CPU%   MEM% STATE   COMMAND
    1 root       0.0    0.2  S      /init
  424 mrutyun    2.3    1.0  R      ./monitor
  317 daemon     0.7    0.3  S      /usr/bin/dbus-daemon --system</pre>
    </section>

    <section id="one-line">
      <h2>🚀 One-Line Build &amp; Run (Host Monitor)</h2>
      <div class="one-line">
        <pre>docker build -t simple-sysmon . &amp;&amp; \
docker run --rm -it --pid=host -v /proc:/hostproc:ro -e PROC_ROOT=/hostproc simple-sysmon</pre>
      </div>
    </section>

    <section id="author">
      <h2>🧑‍💻 Author</h2>
      <p><strong>Mrutyunjaya Mishra</strong><br>
      📦 System Monitor (C++ / ncurses / /proc)<br>
      🐙 GitHub: <a class="link" href="https://github.com/mrutyunjayaM18">mrutyunjayaM18</a></p>
    </section>

    <section id="license">
      <h2>🪪 License</h2>
      <p>This project is licensed under the <strong>MIT License</strong>. You’re free to use, modify, and distribute it with attribution.</p>
    </section>

    <footer>
      <p class="muted">Interactive. Lightweight. Docker-ready. — The Linux System Monitor, reimagined.</p>
    </footer>
  </div>
</body>
</html>
