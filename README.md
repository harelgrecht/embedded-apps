# Embedded Apps

A collection of embedded systems and firmware projects covering real-time OS, networking protocols, FPGA interfacing, and sensor fusion.

---

## Projects

### [Ethernet-1588-Parser](./Ethernet-1588-Parser)
**PTPv2 / IEEE 1588 Packet Monitor**

Captures and parses Precision Time Protocol (PTPv2) Ethernet packets in real-time over Layer 2, extracts timestamps (seconds & nanoseconds), and writes them into FPGA AXI BRAM for further processing.

- **Platform:** Kria KR260 + custom carrier board
- **OS:** Custom PetaLinux distribution
- **Stack:** C, libpcap, Vivado, Vitis 2023.1

---

### [Ethernet-Packet-Processor](./Ethernet-Packet-Processor)
**Modular Network Packet Processing Framework**

A modular, multi-threaded C++ framework for real-time network packet capture, routing, authentication, and tunneling. Designed around a clean pipeline architecture with thread-safe queues and real-time task scheduling.

- **Modules:** EthernetCapture, PacketRouter, PacketSender, TunnelReceiver/Transmitter, NetworkManager, GPIOHandler, Logger, InterfaceStatus
- **Stack:** C++, CMake, Boost, nlohmann/json, POSIX threads

---

### [Stm32u5_ToF_Alarm](./Stm32u5_ToF_Alarm)
**STM32U5 Real-Time Sensor Fusion with FreeRTOS**

A real-time embedded application running on an STM32U5 microcontroller. Reads data from a Time-of-Flight (ToF) distance sensor and an internal temperature sensor across concurrent FreeRTOS tasks, with mutex-protected shared state and non-blocking LED alarm feedback.

- **Platform:** STM32U5 Nucleo + Digilent Pmod ToF (ISL29501)
- **Stack:** C, FreeRTOS (CMSIS-OS V2), STM32 HAL, I2C, ADC, UART, GPIO
- **Tools:** STM32CubeIDE

---

## Tech Stack

| Area | Technologies |
|---|---|
| Languages | C, C++ |
| RTOS | FreeRTOS |
| Protocols | PTPv2 / IEEE 1588, Ethernet Layer 2 |
| Peripherals | I2C, ADC, GPIO, UART, AXI BRAM |
| Build Systems | Make, CMake |
| Platforms | STM32U5, Kria KR260 (Zynq UltraScale+), Linux |
| Tools | libpcap, Vivado, Vitis, STM32CubeIDE, PetaLinux |
