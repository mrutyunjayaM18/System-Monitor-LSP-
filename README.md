<h1 align="center">🖥️ System Monitor (C++17 + ncurses + Docker)</h1>

<p align="center">
  <b>Interactive Linux system monitor built in C++17 with ncurses.</b><br>
  Features CPU & memory tracking, process management, and host/container monitoring via custom <code>/proc</code> mapping.
</p>

<p align="center">
  <a href="https://github.com/mrutyunjayaM18/System-Monitor-LSP-/actions">
    <img src="https://img.shields.io/badge/build-passing-brightgreen?style=for-the-badge" alt="Build Status">
  </a>
  <a href="https://hub.docker.com/_/ubuntu">
    <img src="https://img.shields.io/badge/docker-ready-blue?style=for-the-badge&logo=docker" alt="Docker Ready">
  </a>
  <a href="https://github.com/mrutyunjayaM18/System-Monitor-LSP-/blob/main/LICENSE">
    <img src="https://img.shields.io/badge/license-MIT-yellow?style=for-the-badge" alt="License: MIT">
  </a>
  <a href="https://github.com/mrutyunjayaM18/System-Monitor-LSP-/stargazers">
    <img src="https://img.shields.io/github/stars/mrutyunjayaM18/System-Monitor-LSP-?style=for-the-badge&logo=github" alt="GitHub Stars">
  </a>
  <a href="https://github.com/mrutyunjayaM18/System-Monitor-LSP-/">
    <img src="https://img.shields.io/github/languages/top/mrutyunjayaM18/System-Monitor-LSP-?style=for-the-badge" alt="Top Language">
  </a>
</p>

---

# 📖 Overview

**System Monitor** is a lightweight, interactive Linux process viewer built in **C++17** using **ncurses**.  
It displays live process information — CPU, memory usage, and process state — similar to `top`.  
The monitor can also run inside **Docker**, reading process data from a configurable `/proc` path, making it ideal for **host or container monitoring**.

---

## ✨ Features

- 🧠 Real-time **CPU & memory** usage per process  
- 🔄 **Sorting** by CPU% or MEM% (toggle with `s`)  
- 📉 **Adjustable refresh interval** (`+` and `-`)  
- ❌ **Kill process** by entering PID (`k`)  
- 🐳 **Docker integration** — monitor host or container processes  
- ⚙️ Configurable `/proc` root using the `PROC_ROOT` environment variable  
- 💻 Simple terminal interface using `ncurses`  

---

## 📁 Project Structure

.
├── monitor.cpp # C++17 source code
├── Dockerfile # Multi-stage Docker build
├── Makefile # Local and Docker build automation
└── README.md # Documentation (this file)



# ⚙️ Build Process

Below is the **complete build and run process** for both local and Docker environments.


▶️ Run

make build	Build the local binary
make run	Run locally
make docker-build	Build Docker image
make docker-run	Run in container (container processes only)
make docker-run-host	Run container monitoring host’s /proc
make clean	Remove local binary
make docker-clean	Remove Docker image
make help	Display available make targets

⌨️ Keyboard Controls
Key	Action
q	Quit
s	Toggle sorting between CPU% and MEM%
o	Toggle ascending/descending order
+	Increase refresh interval
-	Decrease refresh interval
k	Enter PID to kill (sends SIGTERM)

🧼 Cleanup
bash
Copy code
make clean          # Remove compiled binary
make docker-clean   # Remove Docker image

🚀 One-Line Build & Run (Host Monitor)
make docker-run-host	Run container monitoring host’s /proc

🧑‍💻 Author
Mrutyunjaya Mishra
📦 System Monitor (C++ / ncurses / /proc)
🐙 GitHub: mrutyunjayaM18

