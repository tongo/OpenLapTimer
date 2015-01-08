
#ifndef _MY_ADAFRUIT_ILI9341H_
#define _MY_ADAFRUIT_ILI9341H_

#include "Adafruit_ILI9341.h"

// Six byte header at beginning of FontCreator font structure, stored in PROGMEM
struct FontHeader {
  uint16_t size;
  uint8_t fixedWidth;
  uint8_t height;
  uint8_t firstChar;
  uint8_t charCount;
};

class MyAdafruit_ILI9341 : public Adafruit_ILI9341 {

	public:
		MyAdafruit_ILI9341(int8_t _CS, int8_t _DC, int8_t _RST = -1);
		void setFont(const uint8_t* font);

		#if ARDUINO >= 100
		  virtual size_t write(uint8_t);
		#else
		  virtual void   write(uint8_t);
		#endif

		void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
	private:
		uint8_t *font;
		FontHeader fontHeader;
		int charWidth(const char letter);
};

#endif
