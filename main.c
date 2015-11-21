/*
The following license only applies to this file. Please note
that the source code for the Mongoose web server, distributed with this
is GPL-licensed.

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
*/

#define _CRT_SECURE_NO_WARNINGS

#include "daqsim.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

//This function displays a formatted string on the seven segment displays.
//Takes the same arguments as printf. Character set is far from complete, but
//it will at least print numbers.
void daq_printf(char* format, ...) {
	const int digits[] = { 252, 96, 218, 242, 102, 182, 190, 224, 254, 246 };
	const int uppercase[] = { 0xEE, 0xFE, 0x9C, 0xFC, 0x9E, 0x8E, 0xBC, 0x6E};
	int dispPos = 0;
	char ascii[17];
	int display[17] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(ascii, 16, format, args);
	va_end(args);
	//printf("%s\n", ascii);
	for (int i = 0; i < 17; i++) {
		if (ascii[i] == 0) break;
		else if (ascii[i] == ' ') display[dispPos++] = 0;
		else if (ascii[i] == '.') {
			if ((i > 0) && ((display[dispPos - 1] & 1) == 0)) {
				display[dispPos - 1] = display[dispPos - 1] | 1;
			}
			else display[dispPos++] = 1;
		}
		else if (ascii[i] == '-') display[dispPos++] = 0x02;
		else if ((ascii[i] >= '0') && (ascii[i] <= '9')) display[dispPos++] = digits[ascii[i] - '0'];
		else if (ascii[i] >= 0x61 && ascii[i] <= 0x7A) display[dispPos++] = uppercase[ascii[i] - 0x61];
		else display[i] = 254;
	}
	if (dispPos > 8) {
		display[7] = 2;
	}
	for (int i = 0; i < 8; i++) {
		displayWrite(display[i], 7-i);
	}
}

//Turns on/off LEDs according to the bits of the function argument.
//Least significant bit corresponds to LED0, and so on.
void set_leds(int pattern) {
	for (int i = 0; i < DAQ_NUM_DIGITAL_OUTPUTS; i++) {
		digitalWrite(i, pattern & 1);
		pattern = pattern >> 1;
	}
}

int main(void) {
	//Declare variables, start simulator server.
	int led_pattern = 0;
	int prev_sw0_state = 0;
	setupDAQ(7);
	//Main loop
	while (continueSuperLoop()) {
		//Sleep to avoid high CPU usage.
		daq_sleep(10);
		//Display a binary counter on the LEDs that increments every time the
		//state of switch/button 0 changes. Use Hardware 2 for best effect.
		int curr_sw0_state = digitalRead(0);
		if (curr_sw0_state != prev_sw0_state) {
			prev_sw0_state = curr_sw0_state;
			led_pattern++;
			set_leds(led_pattern);
		}
		//If switch 1 is on, show the angles of the arms on the display.
		if (digitalRead(1)) {
			daq_printf("%4d%4d", (int)analogRead(0), (int)analogRead(1));
			printf("%4d%4d\n", (int)analogRead(0), (int)analogRead(1));
		}
		//Otherwise display the current time.
		else {
			time_t raw_time;
			struct tm* local_time;
			time(&raw_time);
			char time_str[20];
			strftime(time_str, 19, "%H.%M.%S", localtime(&raw_time));
			daq_printf("%s\n", time_str);
		}
	}
	return 0;
};