# AVR

Collection of drives and examples for AVR MCUs.

## About

Main idea is to develop drives for different devices, sensors and etc. The goal is to have libraries who are  easily portable to other MCUs. Most of the stuff found here is tested and well documented.

Most of the time I am using Arduino UNO (Atmega328p) for developing, so almost all core drives (SPI, USART, etc.) found here are written for this microcontroller. I use Arduino platform because it is easier and faster to build a prototype. Even though I use this platform, entire code found here is based on low-level bare-metal C programming.


## Prerequisites

To be able to use stuff found in this repository you should have avr-gcc compiler and avr-dude in-system programmer.

```
sudo apt-get install arduino
sudo apt-get install arduino-core
```

## How to use

First step is to clone this repository to you PC.
```
git clone "https://github.com/semir-t/avr.git"
```
Makefile which I use is directory dependent. This means that to use Makefile without any changes, directory of your project must be located in the examples directory. Other possibility is to create new directory (for example: my_projects) and store your projects in that directory.

There are 3 directories used for storing source and header files:
* Local directory
* Extra directory
* Core directory

### Local directory
Local source and header files found in the same directory as main.c and Makefile. Great way to use your personal libraries.
### Extra directory
Directory for storing libraries which you use all the time. In my case, I use print.c library in all my projects for debugging and because of that I have it stored in this directory.
### Core directory
This directory stores mcu dependent drives. For example:
* SPI drive
* USART drive
* I2C drive and etc.


## Authors

* **Semir Tursunovic** - semir-t

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details


