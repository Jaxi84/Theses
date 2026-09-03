# IoT Nightlight with Telegram

An ESP8266-based IoT nightlight controlled through a Telegram bot. This was one of my first projects involving bots, microcontrollers and IoT.

The project was developed at OSAO in 2021 as part of the Vocational Qualification in Information and Communications Technology. The physical nightlight was used in my internship workplace and is still in use today.

The implementation was created before I had formal programming experience or access to AI-assisted development. It is intentionally preserved as an educational project and as a record of learning by studying examples, documentation and existing libraries.

## Features

The Telegram bot provides an inline keyboard with the following functions:

- Turn the nightlight on or off.
- Turn the light on for 10, 20 or 30 minutes.
- Read the connected temperature sensor.
- Read the light-dependent resistor (LDR) and report whether the environment is dim or bright.
- Turn the light on when the LDR reports a dim environment during a light check.

The device connects to Wi-Fi and communicates with Telegram over an encrypted HTTPS connection. Telegram messages are checked using long polling.

## Hardware and Pin Assignments

The source code is written for an ESP8266 board. The exact board model and external circuit are not documented in this repository, so the pin assignments must be verified against the original hardware before reuse.

| Signal | ESP8266 pin in the code | Purpose |
| --- | ---: | --- |
| Nightlight output | GPIO 4 | Controls the light. |
| Temperature sensor control | GPIO 5 | Enables or disables the temperature sensor circuit. |
| LDR control | GPIO 2 | Enables or disables the LDR circuit. |
| Analog measurement | `A0` | Shared analog input for the sensors. |

Because the ESP8266 provides only one analog input, the temperature sensor and LDR are activated separately and measured through the same `A0` input.

## Technologies and Libraries

- ESP8266 Arduino core
- Arduino/C++
- `ESP8266WiFi`
- `WiFiClientSecure`
- `ArduinoJson`
- Universal Telegram Bot library
- Telegram Bot API
- HTTPS/TLS certificate validation
- NTP time configuration for secure connections

## How It Works

1. During startup, the ESP8266 configures its GPIO pins and connects to the configured Wi-Fi network.
2. A trusted root certificate is configured for the Telegram HTTPS connection.
3. The bot uses long polling to request new Telegram messages.
4. Normal commands such as `/start` and `/options` return bot instructions or the inline keyboard.
5. Button presses arrive as callback queries and are mapped to light control, timer, temperature or LDR operations.
6. The main loop checks whether the active timer has expired and turns the light off when necessary.

## Configuration

The original sketch expects these values to be configured in the source code:

- Wi-Fi SSID
- Wi-Fi password
- Telegram bot token

The placeholder values in the repository are not valid credentials. A Telegram bot must be created through BotFather, and the selected Wi-Fi network must be available to the device.

Required Arduino libraries and their compatible versions should be installed through the Arduino IDE or another ESP8266 development environment. The exact dependency versions are not recorded in this archive.

## Attribution

The project is based on examples and the Universal Telegram Bot library by Brian Lough:

- [Universal Telegram Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- [Simple Home Automation With Telegram](https://github.com/witnessmenow/Simple-Home-Automation-With-Telegram)

The original sketch also references video tutorials that influenced the project idea. External libraries, examples and tutorials should be used according to their respective licenses and attribution requirements.

## Limitations and Known Issues

This project worked for its original practical purpose, but it should not be considered production-ready by current standards without further development:

- Wi-Fi credentials and the bot token are stored as compile-time constants. Secrets should be kept outside the source code and rotated if exposed.
- There is no allowlist of Telegram users or chat IDs. Anyone who can interact with the bot may be able to control the light and request sensor readings.
- The device waits indefinitely for Wi-Fi during startup and has no robust reconnection strategy.
- Telegram API errors, sensor failures and other runtime errors receive limited handling.
- The LDR is checked only when the corresponding Telegram button is pressed; the device does not continuously adjust the light automatically.
- The temperature conversion depends on the exact sensor, ADC range and ESP8266 board. These electrical assumptions are not documented or calibrated in the repository.
- The timer uses a simple `millis()` comparison and does not explicitly handle counter rollover.
- A blocking `delay(500)` is used while preparing an LDR measurement.
- The code contains unused variables and variable shadowing that make maintenance harder.
- The circuit diagram, bill of materials, enclosure details and installation instructions are missing.
- Dependency versions, automated tests, structured logging, watchdog handling and update procedures are not documented.
- The project uses a third-party library and tutorial-based implementation, so the original licenses should be checked before redistribution.

## Project Status

Archived educational project from 2021. The physical nightlight remains in use, which demonstrates that the implementation has been practically useful over a long period. The source code is preserved as historical material and should be reviewed and modernized before being reused in a new deployment.

## License

No license is specified for this project. The licenses of the third-party libraries and referenced examples remain applicable to their respective contents.
