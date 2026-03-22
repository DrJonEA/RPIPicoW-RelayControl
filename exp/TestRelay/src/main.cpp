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
#include "Relay.h"
#include "RelayLeds.h"

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




int main(){
	uint8_t r,g,b;

	stdio_init_all();
	bool rc = status_led_init();
	bool status = false;

	//UART Init
	uartInit(uart0, UART0_TX, UART0_RX);
	uartInit(uart1, UART1_TX, UART1_RX);

	//Keep brightness down as we are running at 3.3v
	ledsStrip.setBrightness(32);

	// Initialise the LED to all off
	ledsStrip.fill( PicoLed::RGB(0x00, 0x00, 0x00 ));
	ledsStrip.show();

	//Init Relays
	RelayLeds relayLeds;
	relayLeds.setLeds(&ledsStrip);

	Relay k1(RELAY_K1, SW2_GP);
	relayLeds.map(RELAY_K1, 2);
	k1.setObserver(&relayLeds);


	sleep_ms(2000);

	for (;;){
		for (double d = 0.0; d < 1.0; d+=0.1){
			ColorUtils::valToRGB(
					d,
					r, g, b
					);
			ledsStrip.setPixelColor(0,  PicoLed::RGB(r,g,b ));
			ledsStrip.setPixelColor(1,  PicoLed::RGB(r,g,b ));
			ledsStrip.show();
			sleep_ms(100);
		}
		status = ! status;
		status_led_set_state(status);

		uartTest(uart0, "UART0");
		uartTest(uart1, "UART1");
	}


}
