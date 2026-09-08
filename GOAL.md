# Goal

Read the AHT20 (0x38) and BMP280 (0x77) and display their values on the OLED
alongside the clock.

**Current step: 2 of 6**

- [x] 1. Bus scan — confirm addresses
      Found: AHT20 0x38, OLED 0x3C, AT24C32 0x57, DS3231 0x68, BMP280 0x77
- [ ] 2. AHT20 driver — `aht20.h/.c`, init + read      ← current
- [ ] 3. BMP280 driver — id check, calibration, compensation
- [ ] 4. Combine + wire into `main.c`
- [ ] 5. OLED display integration
- [ ] 6. Update CLAUDE.md module status

## Notes

- Design agreed for step 2: `aht20_status_t` enum, `aht20_data_t`
  {float temperature, humidity}, `aht20_init(hi2c)` + `aht20_read(&out)`.
- Step 2 sections: header, init, read, conversion.
