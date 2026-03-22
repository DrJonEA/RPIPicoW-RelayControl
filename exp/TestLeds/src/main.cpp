/*
 * main.cpp

 *      Author: jondurrant
 */


#include "pico/stdlib.h"
#include <stdexcept>
#include "hardware/uart.h"

#include <PicoLed.hpp>
#include "ColorUtils.h"
extern "C"{
#include "pico/status_led.h"
}

#include "RelayControlBoard.h"

// Initialize LED strip
auto ledsStrip = PicoLed::addLeds<PicoLed::WS2812B>(
		pio0,
		0,
		NEOPIXELS_PIN,
		NEOPIXELS_LEN,
		PicoLed::FORMAT_GRB);


void uartInit(uart_inst_t * uart, uint8_t tx, uint8_t rx){
	 uart_init(uart, UART0_BAUDRATE);

	gpio_set_function(tx, UART_FUNCSEL_NUM(uart, tx));
	gpio_set_function(rx, UART_FUNCSEL_NUM(uart, rx));
}

void uartTest(uart_inst_t * uart, const char* name){
	char s[80];
	sprintf(s, "Hello, UART: %s!\n\r", name);
	uart_puts(uart, s);
}



bool swState;

void gpioCallback (uint gpio, uint32_t events){
	if (gpio == SW2_GP){
		if (swState){
			ledsStrip.setPixelColor(0,  PicoLed::RGB(0xff,0,0));
		} else {
			ledsStrip.setPixelColor(0, PicoLed::RGB(0,0xff,0));
		}
		swState = ! swState;
	}
}

void switchInit(uint8_t gpio){
	gpio_init(gpio);
	gpio_set_dir(gpio, GPIO_IN);
	gpio_pull_up(gpio);

	gpio_set_irq_enabled_with_callback(gpio,
		GPIO_IRQ_EDGE_FALL,
		true,
		gpioCallback
	);
}


int main(){
	uint8_t r,g,b;

	stdio_init_all();
	bool rc = status_led_init();
	bool status = false;

	//UART Init
	uartInit(uart0, UART0_TX, UART0_RX);
	uartInit(uart1, UART1_TX, UART1_RX);

	//Init Switches
	switchInit(SW2_GP);


	//Keep brightness down as we are running at 3.3v
	ledsStrip.setBrightness(32);

	// Initialise the LED to all off
	ledsStrip.fill( PicoLed::RGB(0x00, 0x00, 0xFF ));
	ledsStrip.show();
	sleep_ms(2000);

	for (;;){
		for (double d = 0.0; d < 1.0; d+=0.1){
			ColorUtils::valToRGB(
					d,
					r, g, b
					);
			ledsStrip.setPixelColor(1,  PicoLed::RGB(r,g,b ));
			ledsStrip.setPixelColor(2,  PicoLed::RGB(r,g,b ));
			ledsStrip.setPixelColor(3,  PicoLed::RGB(r,g,b ));
			ledsStrip.show();
			sleep_ms(100);
		}
		status = ! status;
		status_led_set_state(status);

		uartTest(uart0, "UART0");
		uartTest(uart1, "UART1");
	}


}
