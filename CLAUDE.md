# BlackPill — Project Guide

STM32F411CEU6 (WeAct BlackPill) real-time clock. Reads time/date from a DS3231
over I2C and displays it on an SSD1306 OLED. Two buttons edit each field
on-device via a 7-state machine in `main.c`.

## Working Method

This project is a learning exercise. The user writes the firmware. Work is driven
by a goal with numbered steps tracked in `GOAL.md`, and code is released only
through the help ladder below.

### Goal tracking

G1. IF a session begins THEN read `GOAL.md` and state the current step before anything else.
G2. IF `GOAL.md` has no goal set THEN ask the user for the goal, propose the steps, write them to `GOAL.md`, and STOP until the user confirms.
G3. IF composing any response THEN end it with the current-step footer.
G4. Footer format, exactly:
```
▸ Step N/M: <step title>
  Next: <the single next action>
```
G4a. IF writing the footer's `Next:` line THEN state one concrete action in plain language, naming the actual identifier or file involved.
     // FAILS:  "rename to break the shadow, then store the handle"
     // PASSES: "rename the static to `i2c_handle`, then assign the parameter to it"
G4b. Do NOT use a technical term in the footer that was not defined earlier in the same response.
G5. IF the user's message is unrelated to the current step THEN answer it fully, then apply G3. Do NOT refuse, defer, or shorten the answer.
    // Commentary: the footer is the anti-drift mechanism, not a gate on questions.
G6. IF the current step's work is complete AND verified THEN tick it in `GOAL.md`, advance the current-step pointer, and state the new step.
G7. IF a step is not yet verified THEN do NOT tick it.
    // Commentary: "it builds" is verification; "I wrote it" is not.
G8. IF the user says "status" or "where am I" THEN display the full step list with the current step marked.
G9. IF the user changes the goal or reorders steps THEN rewrite `GOAL.md` and confirm the new list in one line.
G10. IF the user abandons the goal THEN say so plainly in `GOAL.md` rather than silently leaving stale steps.

### Help ladder

A "section" is a sub-part of the current step. Step 2's sections, for example:
header, init, read, conversion.

H1.  IF "help" AND no section is locked in THEN list the sections needed for the current step, one per line, name only. Give no implementation content.
H2.  IF listing sections THEN state names only — no description of how to implement them, no data structure or algorithm names.
H3.  IF the user names one of the listed sections THEN lock in to that section.
H4.  IF "help" AND a section is locked in THEN give exactly one pseudocode step aimed at the first blocker in that section. Never emit code.
H4a. IF giving a hint at ANY tier THEN target the first missing or wrong piece in **file order**, reading top to bottom. Do NOT hint at a later piece while an earlier one is absent.
     // Commentary: a hint aimed at the middle of an empty file produces code that cannot compile — the user writes a declaration whose types do not exist yet.
     // FAILS:  header is empty → hinting `aht20_status_t aht20_init(...);` before the include guard, the include, and the enum.
     // PASSES: header is empty → hint the include guard, then the include, then the enum, then the struct, then the declarations.
H4b. H4a overrides H5 and H7. Establish file order first; describe or emit the signature only once everything above it is present.
H4c. IF a hint uses a technical term (shadowing, linkage, promotion, aliasing) THEN define it in plain language in the same response, the first time it appears.
     // Commentary: a hint the user has to decode is not a hint. Naming the concept is useful; naming it without defining it is not.
H4d. IF a hint names an action THEN name the identifier, file, or line it applies to. Do NOT describe the action abstractly.
H5.  IF H4 fires AND no function signature exists yet for the locked-in section THEN instead state in prose what the function takes and what it returns. H5 overrides H4.
H6.  IF "help" again on the same locked-in section THEN give the NEXT single pseudocode step at the same tier. Do NOT escalate tiers on repeated "help".
H7.  IF "help+" THEN give exactly one code line, or the function signature if that is what is missing. One line only, no surrounding body.
H8.  IF "help+" again on the same locked-in section THEN give the NEXT single code line. Do not give more than one line per request.
H9.  IF "help++" THEN give the complete code for the locked-in section only. Do NOT write any other section.
H10. IF "help+" or "help++" is used AND no section is locked in THEN list the sections (H1) and ask which one. Do not give code.
H11. IF the user says "list" THEN clear the locked-in section and re-list per H1.
H12. IF the user says "stuck" THEN treat it as "help".
H12a. IF the user says "next" THEN re-read the locked-in section from disk and check the work written so far against that section's requirements.
H12b. IF the H12a check passes AND pieces remain in the section THEN say so in one line and give the next hint at the current tier.
H12c. IF the H12a check fails THEN name the defect and why it is a defect, then give a hint aimed at that defect per H4.
      // Commentary: "next" is a check plus a hint, not a skip. A failed check must not advance past the broken piece.
