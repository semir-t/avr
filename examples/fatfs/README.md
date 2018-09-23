# FAT FS

FAT FS for MMC/SD card

## About

Made a few modifications to the original [MMC/SD card drive](https://github.com/semir-t/avr/tree/master/examples/mmc_sd) so that it could be used as a core drive for Elm Chans [FatFS](http://elm-chan.org/fsw/ff/00index_e.html)

Current version is only working for MMC cards.

Steps to take to enable SDv1 cards:
* Update mmc_ioctl() [mmc.c]

Steps to take to enable SDv2 cards:
* Update mmc_ioctl() [mmc.c]



## Porting drive to other MCU
For this drive to work on other MCUs you need to redefine next macros

```
#define mmc_spi_init(prescaler)             spi_master_init(prescaler)
#define mmc_spi_rxtx_byte(data)             spi_rxtx_byte(data)
#define MMC_SS_LOW                          SPI_SS_LOW
#define MMC_SS_HIGH                         SPI_SS_HIGH
#define MMC_CLK_SLOW()                      spi_baudrate(SPI_BAUDRATE_PRESCALER_64)
#define MMC_CLK_FAST()                      spi_baudrate(SPI_BAUDRATE_PRESCALER_16)
```
* mmc_spi_master_init(prescaler) - SPI peripheral should be initialize in MODE0 with low clock speed
* mmc_spi_rxtx_byte(data) -  Send data via SPI. This function returns value received via SPI
* MMC_SS_LOW - Pull SS line LOW
* MMC_SS_HIGH - Pull SS line HIGH
* MMC_CLK_SLOW() - Change SPI clock speed (100 - 400 kHz)
* MMC_CLK_FAST() - Change SPI clock speed to higher values which dependins on the MMC/SD card used

## Authors

* **Semir Tursunovic** - semir-t

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details


## Acknowledgments
*  Elm Chans [FatFS](http://elm-chan.org/fsw/ff/00index_e.html)
