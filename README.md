# ME 433: Advanced Mechatronics

Coursework from ME 433 at Northwestern University, written in C with the
Raspberry Pi Pico SDK (RP2040), plus Python scripts for signal processing and
image analysis. Each folder is one assignment.

| Folder | What it does |
|---|---|
| hw02-blink | GPIO output: blinks the on-board LED. |
| hw03-usb-adc | USB serial I/O with a button and LED, then reads a requested number of ADC samples and prints them as voltages. |
| hw04-spi-dac | Drives a two-channel SPI DAC with sine and triangle lookup tables. |
| hw05-i2c-io-expander | Reads a button and drives an LED through an I2C GPIO expander. |
| hw06-i2c-oled | Draws text on an SSD1306 OLED over I2C using a 5x7 font. |
| hw07-uart-oled | Interrupt-driven UART receive: echoes each character and shows the received line on the OLED. |
| hw09-uart | UART link between two boards using the same receive interrupt. |
| hw10-kicad-board | KiCad schematic, PCB layout and Gerber files for a small board. |
| hw11-line-detection | Python: finds the edges and centre of a line in a camera image, row by row, from the derivative of IIR-smoothed brightness. |
| hw12-pwm | 50 Hz PWM output that alternates between two duty cycles. |
| hw13-imu-usb-mouse | Reads an MPU-6050 accelerometer over I2C and reports tilt as USB HID mouse movement (TinyUSB). |
| hw14-dsp-filters | Python: FFTs of four sampled signals, then moving-average, IIR and windowed-sinc FIR low-pass filters compared before and after. |
| hw15-robot-cad | CAD renders of the camera mount and wheel for the line-following robot. |

## Notes

- The CMake files and several source files started from Pico SDK examples and
  course starter code (including the SSD1306 driver and the USB descriptors).
  My work is the per-assignment logic on top of them.
- The firmware is kept as submitted and has not been retested. Known problems:
  in hw04 the sine channel indexes the table with a constant, in hw13 the
  Y axis reads the same value as X, and hw13 needs the SSD1306 source from
  hw06 to build.
- The KiCad files in hw10-kicad-board are named HW11 internally.

## Stack

C, Raspberry Pi Pico SDK, I2C, SPI, UART, PWM, ADC, TinyUSB, KiCad, Python (NumPy, pandas, Matplotlib, Pillow)
