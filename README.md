 🔍 checkMSS

> **Low-level C tool for proactive MSS (Maximum Segment Size) discovery via TCP port scanning.**

![Language](https://img.shields.io/badge/language-C-blue?style=flat-square)
![Version](https://img.shields.io/badge/version-1.0-green?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-orange?style=flat-square)

---

## 📖 Description

**checkMSS** is a low-level network tool written in C that performs a TCP SYN port scan against a target host and extracts the **MSS (Maximum Segment Size)** value from each TCP response.

It reports the **lowest MSS discovered** across all open ports, enabling proactive verification and optimization of **TCP segmentation and fragmentation** behavior — useful for network engineers, security auditors, and system administrators.

> ⚠️ **Requires root privileges** — raw socket operations are used to craft and capture packets at the network layer.

---

## ✨ Features

- 🔌 Raw socket-based TCP SYN scanning (no libpcap dependency)
- 📡 Simultaneous packet sniffing and SYN injection
- 📊 MSS value extraction from TCP option fields
- 🏆 Returns the **minimum MSS** found across all open ports
- ⚙️ Configurable source address, destination, timeout, and scan scope
- 🎯 Two scan modes: **top ports** or **all ports**

---

## 🚀 Usage

```bash
./checkmss [-d destination addr] [-s source addr] [-t timeout] [-o all|top]
```

### Options

| Flag | Description | Default |
|------|-------------|---------|
| `-d` | Destination IP address (**required**) | — |
| `-s` | Source IP address | Auto-detected |
| `-t` | Timeout in seconds | `3` |
| `-o` | Scan scope: `all` (all ports) or `top` (top common ports) | `top` |

---

## 💡 Examples

### Scan all ports on a target

```bash
./checkmss -d 8.8.8.8 -o all
```

**Output:**
```
CheckMSS By Tybbow - v 1.0
[+] Source Address : 192.168.137.2
[+] Destination Address : 8.8.8.8
[+] Timeout : 3
[+] Option : 1

[+] Socket created.
[+] Sniffing in progress...
[+] Sending SYN packets...
[+] Port Open: 53  - MSS: 1412
[+] Port Open: 443 - MSS: 1412
[+] Port Open: 853 - MSS: 1412

[+] Value MSS : 1412
```

### Scan top common ports only

```bash
./checkmss -d 192.168.1.1 -o top
```

### Custom source address and timeout

```bash
./checkmss -d 10.0.0.1 -s 10.0.0.100 -t 5 -o all
```

---

## 🛠️ Build

### Prerequisites

- GCC or Clang
- Linux with raw socket support
- Root access (or `CAP_NET_RAW` capability)

### Compile
```bash
Make
Make fclean
make clean
```

### Run

```bash
sudo ./checkmss -d  -o all
```

---

## 🧠 How It Works

1. **Raw socket creation** — Opens a raw `SOCK_RAW` socket to craft custom TCP/IP packets.
2. **SYN packet injection** — Sends TCP SYN packets to ports on the target host.
3. **Packet sniffing** — Simultaneously captures incoming TCP SYN-ACK responses.
4. **MSS extraction** — Parses TCP options in each SYN-ACK to extract the MSS value.
5. **Minimum reporting** — Returns the lowest MSS observed, indicating the most restrictive path MTU constraint.

```
[checkMSS]                          [Target Host]
     |                                    |
     |------- TCP SYN (port 53) -------->|
     |<------ TCP SYN-ACK (MSS=1412) ----|
     |                                    |
     |------- TCP SYN (port 443) ------->|
     |<------ TCP SYN-ACK (MSS=1412) ----|
     |                                    |
     |  => Minimum MSS reported: 1412     |
```

---

## 📐 Why MSS Matters

The **MSS** defines the largest amount of data a TCP segment can carry. A mismatch or oversized MSS value can cause:

- 🐌 **Silent packet drops** due to fragmentation
- ⚠️ **Black-hole routing** issues on paths with ICMP filtering
- 📉 **Degraded throughput** from excessive segmentation

**checkMSS** allows you to proactively detect the effective MSS before issues arise.

---

## ⚠️ Disclaimer

This tool uses **raw sockets** and sends **forged TCP packets**. Usage should be restricted to:

- Networks and systems **you own or are authorized to test**
- Controlled lab environments

Unauthorized port scanning may violate local laws and network policies.

---

## 👤 Author

**Tybbow** — v1.0

---

## 📄 License

This project is licensed under the [MIT License](LICENSE)
