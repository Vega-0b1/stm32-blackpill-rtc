# Goal

Read the AHT20 (0x38) and BMP280 (0x77) and display their values on the OLED
alongside the clock.

**Current step: 5 of 8**

- [x] 1. Bus scan — confirm addresses
      Found: AHT20 0x38, OLED 0x3C, AT24C32 0x57, DS3231 0x68, BMP280 0x77
- [x] 2. AHT20 driver — `aht20.h/.c`, init + read
- [x] 3. Wire AHT20 into `main.c`
- [x] 4. DS3231 refactor — modular driver
- [ ] 5. OLED display integration (AHT20)      ← current
- [ ] 6. BMP280 driver — id check, calibration, compensation
- [ ] 7. Wire BMP280 into `main.c` + OLED
- [ ] 8. Update CLAUDE.md module status

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
- Step 5 layout (2026-09-30): smaller font, three rows — time, date,
  temperature/humidity — on the 128x32 OLED.
- Step 5 sections: font and rows, header, print function, float printing,
  main call.
- Reorder (2026-10-01): DS3231 refactor inserted as step 4, ahead of the
  OLED work. AHT20 values not yet checked on hardware — do that in step 5.
  Step 5's "font and rows" section was locked in, with no edits made.
- Step 4 sections: handle parameter, status returns, field math,
  main cleanup.
- Step 4 field math design (2026-10-01, replaces the earlier edit-module
  design): no new module. `ds3231.h` gains a `ds3231_field_t` enum
  (hours, minutes, seconds, month, date, year) and
  `void ds3231_increment(ds3231_time_t *time, ds3231_field_t field)` —
  pure math on the struct: +1 with wrap-around, month lengths, leap years.
  No buttons, OLED, or I2C. `main.c` reads buttons, tracks the selected
  field, calls increment, then `ds3231_write` and `oled_print_rtc`.
  The old UP-on-seconds behaviour (reset to 0) becomes main's choice.
  `ds3231_edit.h/.c` are not needed — delete them.
  Enum order is struct order and carries no meaning; the edit order is
  main's choice, kept as its own array of `ds3231_field_t` in screen order.
- Step 4 done (2026-10-02): verified by a clean `cmake --build build/Debug`.
  Not yet run on hardware. UP on seconds now increments (wraps at 60)
  instead of the old reset-to-0. The plain `make` Makefile is stale: it
  does not list the app sources and fails to link; CMake is the build.
