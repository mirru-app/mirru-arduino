This Arduino sketch was made to be used in conjunction with the [Mirru Android App](https://github.com/mirru-app/mirru-android)

# For Users
Want to print and assemble your own hand? Visit our [guide](https://github.com/mirru-app/mirru-arduino/wiki)

# For Development

## Supported hardware

This sketch is currently designed to work with the following Arduino boards:
  * [adafruit Feather ESP32](https://www.adafruit.com/product/3405)

## Installation

### Command Line
To build this sketch on the command line, you will need the following tools installed. Please see their respective installation guides for instructions on how to install them.

  1. [make](https://www.gnu.org/software/make/) - Installation of `make` is OS-dependent. The provided link includes a download page for installing make on OSX and Linux. (See [here](https://stackoverflow.com/questions/32127524/how-to-install-and-use-make-in-windows) for more info on installing on Windows).
  1. [python3](https://www.python.org/downloads/) - Used internally by some of the Arduino libraries
  1. [arduino-cli](https://arduino.github.io/arduino-cli/latest/)


With those installed you can build the sketch with:

```shell
$ make build
```
For deploying the sketch please see the additional info on the [arduino-cli site](https://arduino.github.io/arduino-cli/latest/getting-started/#compile-and-upload-the-sketch).

### Arduino IDE

This can also used with the IDE. Just open the sketch in the arduino IDE.
