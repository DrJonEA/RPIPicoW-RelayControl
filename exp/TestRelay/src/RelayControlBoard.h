/***
 * Configuration for Relay Control Board
 */

#ifndef RelayControlBoard_H
#define RelayControlBoard_H

#include "pico/stdlib.h"


#define NEOPIXELS_PIN 26
#define NEOPIXELS_LEN 4

#define SW2_GP 27

#define RELAY_K1 10


#define SDA0 0
#define SCL0 1
#define SDA1 2
#define SCL1 3

#define SPI SPI0
#define SPI_SCK 6
#define SPI_TX 7
#define SPI_RX 8
#define SPI_CS 9

#define UART0_TX 16
#define UART0_RX 17
#define UART0_BAUDRATE 115200

#define UART1_TX 4
#define UART1_RX 5
#define UART1_BAUDRATE 115200



#endif
