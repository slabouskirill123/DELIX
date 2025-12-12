# DELIX KERNEL (BETA)

**DELIX** — a standalone bare-metal kernel for x86, inspired by Unix but not compatible with it.  
The goal of the project is to create a minimal, modular, and extensible operating system from scratch.

## ✨ Features

- **Bare-metal**: runs directly on x86 hardware without intermediaries
- **Terminal interface**: commands, colors, navigation
- **Unix-like architecture**: modularity, simplicity, readability
- **Theme support**: `dark` and `light` modes
- **Built-in RTC**: shows real-time clock
- **Hardware detection**: CPU vendor, RAM size

## 🛠️ Building

### Requirements

- Debian 13 (or compatible)
- `build-essential`, `nasm`, `grub-pc-bin`, `xorriso`, `mtools`

```bash
sudo apt install build-essential nasm grub-pc-bin xorriso mtools
