# PTPv2 Monitor

## Overview
**PTPv2 Monitor** is a project designed to **monitor Precision Time Protocol (PTPv2 - IEEE 1588) Ethernet packets over Layer 2**, extract **timestamps** (seconds & nanoseconds), and store them in an **FPGA AXI BRAM** base address for further processing.

## Features
- Captures **PTPv2 packets** in real-time over **Layer 2** using `libpcap`.
- Parses **timestamp data** into **seconds** and **nanoseconds**.
- Writes parsed timestamps into **AXI BRAM** on an FPGA.
- Designed to run on **Kria KR260 with a custom carrier board**.

## System Architecture
This project is built using:
- **Hardware:** FPGA logic implemented in **Vivado**.
- **Software:** Application developed in **C**, using **Vitis 2023.1**.
- **Operating System:** Custom **PetaLinux** distribution.
- **Packet Capture:** Utilizes `libpcap` to intercept and process **PTPv2 packets**.

