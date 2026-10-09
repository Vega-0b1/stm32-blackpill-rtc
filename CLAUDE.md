# BlackPill RTC — Hardware Reference

## MCU

- **Board:** WeAct BlackPill, STM32F411CEU6.
- **Clock:** HSE + PLL → 96 MHz.
- **FPU:** single-precision only. `float` is hardware-accelerated; `double` is
  software-emulated.

## Wiring

- **I2C1:** PB6 = SCL, PB7 = SDA, 100 kHz. One shared bus for every I2C device below.
- **Buttons:** MODE = PA0, UP = PA7. Internal pull-ups; each button shorts its pin
  to GND, so pressed reads low.

## I2C bus devices

All on I2C1 (PB6/PB7). Addresses confirmed by a bus scan.

| Device          | Purpose                       | Addr (7-bit) | Notes                                   |
|-----------------|-------------------------------|--------------|-----------------------------------------|
| AHT20           | Humidity + temperature        | `0x38`       | On a combo board with the BMP280        |
| SSD1306 OLED    | Display                       | `0x3C`       | 128×32                                  |
| AT24C32 EEPROM  | Non-volatile storage          | `0x57`       | On the DS3231 breakout                  |
| DS3231          | Real-time clock               | `0x68`       | ZS-042-style breakout, battery-backed   |
| BMP280          | Pressure + temperature        | `0x77`       | Not the common `0x76`                   |
| PCF8574 (1602A) | Alternate 16×2 LCD backpack   | `0x27`       | Owned, not wired                        |

- **AHT20 + BMP280** are one combo board exposing two independent I2C devices.
- **AT24C32 + DS3231** share one breakout: unplugging the RTC module removed both
  `0x68` and `0x57` from the scan.

## SSD1306 OLED

Source: Solomon Systech *SSD1306* datasheet, Rev 1.1, Apr 2008.

- 128×32 pixels, I2C.
- Logic supply VDD 1.65–3.3 V; absolute max +4 V (Tables 11-1, 12-1).
- I2C input pins: absolute max VDD + 0.3 V (Table 11-1).
- Panel voltage (7–15 V) is generated on-chip by a charge pump from 3.3–4.2 V.

## DS3231 RTC

- Time registers start at `0x00`: seconds, minutes, hours, day-of-week, date,
  month, year — 7 bytes, BCD-encoded.
- Keeps time on its own crystal and backup battery while the MCU is off.

## AHT20 — humidity + temperature

Source: ASAIR *Data Sheet AHT20*, V1.0, May 2021. VDD 2.2–5.5 V.

| Item                | Detail                                                        | §       |
|---------------------|---------------------------------------------------------------|---------|
| Power-on delay      | ≥100 ms before any command                                     | 7.1/7.4 |
| Status bit 7        | Busy — 1 = measuring, 0 = idle                                 | Table 9 |
| Status bit 3        | Calibrated — 1 = calibrated                                    | Table 9 |
| Calibration check   | `status & 0x18 != 0x18` → initialize registers `0x1B`, `0x1C`, `0x1E` | 7.4 |
| Trigger measurement | `0xAC, 0x33, 0x00`                                             | 7.4     |
| Measurement time    | 80 ms, then confirm bit 7 is 0                                 | 7.4     |
| Read back           | 6 bytes (status + 5 data); optional 7th CRC byte               | 7.4     |
| CRC                 | init `0xFF`, polynomial `1 + X⁴ + X⁵ + X⁸`                     | 7.4     |
| Humidity            | `RH[%] = S_RH / 2²⁰ × 100`                                     | 8.1     |
| Temperature         | `T[°C] = S_T / 2²⁰ × 200 − 50`                                 | 8.2     |

Byte layout: `status | hum | hum | hum·temp (split nibble) | temp | temp | [crc]`.
Both raw values are 20-bit.

Common write-ups get three things wrong: 40 ms power-on (datasheet ≥100 ms),
`0xBE, 0x08, 0x00` init (that is the older AHT10 command), and 7 mandatory bytes
(datasheet: 6, CRC optional).

## BMP280 — pressure + temperature

- Chip ID register `0xD0` reads `0x58`.
- Factory calibration coefficients `dig_T1..T3`, `dig_P1..P9` start at `0x88`;
  some are `int16_t`, some `uint16_t`.
- Temperature compensation produces `t_fine`, which the pressure formula needs.
