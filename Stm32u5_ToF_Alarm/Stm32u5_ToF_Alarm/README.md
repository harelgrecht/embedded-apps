# 🚀 STM32 Real-Time Sensor Fusion with FreeRTOS

Welcome! This repository documents a real-time embedded application built on an STM32U5 microcontroller using FreeRTOS. The project captures data from a Time-of-Flight (ToF) distance sensor and the MCU's internal temperature sensor, processes the data across multiple concurrent tasks using thread-safe synchronization, and provides real-time feedback via LEDs and a serial console.

This project serves as a practical demonstration of multitasking, hardware interfacing (I2C, ADC, GPIO), synchronization techniques (Mutex), and non-blocking design patterns in an RTOS environment.

---

## 🧠 Key Features & Concepts Implemented

* ### Multitasking with FreeRTOS
    * **`startToF` Task:** Periodically reads distance from the ToF sensor (ISL29501) via I2C.
    * **`startMonitor` Task:** Periodically reads the internal CPU temperature using the ADC.
    * **`startAlarm` Task:** Acts as a central display/control task. It polls shared data, prints formatted output (Distance, Temperature, Uptime), and manages LED states based on distance thresholds.

* ### Advanced Task Synchronization with Mutex
    * A single mutex protects a shared global data structure (`globalPayload`) containing distance, temperature, and timestamp information.
    * Demonstrates safe data sharing between multiple producer tasks (`startToF`, `startMonitor`) and a consumer task (`startAlarm`).

* ### Non-Blocking LED Control
    * Implemented a responsive, non-blocking LED feedback system within the `startAlarm` task.
    * LEDs provide dynamic feedback:
        * **Fast Red Blink:** Alarm state (< 10cm).
        * **Slow Blue Blink:** Warning state (< 30cm).
        * **All Off:** Safe state.

* ### Hardware Driver Interfacing
    * **I2C:** Configured and used HAL I2C functions to communicate with the ToF sensor.
    * **ADC:** Configured and calibrated the internal ADC (12-bit, correct sampling time) to read the temperature sensor channel. Includes correct calculation using factory calibration values.
    * **GPIO:** Configured output pins for LEDs and input pins (with pull-up) for sensor interrupts.
    * **UART:** Used for `printf` debugging and displaying formatted sensor data.

* ### Uptime Timestamping
    * Uses `HAL_GetTick()` for millisecond-precision uptime tracking.
    * Includes a helper function (`calcUptime`) to convert milliseconds into a human-readable `hh:mm:ss.ms` format.

---

## 🧰 Tools & Technologies Used

* STM32CubeIDE
* **FreeRTOS** (CMSIS-OS V2 API)
* STM32U5 HAL (Hardware Abstraction Layer)
* C Programming Language
* **Peripherals:** I2C, ADC, GPIO, UART
* Git & GitHub
* STM32U5 Nucleo Board (or similar)
* Digilent Pmod ToF (ISL29501)
