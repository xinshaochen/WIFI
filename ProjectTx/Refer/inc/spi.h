#ifndef __spi_h
#define __spi_h

#include "stm8s.h"
#include "spi.h"


void SPI_Init(void);
u8 SPI_RW(u8 Data);
void SPI_IOConfig(void);

#define SPI_CS               (1<<4)
#define SPI_SCK              (1<<5)
#define SPI_MOSI             (1<<6)
#define SPI_MISO             (1<<7)

#define SPI_CS_GPIO          GPIOC
#define SPI_MISO_GPIO        GPIOC
#define SPI_MOSI_GPIO        GPIOC
#define SPI_SCK_GPIO         GPIOC


#endif