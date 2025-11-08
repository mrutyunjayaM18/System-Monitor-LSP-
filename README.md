# 🖥️ System Monitor Tool (LSP, C++, Docker)

**Interactive Linux system monitor built in C++ with ncurses.**
Features CPU & memory tracking, process management, and host/container monitoring via custom `/proc` mapping.

---

## 📖 Overview

**System Monitor** is a lightweight, interactive Linux process viewer built in **C++** using **ncurses**.
It displays live process information — CPU, memory usage, and process state — similar to `top`.
The monitor can also run inside **Docker**, reading process data from a configurable `/proc` path, making it ideal for **host or container monitoring**.

---

## ✨ Features

* 🧠 Real-time **CPU & memory** usage per process
* 🔄 **Sorting** by CPU% or MEM% (toggle with `s`)
* 📉 **Adjustable refresh interval** (`+` and `-`)
* ❌ **Kill process** by entering PID (`k`)
* 🐳 **Docker integration** — monitor host or container processes
* ⚙️ Configurable `/proc` root using the `PROC_ROOT` environment variable
* 💻 Simple terminal interface using `ncurses`

---

---

## ⚙️ Build Process

Below is the **complete build and run process** for both local and Docker environments.

### ▶️ Run

| Command | Action |
| :--- | :--- |
| `make build` | Build the local binary |
| `make run` | Run locally |
| `make docker-build` | Build Docker image |
| `make docker-run` | Run in container (container processes only) |
| `make docker-run-host` | Run container monitoring host’s `/proc` |
| `make clean` | Remove local binary |
| `make docker-clean` | Remove Docker image |
| `make help` | Display available make targets |

### ⌨️ Keyboard Controls

| Key | Action |
| :--- | :--- |
| `q` | Quit |
| `s` | Toggle sorting between CPU% and MEM% |
| `o` | Toggle ascending/descending order |
| `+` | Increase refresh interval |
| `-` | Decrease refresh interval |
| `k` | Enter PID to kill (sends `SIGTERM`) |

### 🧼 Cleanup


```bash
make clean          # Remove compiled binary
make docker-clean   # Remove Docker image
```
---

### 🚀 One-Line Build & Run (Host Monitor)
```bash
make docker-run-host
```
---
