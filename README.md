# JET Banner

**JET Banner** is a lightweight, persistent classification banner for Windows environments. It displays a customizable banner at the top of the screen that indicates the system's security or usage classification. Designed for IT administrators and security-conscious organizations, JET Banner runs silently at login and can be centrally managed.

---

## Features
- Persistent top-of-screen banner
- Configurable banner text, background color, and font color via `config.txt`
- Automatically reserves space at top of screen (AppBar style)
- Multi-user startup support
- Lightweight EXE with no dependencies
- Runs completely offline with no Internet requirements

---

## Installation
Use the provided `JETBannerSetup.exe` installer to:
- Install to `C:\Program Files\JETBanner`
- Automatically run at startup for all users

The installer will:
- Copy `JETBanner.exe` and `config.txt` to Program Files
- Create a shortcut in the system-wide Startup folder

---

## Configuration
Edit `config.txt` in the install directory to control the banner:

```
color=0,0,0
fontcolor=255,255,255
text=TEST
```
- `color` sets background RGB
- `fontcolor` sets text RGB
- `text` sets the displayed banner label

Changes take effect after restarting the app or logging in again.

---

## License
This project is licensed under the [Apache License 2.0](LICENSE). However, the name **"JETBanner"** and visual branding remain the intellectual property of **JET Systems**.

We kindly request that modified versions retain attribution and are not redistributed under a different name without permission.

For commercial use or inquiries, please contact: **JET Systems** (https://jet.systems).
