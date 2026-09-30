# Goal

Read the AHT20 (0x38) and BMP280 (0x77) and display their values on the OLED
alongside the clock.

**Current step: 3 of 7**

- [x] 1. Bus scan — confirm addresses
      Found: AHT20 0x38, OLED 0x3C, AT24C32 0x57, DS3231 0x68, BMP280 0x77
- [x] 2. AHT20 driver — `aht20.h/.c`, init + read
- [ ] 3. Wire AHT20 into `main.c`      ← current
- [ ] 4. OLED display integration (AHT20)
- [ ] 5. BMP280 driver — id check, calibration, compensation
- [ ] 6. Wire BMP280 into `main.c` + OLED
- [ ] 7. Update CLAUDE.md module status

## Notes

- Design agreed for step 2: no status enum — both functions return
  `HAL_StatusTypeDef`. `aht20_t` {float temperature, humidity},
  `aht20_init(hi2c)` + `aht20_read(hi2c, &out)`.
- Design change (2026-09-29): no file-scope static handle. Every function
  takes the I2C handle as a parameter, so several sensors can each be
  driven through their own bus.
- Step 2 sections: header, init, read, conversion.
- Scope cut (2026-09-29): no calibration check. `aht20_init` only stores
  the handle and waits out power-up; goal is a working reading first.
- Reorder (2026-09-30): BMP280 deferred until the AHT20 is wired in and on
  the OLED. Old step 4 ("Combine + wire") split into steps 3 and 6.
