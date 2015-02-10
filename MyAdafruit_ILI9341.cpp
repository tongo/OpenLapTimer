
// #include "MyAdafruit_ILI9341.h"

// MyAdafruit_ILI9341::MyAdafruit_ILI9341(int8_t cs, int8_t dc, int8_t rst) : Adafruit_ILI9341(cs, dc, rst) { }

// void MyAdafruit_ILI9341::setFont(const uint8_t * font) {
//   this->font = (uint8_t *)font;
//   memcpy_P((void*) &fontHeader, (void*) this->font, sizeof(struct FontHeader));
// }

// int MyAdafruit_ILI9341::charWidth(const char letter) {
// 	//TEST
// 	//return fontHeader.fixedWidth;

// 	if(letter == ' ') {
// 		return fontHeader.fixedWidth;
// 	}

// 	if((uint8_t)letter < fontHeader.firstChar || (uint8_t)letter >= (fontHeader.firstChar + fontHeader.charCount)) { return 0; }

// 	if(fontHeader.size == 0) {
// 		return fontHeader.fixedWidth;
// 	}
// 	return pgm_read_byte(this->font + sizeof(FontHeader) + letter - fontHeader.firstChar);
// }

// #if ARDUINO >= 100
// size_t MyAdafruit_ILI9341::write(uint8_t c) {
// #else
// void MyAdafruit_ILI9341::write(uint8_t c) {
// #endif
// 	uint8_t width = charWidth(c);
// 	if(c == ' ') {
// 		fillRect(cursor_x, cursor_y, width + 1, fontHeader.height + 1, textbgcolor);
// 		cursor_x += (width + 4);
// 	} else if (c == '\n') {
// 		cursor_y += fontHeader.height + 1;
// 		cursor_x  = 0;
// 	} else if (c == '\r') {
// 		// skip em
// 	} else {
// 		drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
// 		cursor_x += (width + 4);
// 		if (wrap && (cursor_x > (_width - (width + 4)))) {
// 			cursor_y += fontHeader.height + 1;
// 			cursor_x = 0;
// 		}
// 	}
// #if ARDUINO >= 100
// 	return 1;
// #endif
// }

// void MyAdafruit_ILI9341::drawChar(int16_t xCursor, int16_t yCursor, unsigned char letter, uint16_t color, uint16_t bg, uint8_t size) {
// //int MyAdafruit_ILI9341::drawChar(const int xCursor, const int yCursor, const char letter, const uint16_t colour, const uint16_t background) {

// 	//Serial.println("drawChar");
// 	if (xCursor < 0 || yCursor < 0 || xCursor >= ILI9341_TFTWIDTH || yCursor >= ILI9341_TFTHEIGHT) { return; }
// 	char c = letter;
// 	int width = charWidth(letter);
// 	uint8_t bytesPerColonna = (fontHeader.height + 7) / 8; // Number of bytes in a single column

// 	char cCopy = c;
// 	cCopy -= fontHeader.firstChar;
// 	int indice = 0;
// 	for (uint8_t i = 0; i < cCopy; i++) {
// 		indice += pgm_read_byte(this->font + sizeof(struct FontHeader) + i);
// 	}
// 	indice = indice * bytesPerColonna + sizeof(struct FontHeader) + fontHeader.charCount;

// 	// TODO scorro i bytes per colonna (riga da 8 x riga da 8)
// 		// TODO scorro tutta la larghezza
// 			// TODO scorro la y del blocco (0-7)
// 				// TODO disegno il pixel

// 	for(uint8_t block = 0; block < bytesPerColonna; block ++) {
// 		for(uint8_t x = 0; x < width + 4; x++) {
// 			int indiceByte = indice + (block * width) + x;
// 			uint8_t byteData;
// 			if(x >= width) {
// 				byteData = 0x0;
// 			} else {
// 				byteData = pgm_read_byte(this->font + indiceByte);
// 			}
// 			for(uint8_t y = 0; y < 8; y++) {
// 				int xPx = xCursor + x;
// 				int yPx = yCursor + (block * 8) + y;

// 				if(byteData & 0x1) {
// 					//colorFont = 1;
// 					drawPixel(xPx, yPx, color);
// 				} else {
// 					drawPixel(xPx, yPx, bg);
// 				}

// 				byteData = byteData >> 1;
// 			}
// 		}
// 	}
// }
