#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "SPI.h"
#include <SD.h>

#define MATRIX_HEIGHT     8
#define MATRIX_WIDTH      8
#define NUMPIXELS         (MATRIX_WIDTH * MATRIX_HEIGHT)
#define FRAME_SIZE        (NUMPIXELS * 3)
#define PIN               9
#define MAX_BRIGHTNESS    255.0
#define CAP_BRIGHTNESS    50
#define RANGE_UPPER_BOUND 180
#define RANGE_LOWER_BOUND 20

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint16_t frame_index;
uint16_t frames;
uint8_t  frame_gap;

File file;

void clear_leds();

void flame();

void update_leds(uint8_t frame);

void setup() {
	pixels.begin();
	// Serial.begin(9600);
	// while (!Serial) {;}
	SD.begin();
	// Serial.println("Loading File");

	file = SD.open("img.dat");
	if (!file) {
		// Serial.println("File failed to load");
		return;
	}
	frames = file.size()/FRAME_SIZE;
	frame_index = frames - 1;
	frame_gap = ((RANGE_UPPER_BOUND - RANGE_LOWER_BOUND)/frames);
	// Serial.print("File Size: ");
	// Serial.println(file.size());
	// Serial.print("Frame Count: ");
	// Serial.println(frames);
	// Serial.print("Frame Gap: ");
	// Serial.println(frame_gap);
	randomSeed(analogRead(0));
}

void loop() {
	delay(500);
	uint16_t val = analogRead(0);
	// Serial.print("analog 0 is: ");
	// Serial.println(val);

  	if (frame_index == 0 && val < RANGE_LOWER_BOUND) {
  		// explode
  		for (int f = 0; f < 20; f++) {
  			flame();
  			delay(100);
  		}
  		clear_leds();
  		frame_index = frames - 1;
  	}
  	else if (val >= (frame_gap * frame_index + RANGE_LOWER_BOUND) && frame_index < frames - 1) {
  		frame_index++;
  	}
  	else if (val < (frame_gap * frame_index + RANGE_LOWER_BOUND)) {
  		frame_index--;
  	}
	if (frame_index == frames - 1 && val > RANGE_UPPER_BOUND) {
		clear_leds();
	}
	update_leds(frame_index);
}

void update_leds(uint8_t frame) {
	// Serial.print("Loading Frame: ");
	// Serial.println(frame);
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