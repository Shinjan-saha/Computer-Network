
# 📡 Linux Network Commands Reference

This document provides a reference of essential Linux networking commands and tools for beginners and system administrators.

---

## 🧭 1. `host` – DNS Lookup
```bash
host google.com
```
Looks up the IP address for a hostname.

---

## 🖧 2. `ifconfig` – Network Interface Info *(Deprecated in many systems)*
```bash
sudo ifconfig
```
Shows all network interfaces and their status.

### Assign IP address manually:
```bash
sudo ifconfig eth0 192.168.1.100 netmask 255.255.255.0 up
```

🔧 *If `ifconfig` is not found:*
```bash
sudo apt install net-tools    # Debian/Ubuntu  

```

---

## 🌐 3. `ip` – Modern Replacement for `ifconfig`
```bash
ip addr
```
Show IP addresses.

```bash
ip link show
```
List all interfaces.

```bash
ip route show
```
Display routing table.

---

## 📍 4. `route` – Routing Table Viewer *(Deprecated)*
```bash
route -n
```
Displays kernel routing table.

🆕 Alternative:
```bash
ip route
```

---

## 📊 5. `netstat` – Network Stats *(Deprecated)*
```bash
sudo netstat -tulnp
```
Show listening ports and their programs.

🔧 *If not available:*
```bash
sudo apt install net-tools
```

🆕 Modern replacement:
```bash
ss -tulnp
```

---

## 📡 6. `tcpdump` – Packet Sniffer
```bash
sudo tcpdump -i any
```
Capture packets on all interfaces.

```bash
sudo tcpdump -i eth0
```
Capture on specific interface.

```bash
sudo tcpdump -i eth0 -w packets.pcap
```
Save to file for Wireshark.

---

## 🧪 7. `Wireshark` – GUI Packet Analyzer
```bash
sudo wireshark
```

### Install Wireshark:
```bash
sudo apt install wireshark          # Debian/Ubuntu
```

👤 Allow non-root packet capturing:
```bash
sudo usermod -aG wireshark $USER
```
Then logout and login again.


```bash
wireshark packets.pcap

```

---

