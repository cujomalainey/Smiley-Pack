#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "SPI.h"
#include <SD.h>

#define MATRIX_HEIGHT   8
#define MATRIX_WIDTH    8
#define NUMPIXELS       MATRIX_WIDTH * MATRIX_HEIGHT
#define FRAME_SIZE      NUMPIXELS * 3
#define PIN             9
#define FRAMES          10
#define MAX_BRIGHTNESS  255.0
#define CAP_BRIGHTNESS  50

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const uint16_t levels[] = {20, 50, 75, 100, 150, 200, 250, 300, 350, 400, 425};

uint8_t frame_index = FRAMES - 1;

File file;

void clear_leds();

void flame();

void update_leds(uint8_t frame);

void setup() {
	pixels.begin();
	Serial.begin(9600);
	while (!Serial) {;}
	SD.begin();
	Serial.println("Loading File");
	file = SD.open("img.dat");
	if (!file) {
		Serial.println("File failed to load");
		return;
	}
	randomSeed(analogRead(0));
}

void loop() {
	delay(500);
	uint16_t val = analogRead(0);
  	Serial.print("analog 0 is: ");
  	Serial.println(val);
  	if (frame_index == 0 && val < levels[frame_index]) {
  		// explode
  		for (int f = 0; f < 20; f++) {
  			flame();
  			delay(100);
  		}
  		clear_leds();
  		frame_index = FRAMES - 1;
  	}
  	else if (val >= levels[frame_index] && frame_index + 1 < FRAMES) {
  		frame_index++;
  	}
  	else if (val < levels[frame_index - 1]) {
  		frame_index--;
  	}
	if (frame_index < 0) {
		clear_leds();
	}
	else {
		update_leds(frame_index);
	}
}

void update_leds(uint8_t frame) {
	Serial.print("Loading Frame: ");
	Serial.println(frame);
	file.seek(frame*FRAME_SIZE);
	uint16_t t = millis();
	for (int y = 0; y < MATRIX_HEIGHT; y++)
	{
		for (int x = 0; x < MATRIX_WIDTH; x++)
		{
			uint8_t red   = file.read();
			uint8_t green = file.read();
			uint8_t blue  = file.read();
			pixels.setPixelColor(y*MATRIX_WIDTH + x,
      			pixels.Color((red/MAX_BRIGHTNESS) * CAP_BRIGHTNESS, (green/MAX_BRIGHTNESS) * CAP_BRIGHTNESS, (blue/MAX_BRIGHTNESS) * CAP_BRIGHTNESS)
      			);
		}
	}
	t = millis() - t;
	Serial.print("Took: ");
	Serial.println(t);
	Serial.println("Displaying.");
	pixels.show();
}

void clear_leds() {
	for (int y = 0; y < MATRIX_HEIGHT; y++)
	{
		for (int x = 0; x < MATRIX_WIDTH; x++)
		{
			pixels.setPixelColor(y*MATRIX_WIDTH + x, pixels.Color(0, 0, 0));
		}
	}
	pixels.show();
}

void flame() {
	for (int y = 0; y < MATRIX_HEIGHT; y++)
	{
		for (int x = 0; x < MATRIX_WIDTH; x++)
		{
			uint8_t rnd = random(3);
			switch (rnd) {
				case 0:
					pixels.setPixelColor(y*MATRIX_WIDTH + x,
		      			pixels.Color((255/MAX_BRIGHTNESS) * CAP_BRIGHTNESS, 0, 0)
		      			);
					break;
				case 1:
					pixels.setPixelColor(y*MATRIX_WIDTH + x,
		      			pixels.Color((255/MAX_BRIGHTNESS) * CAP_BRIGHTNESS, (102/MAX_BRIGHTNESS) * CAP_BRIGHTNESS, 0)
		      			);
					break;
				case 2:
					pixels.setPixelColor(y*MATRIX_WIDTH + x,
		      			pixels.Color((255/MAX_BRIGHTNESS) * CAP_BRIGHTNESS, (255/MAX_BRIGHTNESS) * CAP_BRIGHTNESS, 0)
		      			);
					break;
			}
		}
	}
	pixels.show();
}