H12d. IF the H12a check passes AND no pieces remain in the section THEN state the section is complete and list the remaining sections per H1.
H12e. IF "next" arrives AND no section is locked in THEN list the sections per H1.
H12f. "next" does not change the help tier. It repeats the tier last used in this section, defaulting to "help".
H13. IF giving help THEN use the identifiers, signature, and style already present in the user's files. Do NOT rename their variables or functions.
H14. IF a part of the locked-in section is already written THEN do not give it as a hint. Give the first part that is missing or wrong.
H15. IF no help tier has been requested THEN do NOT show code. Explain the concept in prose and stop.
H16. A reference fact — a header name, type name, register address, command byte — is not code. Answer it directly under H15.
     // Example: "what bytes trigger a measurement?" → `0xAC, 0x33, 0x00`. Allowed.
     //          Showing the `aht20_read()` body that sends them. Not allowed.
H17. H1–H14 override H15.
H18. IF the user pastes code they wrote THEN review it: state what is correct, name each defect and why. Do NOT rewrite it unless a help tier was requested.
H19. IF any condition not covered by G1–G10 or H1–H18 (including lettered sub-rules) arises THEN stop, describe the situation to the user, and ask how to proceed. Do not improvise.

- **Framework:** STM32Cube HAL (generated from `BlackPill.ioc`). Not Arduino.
- **MCU clock:** HSE + PLL → 96 MHz (`SystemClock_Config` in `main.c`).
- **I2C:** `hi2c1` on PB6 (SCL) / PB7 (SDA), 100 kHz, shared bus.
- **Buttons:** MODE = PA0, UP = PA7, internal pull-ups, active-low to GND.

### I2C bus devices
All share `hi2c1` (PB6/PB7). Addresses below are **confirmed by an `i2c_scan` sweep**, not assumed.

| Device          | Purpose                       | Addr (7-bit) | Status                    |
|-----------------|-------------------------------|--------------|---------------------------|
| AHT20           | Humidity + temperature        | `0x38`       | Driver in progress        |
| SSD1306 OLED    | Display                       | `0x3C`       | Active (`oled`)           |
| AT24C32 EEPROM  | NV storage (on DS3231 module) | `0x57`       | Present, unused           |
| DS3231          | RTC                           | `0x68`       | Active (`ds3231`)         |
| BMP280          | Pressure + temperature        | `0x77`       | Wired, no driver yet      |
| PCF8574 (1602A) | Display (alt)                 | `0x27`       | Optional (`lcd`), unwired |

Notes from the scan:
- **BMP280 is at `0x77`, not `0x76`.** The AHT20+BMP280 is one combo board exposing two independent I2C devices (AHT20 `0x38`, BMP280 `0x77`), wired to the shared bus + VDD/GND. No firmware driver yet.
- **`0x57` is an AT24C32 EEPROM riding on the DS3231 breakout** (the ZS-042-style board bundles both). Confirmed: unplugging the RTC module dropped both `0x68` and `0x57`. Free non-volatile storage on the bus; currently unused.
- Re-run the scan with `i2c_scan(&hi2c1, buf, sizeof buf)` (see `i2c_util`) whenever a device goes silent.

## Modules

Application drivers live in `Core/Src` + `Core/Inc`. Each is a thin, self-contained
unit with a small public API.

> **Naming convention:** lowercase `snake_case` for functions, variables, and types; `ALL_CAPS` only for macros (`DS3231_ADDR`, `LCD_ADDR`) and enum constants (`BUTTON_MODE`, `BUTTON_UP`). Type names keep a `_t` suffix (`ds3231_time_t`, `Button_t`). External `HAL_*` / `ssd1306_*` symbols are left as their libraries define them.

### ds3231 — RTC read/write + field editing
`ds3231.h`
- `ds3231_time_t` — struct: `seconds, minutes, hours, date, month, year` (all `uint8_t`).
- `ds3231_read(ds3231_time_t *t)` / `ds3231_write(ds3231_time_t *t)` — bulk read/write over I2C (addr `0x68`).
- `ds3231_adjust_hours/minutes/seconds/month/date/year(rtc, &curr_state)` — one function per edit state; each handles UP-button increment, wrap-around, and advancing `curr_state` on MODE. Month/date/year enforce per-month day limits and leap years.

