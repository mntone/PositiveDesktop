# Positive☆Desktop

"Positive☆Desktop" is reworked virtual desktop utility for Windows 10 and 11. The source code is written in native (C++), so footprint is very small.

https://user-images.githubusercontent.com/901816/209556339-4c579869-0df4-44a7-974c-ef1b72b294ef.mp4

## Features

- [x] Show current desktop information
  - [x] View desktop name
  - [x] View desktop index
- [x] Set notification position
- [x] Apply window corner (square, round, small round) to notification window (Windows 11)
- [x] Listen for key input from global hook (about keys in the following section for experimental version)

### Shortcut keys

In the future, you will be able to customize keys.

- `Win`+`Ctrl`+`X`: Exit application.
- `Win`+`Ctrl`+`I`: Show settings window.
- `Win`+`Ctrl`+`1` to `9` and `0`: Switch "Desktop 1" to "Desktop 9" and "Desktop 10."
- `Win`+`Ctrl`+`Home`: Switch the first desktop.
- `Win`+`Ctrl`+`End`: Switch the last desktop.
- `Win`+`Ctrl`+`PgUp`: Switch the left desktop (`Win`+`Ctrl`+`←` is provided by Windows default).
- `Win`+`Ctrl`+`PgDn`: Switch the right desktop (`Win`+`Ctrl`+`→` is provided by Windows default).
- `Win`+`Ctrl`+`Alt`+`1` to `9` and `0`: Move foreground window to "Desktop 1" to "Desktop 9" and "Desktop 10" and switch it.
- `Win`+`Ctrl`+`Alt`+`Home`: Move foreground window to the first desktop and switch it.
- `Win`+`Ctrl`+`Alt`+`End`: Move foreground window to the last desktop and switch it.
- `Win`+`Ctrl`+`Alt`+`←` or `PgUp`: Move foreground window to the left desktop and switch it.
- `Win`+`Ctrl`+`Alt`+`→` or `PgDn`: Move foreground window to the right desktop and switch it.
- `Win`+`Ctrl`+`Alt`+`D`: Move foreground window to the new desktop and switch it.
- `Win`+`Ctrl`+`Alt`+`T`: Pin and unpin foreground window on the current desktop.

## Requirements

Positive☆Desktop have the following system requirements:

- Windows 10 version 1809 (build 17763) or later.

## Development

It runs on Windows 10 version 1809 or later.

You'll need the following software:

- Visual Studio 2022 Version 17.4 or later (no test in earlier version)
  - Desktop development with C++
  - .NET SDK (Use the build for DesktopBridge app; this option includes .NET 6 & 7 Runtime)
  - Test Adapter for Google Test (option)
- Windows 10 SDK, version 1809 (10.0.17763.0)
  https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/

1. Clone the repository:

   ```sh
   git clone https://github.com/mntone/PositiveDesktop.git
   ```
2. Open the `PositiveDesktop.sln` file.
3. Open the `PositiveDesktopPackage/Package.appxmanifest` file on Visual Studio.
4. Go the "Packaging" tab, click "Choose Certificate...", and click "Create".

## Authors

- mntone - Initial work.

## License

Positive☆Desktop is licensed under the GPLv3 license - see the [LICENSE.txt](https://github.com/mntone/PositiveDesktop/blob/master/LICENSE.txt) file for details.

## Acknowledgements

I wish to thank the flow concept given by [SylphyHorn](https://github.com/Grabacr07/SylphyHorn).

Also, I wish to thank the app name inspired by vocaloid music ''[Positive☆Dance Time](https://youtu.be/FT91CrPPAqc)''.
