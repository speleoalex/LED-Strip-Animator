# LED Strip Animator

A complete system for creating and playing animations on NeoPixel/WS2812B LED strips, consisting of Arduino firmware and a web-based design interface.

## Description

LED Strip Animator allows you to:

- **Visually design** LED animation sequences through an intuitive web interface
- **Export** animations to INI files to load onto an SD card
- **Play** animations on NeoPixel LED strips using Arduino

The system reads animation patterns from an SD card, allowing you to change animations without reprogramming the microcontroller.

## Architecture

```
LED-Strip-Animator/
├── firmware/           # Arduino code for LED control
│   └── multiled.ino    # Main sketch
├── interface/          # Web app for animation design
│   ├── index.html      # Main page
│   ├── js/main.js      # Application logic
│   └── css/            # Styles
└── presets/            # Pre-configured animation examples
    └── fulmine.ini     # "Lightning" animation
```

## Required Hardware

- **Arduino** (Uno, Mega, or compatible with ATmega328p/ATmega2560)
- **NeoPixel LED Strip** (WS2812B) - supports up to 1000 LEDs
- **SD Card Module** - for loading configuration files
- **Power Supply** adequate for the LED strip length

### Wiring Diagram

| Component | Arduino Pin |
|-----------|-------------|
| NeoPixel Data | D4 |
| SD Card CS | D6 |
| SD Card MOSI | D11 (standard SPI) |
| SD Card MISO | D12 (standard SPI) |
| SD Card SCK | D13 (standard SPI) |

## Installation

### Arduino Firmware

1. Install the required libraries in the Arduino IDE:
   - [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
   - [SdFat](https://github.com/greiman/SdFat)

2. Open `firmware/multiled.ino` in the Arduino IDE

3. Select the correct board (Arduino Uno/Mega)

4. Upload the sketch to the board

### Web Interface

The web interface requires no installation. You can:

- Open `interface/index.html` directly in your browser
- On Linux, use the `run-linux.sh` script to launch in Chrome app mode

```bash
cd interface
./run-linux.sh
```

## Usage

### 1. Create an Animation

1. Open the web interface (`interface/index.html`)
2. Set the **number of LEDs** on your strip
3. Set the **number of steps** in the animation
4. Set the **interval** in milliseconds between steps
5. Select a color from the palette
6. Click on LEDs in the grid to color them
7. Use **Preview** to see the animation in real-time
8. Click **Save** to download the INI file

### 2. Load the Animation

1. Rename the downloaded file to `leds.ini`
2. Copy the file to the root of the SD card
3. Insert the SD card into the module connected to Arduino
4. Power on Arduino - the animation will start automatically

## INI File Format

The configuration file has this format:

```ini
interval=100          # Milliseconds between each step
numleds=14            # Total number of LEDs
numsteps=30           # Number of steps in the animation

# Format: stepN_ledM=R,G,B
step1_led1=255,255,0  # Step 1, LED 1: yellow
step1_led2=255,0,0    # Step 1, LED 2: red
step2_led1=0,255,0    # Step 2, LED 1: green
...
```

## Included Examples

- **fulmine.ini** - Lightning animation that scrolls along the LED strip

## Firmware Build (Alternative)

To compile via command line using the Makefile:

```bash
cd firmware/Arduino
make build        # Compile the firmware
make upload       # Upload to Arduino
make clean        # Clean compiled files
```

## Dependencies

### Firmware
- Arduino Core
- Adafruit NeoPixel Library
- SdFat Library

### Web Interface
- jQuery 1.11.2 (included)
- Bootstrap 3.x (included)

## Compatibility

- **Microcontrollers**: ATmega328p (Arduino Uno), ATmega2560 (Arduino Mega), ATtiny85 (Trinket)
- **Supported LEDs**: WS2812B, NeoPixel, and compatible (NEO_GRB + NEO_KHZ800)
- **Browsers**: Chrome, Firefox, Safari, Edge (for web interface)

## Known Limitations

- Maximum 1000 LEDs supported (modifiable in code)
- SD card must be formatted FAT16/FAT32
- File must be named `leds.ini` and located in the SD root

## License

GPLv3 - See source code for details.

## Author

Alessandro Vernassa (speleoalex@gmail.com)

## Contributing

1. Fork the repository
2. Create a branch for your feature (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push the branch (`git push origin feature/new-feature`)
5. Open a Pull Request