### oled — active display (SSD1306, in use)
`oled.h` — depends on `ds3231.h`
- `oled_init(I2C_HandleTypeDef *hi2c)`
- `oled_set_cursor(row, col)`, `oled_print(const char *)`, `oled_clear()`
- `oled_print_rtc(ds3231_time_t *rtc, uint8_t curr_state)` — renders the clock; highlights the field being edited based on `curr_state`.
- Wraps the vendored `ssd1306` library below.

### lcd — alternate display (1602A over PCF8574, OPTIONAL / not compiled in)
`lcd.h` — depends on `ds3231.h`
- API mirrors `oled` exactly: `lcd_init`, `lcd_set_cursor`, `lcd_print`, `lcd_clear`, `lcd_print_rtc`.
- **Kept on purpose as a drop-in alternative** — for reusing in another project, or switching this one back to a 1602A LCD (I2C backpack addr `0x27`). Not a leftover; do not delete.
- Because the API matches `oled` 1:1, swapping displays is a header + call-name change, not a rewrite. See "Switching the display" below.

### button — debounced input
`button.h`
- `Button_t` enum: `BUTTON_MODE`, `BUTTON_UP`.
- `button_init()` — configures PA0/PA7 with pull-ups.
- `button_pressed(Button_t)` — returns 1 on a debounced press edge.

### i2c_util — bus diagnostics
`i2c_util.h`
- `i2c_scan(I2C_HandleTypeDef *hi2c, uint8_t *out, uint8_t max)` — probes 7-bit addresses `0x08..0x77`, writes each responder (7-bit) into `out` up to `max`, returns the total count found. Bus-agnostic (takes the handle), caller owns the buffer.
- Not called from `main()` in normal builds; invoke from a breakpoint or the dap/gdb REPL when a device goes silent. Used to confirm the address map above.

### aht20 — humidity + temperature (IN PROGRESS)
`aht20.h` — addr `0x38`. Agreed design, not yet implemented:
```c
typedef enum {
  AHT20_OK = 0,
  AHT20_ERR_I2C,          /* no ACK / HAL transfer failed */
  AHT20_ERR_BUSY,         /* measurement not ready yet */
  AHT20_ERR_UNCALIBRATED  /* init: cal bit never came up */
} aht20_status_t;

typedef struct { float temperature; float humidity; } aht20_data_t;

aht20_status_t aht20_init(I2C_HandleTypeDef *hi2c);
aht20_status_t aht20_read(aht20_data_t *out);
```
- `aht20_init` stashes the handle in a file-scope `static` (same pattern as `oled_init`), waits for power-up, checks the calibrated status bit, sends the init command if needed.
- `aht20_read` triggers a measurement, waits ~80 ms, reads 7 bytes, unpacks two **20-bit** raw values, converts.

**Protocol — verified against the primary datasheet.**
Source: ASAIR *Data Sheet AHT20*, Version V1.0, May 2021 (16 pp), §7.1, §7.3, §7.4, §8.
VDD range 2.2–5.5 V.

| Phase | Detail | §  |
|---|---|---|
| I²C address | `0x38` 7-bit (`0b0111000x` + R/W bit) — matches the bus scan | 7.3 |
| Power-on delay | **≥100 ms** to reach idle before any command | 7.1 / 7.4 |
| Status register | read command `0x71` | 7.4 |
| Status bit 7 | **Busy** — 1 = measuring, 0 = idle | Table 9 |
| Status bit 3 | **CAL Enable** — 1 = calibrated, 0 = uncalibrated | Table 9 |
| Calibration check | `status & 0x18 != 0x18` → initialize registers `0x1B`, `0x1C`, `0x1E` | 7.4 |
| Trigger measurement | wait 10 ms, then `0xAC, 0x33, 0x00` | 7.4 |
| Measurement wait | **80 ms**, then confirm status bit 7 is 0 before reading | 7.4 |
| Read back | **6 bytes** (status + 5 data). A **7th CRC byte** is optional — ACK the 6th to get it | 7.4 |
| CRC | init `0xFF`, polynomial `1 + X⁴ + X⁵ + X⁸` | 7.4 |
| Humidity | `RH[%] = (S_RH / 2²⁰) × 100` | 8.1 |
| Temperature | `T[°C] = (S_T / 2²⁰) × 200 − 50` | 8.2 |

Byte layout: `status | hum | hum | hum·temp (split nibble) | temp | temp | [crc]`.
Both raw values are **20-bit**, sharing the middle byte.

> **Where the internet is wrong.** Widely-copied write-ups say *40 ms* power-on
> (datasheet: ≥100 ms), *`0xBE, 0x08, 0x00`* init (datasheet: registers `0x1B/0x1C/0x1E`
> when `status & 0x18 != 0x18` — `0xBE` is the older AHT10 command), and *7 mandatory
> bytes* (datasheet: 6, with CRC optional). Trust the datasheet.

