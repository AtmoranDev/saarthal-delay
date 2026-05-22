# Saarthal Delay

A high-quality delay pedal plugin (VST3) and standalone application for Windows, macOS, and Linux.

## Download

Grab the latest release for your platform from the [Releases](../../releases/latest) page.

## Installation

### VST3 Plugin

| Platform | Plugin folder |
|---|---|
| Windows | `C:\Program Files\Common Files\VST3\` |
| macOS | `~/Library/Audio/Plug-Ins/VST3/` |
| Linux | `~/.vst3/` or `/usr/lib/vst3/` |

Extract the zip and copy the `VST3/Saarthal Delay.vst3` folder to the appropriate path above.

### Standalone

Extract the `Standalone/` folder from the zip and run the executable directly — no install needed.

## Parameters

| Knob | Range | Description |
|---|---|---|
| Time | 10ms – 2s | Delay time |
| Feedback | 0 – 95% | Repeat decay |
| Mix | 0 – 100% | Dry/wet blend |
| Wow | 0 – 100% | Tape wow depth (Tape mode only) |
| Flutter | 0 – 100% | Tape flutter depth (Tape mode only) |

### Modes

- **Digital** — clean, transparent repeats (Boss DD-7 style)
- **Tape** — wow/flutter modulation with HF rolloff per repeat (Echoplex style)
- **Analog** — BBD bucket-brigade filtering with soft saturation on feedback (MXR Carbon Copy style)

## Building from Source

Requires CMake 3.22+ and a C++17 compiler. JUCE is fetched automatically.

```bash
git clone https://github.com/AtmoranDev/saarthal-delay.git
cd saarthal-delay
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Linux also requires:
```bash
sudo apt-get install libasound2-dev libx11-dev libxcomposite-dev libxcursor-dev \
  libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
  libfreetype6-dev libfontconfig1-dev libglu1-mesa-dev mesa-common-dev
```
