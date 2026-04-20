# ME 433 — Advanced Mechatronics
**Northwestern University · McCormick School of Engineering**

Hands-on embedded systems course covering bare-metal firmware development on the PIC32 microcontroller, hardware communication protocols, signal processing, and computer vision. All projects implemented in C with supporting Python scripts.

---

## Projects

### HW2 — Blink
GPIO configuration and LED control. Entry point for bare-metal PIC32 development — clock setup, pin configuration, and timing loops without an RTOS.

### HW3 — CDC, IO, and ADC
USB CDC (Communications Device Class) for serial communication over USB. GPIO digital I/O and 10-bit ADC sampling with proper reference voltage configuration.

### HW4 — SPI DAC
SPI peripheral configuration to drive a DAC (Digital-to-Analog Converter). Implemented waveform generation (sine, triangle, sawtooth) at controlled frequencies via SPI bit-banging and hardware SPI.

### HW5 — I²C IO Expander
I²C master implementation to interface with an MCP23008 GPIO expander. Register-level reads and writes, address configuration, and interrupt handling.

### HW6 — I²C OLED Display
Drove an SSD1306 OLED display over I²C. Implemented a custom graphics library from scratch — pixel addressing, character rendering, and screen buffering entirely in C.

### HW7 & HW9 — UART Communication
UART serial communication at configurable baud rates. Interrupt-driven RX/TX with ring buffers for robust asynchronous data handling.

### HW12 — PWM Motor Control
PWM signal generation for brushed DC motor speed and direction control. Timer configuration, duty cycle modulation, and H-bridge interfacing.

### HW13 — IMU Integration
I²C interface to an LSM6DS3 IMU (accelerometer + gyroscope). Real-time sensor fusion and tilt angle estimation from raw 16-bit readings.

### HW14 — DSP Signal Processing
Digital signal processing on live ADC data. Implemented FIR filters, computed FFTs, and analyzed frequency content of real sensor signals.

### HW15 — Computer Vision Pipeline
Python-based CV pipeline using OpenCV for camera-based robot navigation. Color detection, contour tracking, and centroid-based steering commands sent back to the PIC32 over USB serial.

---

## Hardware
- **MCU:** PIC32MX470F512H
- **Protocols:** SPI, I²C, UART, USB CDC
- **Peripherals:** SSD1306 OLED, MCP23008 GPIO expander, LSM6DS3 IMU, SPI DAC, DC motors

## Stack
`C` `Python` `OpenCV` `MPLAB X IDE` `PIC32` `Harmony v2`

---

*Northwestern University — ME 433, Spring 2024 · Advanced Mechatronics*