Resolution 0.01 °C / 0.024 %RH. Use `f` suffixes throughout and avoid `pow()` — it
returns `double`; write `1048576.0f` for 2²⁰. The calibration check is only needed at
power-on, not per reading (§7.4 note).
- Address, status-bit handling, 20-bit unpacking and conversion math all stay `static` inside the `.c`.
- On error, leave `*out` untouched — do not clobber the caller's struct with zeros.

### bmp280 — pressure + temperature (PLANNED)
`bmp280.h` — addr **`0x77`** (confirmed by scan, not the more common `0x76`).
- Separate module from `aht20`: independent chips, and BMP280 needs persistent calibration state AHT20 does not.
- Init must read ~12 factory calibration coefficients (`dig_T1..T3`, `dig_P1..P9`) from `0x88` and keep them in a file-scope `static`. Mind the signedness — some are `int16_t`, some `uint16_t`.
- Sanity check: register `0xD0` must read `0x58`.
- Compensation: **temperature first** — it produces the `t_fine` intermediate that the pressure formula depends on.

> **Floating point on this MCU:** the F411's FPU is **single-precision only**. Use
> `float`, never `double`, and suffix every decimal literal with `f`
> (`200.0f`, not `200.0`). An unsuffixed literal is a `double` and silently drags
> the whole expression into slow software emulation. Same for math functions —
> `sqrtf()` not `sqrt()`.

### ssd1306 — vendored graphics library (do not hand-edit)
`ssd1306.h`, `ssd1306.c`, `ssd1306_fonts.*`, `ssd1306_conf.h`
- Upstream: afiskon/stm32-ssd1306. Primitives: `ssd1306_Init`, `ssd1306_Fill`, `ssd1306_UpdateScreen`, `ssd1306_WriteString`, shapes, bitmaps.
- Configure via `ssd1306_conf.h` (I2C vs SPI, dimensions, addr `0x3C`). Treat as third-party; the `oled` module is the intended app-facing wrapper.

## Switching the display (OLED ↔ LCD)

R1. IF using the OLED THEN in `main.c` include `oled.h` and call the `oled_*` functions (current state).
R2. IF switching to the 1602A LCD THEN replace `#include "oled.h"` with `#include "lcd.h"` and rename each `oled_*` call to its `lcd_*` counterpart. No other logic changes — the APIs are identical.
R3. IF adding a new display THEN give it the same 5-function surface (`*_init/set_cursor/print/clear/print_rtc`) so it stays swappable.
R4. IF only one display is wired THEN only its driver's `.c` needs to compile; the unused driver can remain in the tree unreferenced.

## Building & flashing

- Build: `make` (Makefile) or PlatformIO (`platformio.ini`, env `blackpill_f411ce`).
- Flash (ST-Link/OpenOCD):
  `openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/BlackPill.elf verify reset exit"`
- Or DFU: hold BOOT0 on reset, drag-and-drop the `.bin`.
- Regenerating from `BlackPill.ioc` overwrites code outside `/* USER CODE BEGIN/END */` guards — keep app logic inside them.

## Debugging (on-chip, via Neovim)

There **is** a full DAP setup — don't assume otherwise.

- **Debugger:** `nvim-dap` + `nvim-dap-cortex-debug` (+ dap-ui, dap-virtual-text).
- **Config source:** `/etc/nixos/nix-config/neovim.nix` (home-manager `programs.neovim`). The live `~/.config/nvim/init.lua` is generated Nix-store output — a **symlink**, so `grep -r` won't follow it; inspect `neovim.nix`. Any `lazy-lock.json` / `lua/plugins/*.lua` in `~/.config/nvim` are stale leftovers from an old lazy.nvim config, not active.
- **Stack:** `openocd` → `arm-none-eabi-gdb` (MI) → cortex-debug `debugadapter.js` (node, from `pkgs.vscode-extensions.marus25.cortex-debug`) → nvim-dap.
- **Glue:** a custom `embedded.lua` resolves the PlatformIO project, derives the OpenOCD target from the board manifest, rebuilds, then launches. `<F5>` auto-builds + flashes + attaches this project (`blackpill_f411ce`).
- **Keymaps:** `<F5>` start/continue · `<F10>` step over · `<F11>`/`<S-F11>` into/out · `<leader>db` breakpoint · `<leader>du` UI · `<leader>dr` REPL · `<leader>dc` run-to-cursor · `CortexDebugMemory` + RTT views.
- **Implication:** for on-chip inspection (e.g. reading a `found[]` array from an I2C bus scan), breakpoint + dap-ui variables/memory pane — no UART/printf needed just to see values.
