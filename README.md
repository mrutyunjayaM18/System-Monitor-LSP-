# 🖥️ System Monitor (C++17 + ncurses + Docker)

**Interactive Linux system monitor built in C++17 with ncurses.**
Features CPU & memory tracking, process management, and host/container monitoring via custom `/proc` mapping.

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen?style=for-the-badge)](https://github.com/mrutyunjayaM18/System-Monitor-LSP-/actions)
[![Docker Ready](https://img.shields.io/badge/docker-ready-blue?style=for-the-badge&logo=docker)](https://hub.docker.com/_/ubuntu)
[![License: MIT](https://img.shields.io/badge/license-MIT-yellow?style=for-the-badge)](https://github.com/mrutyunjayaM18/System-Monitor-LSP-/blob/main/LICENSE)
[![GitHub Stars](https://img.shields.io/github/stars/mrutyunjayaM18/System-Monitor-LSP-?style=for-the-badge&logo=github)](https://github.com/mrutyunjayaM18/System-Monitor-LSP-/stargazers)
[![Top Language](https://img.shields.io/github/languages/top/mrutyunjayaM18/System-Monitor-LSP-?style=for-the-badge)](https://github.com/mrutyunjayaM18/System-Monitor-LSP-/)

---

## 📖 Overview

**System Monitor** is a lightweight, interactive Linux process viewer built in **C++17** using **ncurses**.
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

## 📁 Project Structure
