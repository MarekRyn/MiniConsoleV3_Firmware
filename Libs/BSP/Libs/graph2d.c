/*******************************************************************
 * MiniConsole V3 - Board Support Package - Graph 2D Library
 *
 * Author: Marek Ryn
 * Version: 1.0
 *
 * Changelog:
 *
 * - 0.1b	- Development version
 * - 0.2b 	- Added hardware JPEG decoding
 * - 0.3b	- Added ARGB1555 and ARGB4444 modes
 * - 0.4b   - Added RoundRect and FillRoundRect
 * 			- Added DrawIconBlend and TextBlend
 * - 1.0	- Major refactoring
 *******************************************************************/

#ifdef CORE_CM7

#include "graph2d.h"
#include "math.h"

#define ABS(X)		((X) > 0 ? (X) : -(X))
#define MIN(A, B)	((A) > (B) ? (B) : (A))
#define MAX(A, B)	((A) > (B) ? (A) : (B))


inline static uint8_t _charw(const uint8_t *font, uint8_t ch) {
	// This function only returns character width

	uint16_t *adr0;								// Start address of encoded character
	uint16_t *adr1;								// End address of encoded character
	uint8_t swidth = *(font + 1);			   	// Width of space (empty character)

	// Testing if character is outside allowable range
	if ((ch < 33) || (ch > 126)) return swidth;

	// Looking for character in index
	uint16_t a = (ch - 33) * 2;
	adr0 = (uint16_t*)(font + a + 2);
	adr1 = (uint16_t*)(font + a + 4);

	// Testing if character is outside defined subset
	if (*adr0 == *adr1) return swidth;

	// Reading width of given character
	return *(font + *adr0);
}


inline static uint8_t _char(int16_t x, int16_t y, const uint8_t *font, uint8_t ch, uint32_t *text_clut) {
	// Function rendering character on screen
	uint16_t *adr0;								// Start address of encoded character
	uint16_t *adr1;								// End address of encoded character
	uint8_t swidth = *(font + 1);			   	// Width of space (empty character)

	// Testing if character is outside allowable range
	if ((ch < 33) || (ch > 126)) return swidth;

	// Looking for character in index
	uint16_t a = (ch - 33) * 2;
	adr0 = (uint16_t*)(font + a + 2);
	adr1 = (uint16_t*)(font + a + 4);

	// Testing if character is outside defined subset
	if (*adr0 == *adr1) return swidth;

	// Reading width of given character
	uint8_t width = *(font + *adr0);

	// Decoding and drawing character
	uint8_t m;
	int16_t fx = 0;
	int16_t fy = 0;
	uint32_t offset = BSP_LCD_GetEditFrameAddr();


	for (uint16_t j = *adr0 + 1; j < *adr1; j++) {
		m = *(font + j);
		switch (m >> 6) {
		case 0: // 0% color
		case 3:	// 100% color
			for (uint8_t i = 0; i < (m & 0x3F); i++) {
				BSP_LCD_UpdatePixel(offset, x+fx, y+fy, text_clut[m >> 6]);
				fx++;
				if (fx==width) { fx = 0; fy++; }
			}
			break;
		case 1: // 33% color
		case 2: // 66% color
			for (int8_t i = 6; i >= 0; i-=2) {
				BSP_LCD_UpdatePixel(offset, x+fx, y+fy, text_clut[(m >> i) & 0x03]);
				fx++;
				if (fx==width) { fx = 0; fy++; }
			}
			break;
		}
	}
	return width;
}


inline static uint8_t _charblend( int16_t x, int16_t y, const uint8_t *font, uint8_t ch, uint32_t *text_clut) {
	// Function rendering character on screen
	uint16_t *adr0;								// Start address of encoded character
	uint16_t *adr1;								// End address of encoded character
	uint8_t swidth = *(font + 1);			   	// Width of space (empty character)

	// Testing if character is outside allowable range
	if ((ch < 33) || (ch > 126)) return swidth;

	// Looking for character in index
	uint16_t a = (ch - 33) * 2;
	adr0 = (uint16_t*)(font + a + 2);
	adr1 = (uint16_t*)(font + a + 4);

	// Testing if character is outside defined subset
	if (*adr0 == *adr1) return swidth;

	// Reading width of given character
	uint8_t width = *(font + *adr0);

	// Decoding and drawing character
	uint8_t m;
	int16_t fx = 0;
	int16_t fy = 0;
	uint32_t offset = BSP_LCD_GetEditFrameAddr();


	for (uint16_t j = *adr0 + 1; j < *adr1; j++) {
		m = *(font + j);
		switch (m >> 6) {
		case 0: // 0% color
		case 3:	// 100% color
			for (uint8_t i = 0; i < (m & 0x3F); i++) {
				BSP_LCD_UpdatePixelBlend(offset, x+fx, y+fy, text_clut[m >> 6]);
				fx++;
				if (fx==width) { fx = 0; fy++; }
			}
			break;
		case 1: // 33% color
		case 2: // 66% color
			for (int8_t i = 6; i >= 0; i-=2) {
				BSP_LCD_UpdatePixelBlend(offset, x+fx, y+fy, text_clut[(m >> i) & 0x03]);
				fx++;
				if (fx==width) { fx = 0; fy++; }
			}
			break;
		}
	}
	return width;
}


uint32_t static _getbufpixel_ARGB8888(uint32_t offset, int16_t width, int16_t x, int16_t y) {
	uint32_t addr = offset;
	addr += (x + y * width) << 2;
	return *(uint32_t *)addr;
}

uint32_t static _getbufpixel_ARGB1555(uint32_t offset, int16_t width, int16_t x, int16_t y) {
	uint32_t addr = offset;
	addr += (x + y * width) << 1;
	return *(uint16_t *)addr;
}

uint32_t static _getbufpixel_ARGB4444(uint32_t offset, int16_t width, int16_t x, int16_t y) {
	uint32_t addr = offset;
	addr += (x + y * width) << 1;
	return *(uint16_t *)addr;
}

uint32_t static _getbufpixel_RGB888(uint32_t offset, int16_t width, int16_t x, int16_t y) {
	uint32_t addr0 = offset + (x + y * width) * 3;
	uint32_t addr1 = addr0 + 1;
	if (addr0 & 1) return *(uint8_t *)(addr0) | (*(uint16_t *)(addr1) << 8);
	return *(uint16_t *)(addr0) | (*(uint8_t *)(addr1) << 16);
}

uint32_t static _getbufpixel_AL88(uint32_t offset, int16_t width, int16_t x, int16_t y) {
	uint32_t addr = offset;
	addr += (x + y * width) << 1;
	return *(uint16_t *)addr;
}

uint32_t static _getbufpixel_L8(uint32_t offset, int16_t width, int16_t x, int16_t y) {
	uint32_t addr = offset;
	addr += (x + y * width);
	return *(uint8_t *)addr;
}

void G2D_ClearFrame(void) {
	BSP_LCD_FillBuf(0, 0, LCD_WIDTH, LCD_HEIGHT, 0, 0x00000000);
}


void G2D_FillFrame(uint32_t color) {
	BSP_LCD_FillBuf(0, 0, LCD_WIDTH, LCD_HEIGHT, 0, color);
}


void G2D_CopyPrevFrame(void) {

	uint32_t src_addr = BSP_LCD_GetPrevFrameAddr();

	BSP_LCD_CopyBuf(src_addr, 0, 0, 0, 0, LCD_WIDTH, LCD_HEIGHT);
}

void G2D_CopyScrollPrevFrame(int16_t dx, int16_t dy) {
	int16_t x0 = MAX(0, -dx);
	int16_t y0 = MAX(0, -dy);
	int16_t	x1 = MIN(LCD_WIDTH, LCD_WIDTH - dx);
	int16_t y1 = MIN(LCD_HEIGHT, LCD_HEIGHT - dy);
	int16_t x2 = x0 + dx;
	int16_t y2 = y0 + dy;
	int16_t w = x1 - x0;
	int16_t h = y1 - y0;
	int16_t offset = LCD_WIDTH - w;

	uint32_t src_addr = BSP_LCD_GetPrevFrameAddr() + ((x0 + y0 * LCD_WIDTH) * BSP_LCD_GetBytesPerPixel());

	BSP_LCD_CopyBuf(src_addr, offset, x2, y2, offset, w, h);
}


void G2D_DrawPixel(int16_t x, int16_t y, uint32_t color) {

	uint32_t offset = BSP_LCD_GetEditFrameAddr();

	BSP_LCD_DMA2D_Wait();
	BSP_LCD_UpdatePixel(offset, x, y, color);
}

void G2D_DrawHLine(int16_t x, int16_t y, int16_t length, uint32_t color) {
	if (length == 0) return;
	if (length < 0) {x -= length + 1; length = -length;}
	if (y < 1) return;
	if (y >= LCD_HEIGHT) return;
	if ((x + length) < 1) return;
	if (x >= LCD_WIDTH) return;
	uint16_t x0 = MAX(0, x);
	uint16_t x1 = MIN(LCD_WIDTH, x + length);
	uint16_t l = x1 - x0;
	uint16_t lo = LCD_WIDTH - l;
	BSP_LCD_FillBuf(x0, y, l, 1, lo, color);
}


void G2D_DrawHLineBlend(int16_t x, int16_t y, int16_t length, uint32_t color) {
	if (length == 0) return;
	if (length < 0) {x -= length + 1; length = -length;}
	if (y < 1) return;
	if (y >= LCD_HEIGHT) return;
	if ((x + length) < 1) return;
	if (x >= LCD_WIDTH) return;
	uint16_t x0 = MAX(0, x);
	uint16_t x1 = MIN(LCD_WIDTH, x + length);
	uint16_t l = x1 - x0;
	uint16_t lo = LCD_WIDTH - l;
	BSP_LCD_FillBufBlend(x0, y, l, 1, lo, color);
}


void G2D_DrawVLine(int16_t x, int16_t y, int16_t length, uint32_t color) {
	if (length == 0) return;
	if (length < 0) {y -= length + 1; length = -length;}
	if (x < 1) return;
	if (x >= LCD_WIDTH) return;
	if ((y + length) < 1) return;
	if (y >= LCD_HEIGHT) return;
	uint16_t y0 = MAX(0, y);
	uint16_t y1 = MIN(LCD_HEIGHT, y + length);
	uint16_t l = y1 - y0;
	if (l == 0) return;
	uint16_t lo = LCD_WIDTH - 1;
	BSP_LCD_FillBuf(x, y0, 1, l, lo, color);
}


void G2D_DrawVLineBlend(int16_t x, int16_t y, int16_t length, uint32_t color) {
	if (length == 0) return;
	if (length < 0) {y -= length + 1; length = -length;}
	if (x < 1) return;
	if (x >= LCD_WIDTH) return;
	if ((y + length) < 1) return;
	if (y >= LCD_HEIGHT) return;
	uint16_t y0 = MAX(0, y);
	uint16_t y1 = MIN(LCD_HEIGHT, y + length);
	uint16_t l = y1 - y0;
	if (l == 0) return;
	uint16_t lo = LCD_WIDTH - 1;
	BSP_LCD_FillBufBlend(x, y0, 1, l, lo, color);
}


void G2D_DrawLine(int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint32_t color) {
  int16_t dx = 0;
  int16_t dy = 0;
  int16_t x = 0;
  int16_t y = 0;
  int16_t xinc1 = 0;
  int16_t xinc2 = 0;
  int16_t yinc1 = 0;
  int16_t yinc2 = 0;
  int16_t den = 0;
  int16_t num = 0;
  int16_t numadd = 0;
  int16_t numpixels = 0;
  int16_t curpixel = 0;

  dx = ABS(X2 - X1);        									/* The difference between the x's */
  dy = ABS(Y2 - Y1);        									/* The difference between the y's */
  x = X1;                   									/* Start x off at the first pixel */
  y = Y1;                  										/* Start y off at the first pixel */

  if (X2 >= X1) {xinc1 = 1; xinc2 = 1;}         				/* The x-values are increasing */
  else {xinc1 = -1; xinc2 = -1;}                				/* The x-values are decreasing */

  if (Y2 >= Y1) {yinc1 = 1; yinc2 = 1;}         				/* The y-values are increasing */
  else {yinc1 = -1; yinc2 = -1;}                  				/* The y-values are decreasing */

  if (dx >= dy) {					            				/* There is at least one x-value for every y-value */
    xinc1 = 0;              									/* Don't change the x when numerator >= denominator */
    yinc2 = 0;              									/* Don't change the y for every iteration */
    den = dx;
    num = dx >> 1;
    numadd = dy;
    numpixels = dx;         									/* There are more x-values than y-values */
  } else {					                    				/* There is at least one y-value for every x-value */
    xinc2 = 0;              									/* Don't change the x for every iteration */
    yinc1 = 0;              									/* Don't change the y when numerator >= denominator */
    den = dy;
    num = dy >> 1;
    numadd = dx;
    numpixels = dy;         									/* There are more y-values than x-values */
  }

  uint32_t offset = BSP_LCD_GetEditFrameAddr();

  BSP_LCD_DMA2D_Wait();

  for (curpixel = 0; curpixel <= numpixels; curpixel++) {
	BSP_LCD_UpdatePixel(offset, x, y, color);				   	/* Draw the current pixel */
    num += numadd;                            					/* Increase the numerator by the top of the fraction */
    if (num >= den) {                         					/* Check if numerator >= denominator */
      num -= den;                             					/* Calculate the new numerator value */
      x += xinc1;                            					/* Change the x as appropriate */
      y += yinc1;                             					/* Change the y as appropriate */
    }
    x += xinc2;                               					/* Change the x as appropriate */
    y += yinc2;                               					/* Change the y as appropriate */
  }
}


void G2D_DrawRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color) {
	int16_t xx = x + width;
	int16_t yy = y + height;
	uint16_t x1 = MAX(0, x);
	uint16_t y1 = MAX(0, y);
	uint16_t x2 = MIN(LCD_WIDTH, xx);
	uint16_t y2 = MIN(LCD_HEIGHT, yy);
	uint16_t w = x2 - x1;
	uint16_t h = y2 - y1;

	G2D_DrawHLine(x1, y, w, color);
	G2D_DrawHLine(x1, yy, w, color);
	G2D_DrawVLine(x, y1, h, color);
	G2D_DrawVLine(xx, y1, h, color);
}


void G2D_DrawFillRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color) {
	uint16_t x1 = MAX(0, x);
	uint16_t y1 = MAX(0, y);
	uint16_t x2 = MIN(LCD_WIDTH, (x + width));
	uint16_t y2 = MIN(LCD_HEIGHT, (y + height));
	uint16_t w = x2 - x1;
	uint16_t h = y2 - y1;
	uint16_t ol = LCD_WIDTH - w;

	BSP_LCD_FillBuf(x1, y1, w, h, ol, color);
}

void G2D_DrawFillRectBlend(int16_t x, int16_t y, uint16_t width, uint16_t height, uint32_t color) {
	uint16_t x1 = MAX(0, x);
	uint16_t y1 = MAX(0, y);
	uint16_t x2 = MIN(LCD_WIDTH, (x + width));
	uint16_t y2 = MIN(LCD_HEIGHT, (y + height));
	uint16_t w = x2 - x1;
	uint16_t h = y2 - y1;
	uint16_t ol = LCD_WIDTH - w;

	BSP_LCD_FillBufBlend(x1, y1, w, h, ol, color);
}

void G2D_DrawCircle(int16_t x, int16_t y, uint16_t r, uint32_t color) {
	int32_t d;			/* Decision Variable */
	int32_t curx;		/* Current X Value */
	int32_t cury;		/* Current Y Value */

	d = 3 - (r << 1);
	curx = 0;
	cury = r;

	uint32_t offset = BSP_LCD_GetEditFrameAddr();

	BSP_LCD_DMA2D_Wait();

	while (curx <= cury) {
		BSP_LCD_UpdatePixel(offset, (x + curx), (y - cury), color);
		BSP_LCD_UpdatePixel(offset, (x - curx), (y - cury), color);
		BSP_LCD_UpdatePixel(offset, (x + cury), (y - curx), color);
		BSP_LCD_UpdatePixel(offset, (x - cury), (y - curx), color);
		BSP_LCD_UpdatePixel(offset, (x + curx), (y + cury), color);
		BSP_LCD_UpdatePixel(offset, (x - curx), (y + cury), color);
		BSP_LCD_UpdatePixel(offset, (x + cury), (y + curx), color);
		BSP_LCD_UpdatePixel(offset, (x - cury), (y + curx), color);

		if (d < 0) {
			d += (curx << 2) + 6;
		} else {
			d += ((curx - cury) << 2) + 10;
			cury--;
		}
		curx++;
	}
}


void G2D_DrawFillCircle(int16_t x, int16_t y, uint16_t r, uint32_t color) {
	int32_t  d;    	/* Decision Variable */
	int32_t  curx;	/* Current X Value */
	int32_t  cury;	/* Current Y Value */

	d = 3 - (r << 1);
	curx = 0;
	cury = r;

	while (curx <= cury) {
		if(cury > 0) {
			G2D_DrawHLine(x - cury, y + curx, 2 * cury + 1, color);
			G2D_DrawHLine(x - cury, y - curx, 2 * cury + 1, color);
		}

		if(curx > 0) {
			G2D_DrawHLine(x - curx, y - cury, 2 * curx + 1, color);
			G2D_DrawHLine(x - curx, y + cury, 2 * curx + 1, color);
		}
		if (d < 0) {
			d += (curx << 2) + 6;
		} else {
			d += ((curx - cury) << 2) + 10;
			cury--;
		}
		curx++;
	}
}


void G2D_DrawFillCircleBlend(int16_t x, int16_t y, uint16_t r, uint32_t color) {
	int32_t  d;    	/* Decision Variable */
	int32_t  curx;	/* Current X Value */
	int32_t  cury;	/* Current Y Value */

	d = 3 - (r << 1);
	curx = 0;
	cury = r;

	while (curx <= cury) {
		if(cury > 0) {
			G2D_DrawHLineBlend(x - cury, y + curx, 2 * cury + 1, color);
			G2D_DrawHLineBlend(x - cury, y - curx, 2 * cury + 1, color);
		}

		if(curx > 0) {
			G2D_DrawHLineBlend(x - curx, y - cury, 2 * curx + 1, color);
			G2D_DrawHLineBlend(x - curx, y + cury, 2 * curx + 1, color);
		}
		if (d < 0) {
			d += (curx << 2) + 6;
		} else {
			d += ((curx - cury) << 2) + 10;
			cury--;
		}
		curx++;
	}
}


void G2D_DrawRoundRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t radius, uint32_t color) {
	int32_t  d;    	/* Decision Variable */
	int32_t  curx;	/* Current X Value */
	int32_t  cury;	/* Current Y Value */

	uint16_t radius2 = radius << 1;

	if (height < radius2) return;
	if (width < radius2) return;

	d = 3 - radius2;
	curx = 0;
	cury = radius;

	uint32_t offset = BSP_LCD_GetEditFrameAddr();
	int16_t x0 = x + radius;
	int16_t y0 = y + radius;
	int16_t x1 = x + width - radius;
	int16_t y1 = y + height - radius;

	// Drawing H lines
	if (width > radius2) {
		G2D_DrawHLine(x0, y, width - radius2, color);
		G2D_DrawHLine(x0, y + height, width - radius2, color);
	}
	// Drawing V lines
	if (height > radius2) {
		G2D_DrawVLine(x, y0, height - radius2, color);
		G2D_DrawVLine(x + width, y0, height - radius2, color);
	}

	BSP_LCD_DMA2D_Wait();

	// Drawing round corners
	while (curx <= cury) {
		BSP_LCD_UpdatePixel(offset, (x0 - curx), (y0 - cury), color);
		BSP_LCD_UpdatePixel(offset, (x0 - cury), (y0 - curx), color);
		BSP_LCD_UpdatePixel(offset, (x1 + curx), (y0 - cury), color);
		BSP_LCD_UpdatePixel(offset, (x1 + cury), (y0 - curx), color);
		BSP_LCD_UpdatePixel(offset, (x0 - curx), (y1 + cury), color);
		BSP_LCD_UpdatePixel(offset, (x0 - cury), (y1 + curx), color);
		BSP_LCD_UpdatePixel(offset, (x1 + cury), (y1 + curx), color);
		BSP_LCD_UpdatePixel(offset, (x1 + curx), (y1 + cury), color);
		if (d < 0) {
			d += (curx << 2) + 6;
		} else {
			d += ((curx - cury) << 2) + 10;
			cury--;
		}
		curx++;
	}

}


void G2D_DrawFillRoundRect(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t radius, uint32_t color) {
	int32_t  d;    	/* Decision Variable */
	int32_t  curx;	/* Current X Value */
	int32_t  cury;	/* Current Y Value */

	uint16_t radius2 = radius << 1;

	if (height < radius2) return;
	if (width < radius2) return;

	d = 3 - radius2;
	curx = 0;
	cury = radius;

	int16_t x0 = x + radius;
	int16_t y0 = y + radius;
	int16_t y1 = y + height - radius;
	uint16_t wr = width - radius2;

	// Drawing middle filled area
	G2D_DrawFillRect(x, y0 + 1, width, height - radius2 - 1, color);

	// Drawing round parts (top and bottom)
	while (curx <= cury) {
		if(cury > 0) {
			G2D_DrawHLine(x0 - cury, y1 + curx, 2 * cury + wr, color);
			G2D_DrawHLine(x0 - cury, y0 - curx, 2 * cury + wr, color);
		}

		if(curx > 0) {
			G2D_DrawHLine(x0 - curx, y0 - cury, 2 * curx + wr, color);
			G2D_DrawHLine(x0 - curx, y1 + cury, 2 * curx + wr, color);
		}
		if (d < 0) {
			d += (curx << 2) + 6;
		} else {
			d += ((curx - cury) << 2) + 10;
			cury--;
		}
		curx++;
	}

}


void G2D_DrawFillRoundRectBlend(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t radius, uint32_t color) {
	int32_t  d;    	/* Decision Variable */
	int32_t  curx;	/* Current X Value */
	int32_t  cury;	/* Current Y Value */

	uint16_t radius2 = radius << 1;

	if (height < radius2) return;
	if (width < radius2) return;

	d = 3 - radius2;
	curx = 0;
	cury = radius;

	int16_t x0 = x + radius;
	int16_t y0 = y + radius;
	int16_t y1 = y + height - radius;
	uint16_t wr = width - radius2;

	// Drawing middle filled area
	G2D_DrawFillRectBlend(x, y0 + 1, width, height - radius2 - 1, color);

	// Drawing round parts (top and bottom)
	while (curx <= cury) {
		if(cury > 0) {
			G2D_DrawHLineBlend(x0 - cury, y1 + curx, 2 * cury + wr, color);
			G2D_DrawHLineBlend(x0 - cury, y0 - curx, 2 * cury + wr, color);
		}

		if(curx > 0) {
			G2D_DrawHLineBlend(x0 - curx, y0 - cury, 2 * curx + wr, color);
			G2D_DrawHLineBlend(x0 - curx, y1 + cury, 2 * curx + wr, color);
		}
		if (d < 0) {
			d += (curx << 2) + 6;
		} else {
			d += ((curx - cury) << 2) + 10;
			cury--;
		}
		curx++;
	}

}


uint16_t G2D_Text(int16_t x, int16_t y, const uint8_t *font, char *str, uint32_t color, uint32_t bgcolor) {
	// Calculating color array for anty-aliasing
	uint32_t text_clut[4];
	uint32_t a1, a2, r1, r2, b1, b2, g1, g2;

	a1 = 0;
	a2 = 0;

	switch (BSP_LCD_GetColorMode()) {
	case LCD_COLOR_MODE_ARGB8888:
	case LCD_COLOR_MODE_RGB888:
		a1 = (color & 0xFF000000) >> 24;
		a2 = (bgcolor & 0xFF000000) >> 24;
		r1 = (color & 0x00FF0000) >> 16;
		g1 = (color & 0x0000FF00) >> 8;
		b1 = (color & 0x000000FF) >> 0;
		r2 = (bgcolor & 0x00FF0000) >> 16;
		g2 = (bgcolor & 0x0000FF00) >> 8;
		b2 = (bgcolor & 0x000000FF) >> 0;

		text_clut[0] = bgcolor;
		text_clut[1] = (((a1 * 85 + a2 * 171) >> 8) << 24) | (((r1 * 85 + r2 * 171) >> 8) << 16) | (((g1 * 85 + g2 * 171) >> 8) << 8) | (((b1 * 85 + b2 * 171) >> 8) << 0);
		text_clut[2] = (((a1 * 171 + a2 * 85) >> 8) << 24) | (((r1 * 171 + r2 * 85) >> 8) << 16) | (((g1 * 171 + g2 * 85) >> 8) << 8) | (((b1 * 171 + b2 * 85) >> 8) << 0);
		text_clut[3] = color;
		break;
	case LCD_COLOR_MODE_ARGB1555:
		r1 = (color & 0x7C00) >> 10;
		g1 = (color & 0x03E0) >> 5;
		b1 = (color & 0x001F) >> 0;
		r2 = (bgcolor & 0x7C00) >> 10;
		g2 = (bgcolor & 0x03E0) >> 5;
		b2 = (bgcolor & 0x001F) >> 0;

		text_clut[0] = bgcolor;
		text_clut[1] = (1 << 15) | (((r1 * 85 + r2 * 171) >> 8) << 10) | (((g1 * 85 + g2 * 171) >> 8) << 5) | (((b1 * 85 + b2 * 171) >> 8) << 0);
		text_clut[2] = (1 << 15) | (((r1 * 171 + r2 * 85) >> 8) << 10) | (((g1 * 171 + g2 * 85) >> 8) << 5) | (((b1 * 171 + b2 * 85) >> 8) << 0);
		text_clut[3] = color;
		break;
	case LCD_COLOR_MODE_ARGB4444:
		a1 = (color & 0xF000) >> 12;
		r1 = (color & 0x0F00) >> 8;
		g1 = (color & 0x00F0) >> 4;
		b1 = (color & 0x000F) >> 0;
		a2 = (bgcolor & 0XF000) >> 12;
		r2 = (bgcolor & 0x0F00) >> 8;
		g2 = (bgcolor & 0x00F0) >> 4;
		b2 = (bgcolor & 0x000F) >> 0;

		text_clut[0] = bgcolor;
		text_clut[1] = (((a1 * 85 + a2 * 171) >> 8) << 12) | (((r1 * 85 + r2 * 171) >> 8) << 8) | (((g1 * 85 + g2 * 171) >> 8) << 4) | (((b1 * 85 + b2 * 171) >> 8) << 0);
		text_clut[2] = (((a1 * 171 + a2 * 85) >> 8) << 12) | (((r1 * 171 + r2 * 85) >> 8) << 8) | (((g1 * 171 + g2 * 85) >> 8) << 4) | (((b1 * 171 + b2 * 85) >> 8) << 0);
		text_clut[3] = color;
		break;
	case LCD_COLOR_MODE_AL88:
	case LCD_COLOR_MODE_L8:
		// No anty-aliasing in AL and L modes
		text_clut[0] = bgcolor;
		text_clut[1] = bgcolor;
		text_clut[2] = color;
		text_clut[3] = color;
		break;
	}


	uint8_t h = *(font);	// Font height
	uint8_t w = 0;			// Character width
	uint8_t flag = 0;		// Flag -> character pointer not to beupdated in first loop

	// Waiting for DMA2D to stop
	BSP_LCD_DMA2D_Wait();

	while (*str > 0) {
		x += w;
		if (flag) str += 1;
		flag = 1;

		// Getting character width
		w = _charw(font, *str);

		// Is character within screen area?
		if (((x + w) < 0) || (x >= LCD_WIDTH) || ((y + h) < 0) || (y >= LCD_HEIGHT)) continue; // No rendering

		// Character rendering
		_char(x, y, font, *str, text_clut);

	}
	return x + w;
}


uint16_t G2D_TextBlend(int16_t x, int16_t y, const uint8_t *font, char *str, uint32_t color) {
	// Calculating color array for anty-aliasing
	uint32_t text_clut[4];
	uint32_t a1 = 0;

	switch (BSP_LCD_GetColorMode()) {
	case LCD_COLOR_MODE_ARGB8888:
	case LCD_COLOR_MODE_RGB888:
		a1 = (color & 0xFF000000) >> 24;
		text_clut[0] = 0;
		text_clut[1] = (((a1 * 85) >> 8) << 24) | (color & 0x00FFFFFF);
		text_clut[2] = (((a1 * 171) >> 8) << 24) | (color & 0x00FFFFFF);
		text_clut[3] = color;
		break;
	case LCD_COLOR_MODE_ARGB1555:
		text_clut[0] = 0;
		text_clut[1] = 0;
		text_clut[2] = color;
		text_clut[3] = color;
		break;
	case LCD_COLOR_MODE_ARGB4444:
		a1 = (color & 0xF000) >> 12;

		text_clut[0] = 0;
		text_clut[1] = (((a1 * 85) >> 8) << 12) | (color & 0x0FFF);
		text_clut[2] = (((a1 * 171) >> 8) << 12) | (color & 0x0FFF);
		text_clut[3] = color;
		break;

	case LCD_COLOR_MODE_AL88:
	case LCD_COLOR_MODE_L8:
		// No anty-aliasing in AL and L modes
		text_clut[0] = 0;
		text_clut[1] = 0;
		text_clut[2] = color;
		text_clut[3] = color;
		break;
	}


	uint8_t h = *(font);	// Font height
	uint8_t w = 0;			// Character width
	uint8_t flag = 0;		// Flag -> character pointer not to beupdated in first loop

	// Waiting for DMA2D to stop
	BSP_LCD_DMA2D_Wait();

	while (*str > 0) {
		x += w;
		if (flag) str += 1;
		flag = 1;

		// Getting character width
		w = _charw(font, *str);

		// Is character within screen area?
		if (((x + w) < 0) || (x >= LCD_WIDTH) || ((y + h) < 0) || (y >= LCD_HEIGHT)) continue; // No rendering

		// Character rendering
		_charblend(x, y, font, *str, text_clut);

	}
	return x + w;
}


uint8_t G2D_GetTextHeight(const uint8_t *font) {
	return *(font);
}


void G2D_DrawBitmapBlend(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t alpha) {
	// Checking if bitmap is on the screen
	if (((x + width) < 1) || (x >= LCD_WIDTH) || ((y + height) < 1) || (y >= LCD_HEIGHT)) return;

	// Calculating parameters for partial display on screen
	int16_t xx0 = MAX(x, 0);
	int16_t yy0 = MAX(y, 0);
	int16_t xx1 = MIN(x + width, LCD_WIDTH);
	int16_t yy1 = MIN(y + height, LCD_HEIGHT);
	uint16_t ww = xx1 - xx0;
	uint16_t hh = yy1 - yy0;
	uint16_t oscr = width - ww;
	uint16_t odest = LCD_WIDTH - ww;
	uint16_t sx = 0;
	uint16_t sy = 0;
	if (x < 0) sx = -x;
	if (y < 0) sy = -y;

	// Sending rendered bitmap to screen
	uint32_t src_addr = sourcedata + ((sx + sy * width) * BSP_LCD_GetBytesPerPixel());
	BSP_LCD_CopyBufBlend(src_addr, oscr, xx0, yy0, odest, ww, hh, alpha);
}

void G2D_DrawBitmapBlendC(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, uint8_t alpha) {
	x -= width >> 1;
	y -= height >> 1;
	G2D_DrawBitmapBlend(sourcedata, x, y, width, height, alpha);
}

void G2D_DrawBitmap(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height) {
	// Checking if bitmap is on the screen
	if (((x + width) < 1) || (x >= LCD_WIDTH) || ((y + height) < 1) || (y >= LCD_HEIGHT)) return;

	// Calculating parameters for partial display on screen
	int16_t xx0 = MAX(x, 0);
	int16_t yy0 = MAX(y, 0);
	int16_t xx1 = MIN(x + width, LCD_WIDTH);
	int16_t yy1 = MIN(y + height, LCD_HEIGHT);
	uint16_t ww = xx1 - xx0;
	uint16_t hh = yy1 - yy0;
	uint16_t oscr = width - ww;
	uint16_t odest = LCD_WIDTH - ww;
	uint16_t sx = 0;
	uint16_t sy = 0;
	if (x < 0) sx = -x;
	if (y < 0) sy = -y;

	// Sending rendered bitmap to screen
	uint32_t src_addr = sourcedata + ((sx + sy * width) * BSP_LCD_GetBytesPerPixel());
	BSP_LCD_CopyBuf(src_addr, oscr, xx0, yy0, odest, ww, hh);
}


void G2D_DrawBitmapC(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height) {
	x -= width >> 1;
	y -= height >> 1;
	G2D_DrawBitmap(sourcedata, x, y, width, height);
}


void G2D_DrawBitmapRotate(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, float angle) {

	// Checking if bitmap is within screen area
	if (((x + width) < 1) || (x >= LCD_WIDTH)) return;
	if (((y + height) < 1) || (y >= LCD_HEIGHT)) return;

	// Calculating destination address
	uint32_t faddr = BSP_LCD_GetEditFrameAddr();

	// Setting up buffer read function
	uint32_t (*_getbufpixel)(uint32_t offset, int16_t width, int16_t x, int16_t y);
	switch (BSP_LCD_GetColorMode()) {
	case LCD_COLOR_MODE_ARGB8888:
		_getbufpixel = _getbufpixel_ARGB8888;
		break;
	case LCD_COLOR_MODE_ARGB1555:
		_getbufpixel = _getbufpixel_ARGB1555;
		break;
	case LCD_COLOR_MODE_ARGB4444:
		_getbufpixel = _getbufpixel_ARGB4444;
		break;
	case LCD_COLOR_MODE_RGB888:
		_getbufpixel = _getbufpixel_RGB888;
		break;
	case LCD_COLOR_MODE_AL88:
		_getbufpixel = _getbufpixel_AL88;
		break;
	case LCD_COLOR_MODE_L8:
		_getbufpixel = _getbufpixel_L8;
		break;
	default: return;
	}

	// Calculating required parameters
	int32_t sin_a = sin(-angle) * 0xFFFF;
	int32_t cos_a = cos(-angle) * 0xFFFF;
	int32_t ox = width >> 1;
	int32_t oy = height >> 1;

	int32_t sin_a_x = -sin_a * ox;
	int32_t cos_a_x = -cos_a * ox;
	int32_t sin_a_y = -sin_a * oy;
	int32_t cos_a_y = -cos_a * oy;

	int32_t tcx = cos_a_x - sin_a_y + (ox << 16);
	int32_t tcy = sin_a_x + cos_a_y + (oy << 16);

	int32_t cx = tcx;
	int32_t cy = tcy;

	int32_t cxx = cx >> 16;
	int32_t cyy = cy >> 16;

	BSP_LCD_DMA2D_Wait();

	for (int32_t iy = y; iy < (height + y); iy++) {
		for (int32_t ix = x; ix < (width + x); ix++) {

			cx += cos_a;
			cxx = cx >> 16;
			cy += sin_a;
			cyy = cy >> 16;

			// By casting to uint32_t we can check if variable is within range <0,limit) with only one comparison.
			if (((uint32_t)cxx > width) || ((uint32_t)cyy > height)) continue;
			BSP_LCD_UpdatePixel(faddr, ix, iy, _getbufpixel(sourcedata, width, cxx, cyy));
		}
		tcx -= sin_a;
		tcy += cos_a;
		cx = tcx;
		cy = tcy;
	}
}


void G2D_DrawBitmapRotateC(uint32_t sourcedata, int16_t x, int16_t y, int16_t width, int16_t height, float angle) {
	x -= width >> 1;
	y -= height >> 1;
	G2D_DrawBitmapRotate(sourcedata, x, y, width, height, angle);
}


void G2D_DrawIcon(uint32_t iconsource, int16_t x, int16_t y, uint32_t color, uint32_t bgcolor) {
	// Calculating color array for anty-aliasing
	uint32_t icon_clut[4];
	uint32_t a1, a2, r1, r2, b1, b2, g1, g2;

	switch (BSP_LCD_GetColorMode()) {
	case LCD_COLOR_MODE_ARGB8888:
	case LCD_COLOR_MODE_RGB888:
		a1 = (color & 0xFF000000) >> 24;
		r1 = (color & 0x00FF0000) >> 16;
		g1 = (color & 0x0000FF00) >> 8;
		b1 = (color & 0x000000FF) >> 0;
		a2 = (bgcolor & 0xFF000000) >> 24;
		r2 = (bgcolor & 0x00FF0000) >> 16;
		g2 = (bgcolor & 0x0000FF00) >> 8;
		b2 = (bgcolor & 0x000000FF) >> 0;

		icon_clut[0] = bgcolor;
		icon_clut[1] = (((a1 * 85 + a2 * 171) >> 8) << 24) | (((r1 * 85 + r2 * 171) >> 8) << 16) | (((g1 * 85 + g2 * 171) >> 8) << 8) | (((b1 * 85 + b2 * 171) >> 8) << 0);
		icon_clut[2] = (((a1 * 171 + a2 * 85) >> 8) << 24) | (((r1 * 171 + r2 * 85) >> 8) << 16) | (((g1 * 171 + g2 * 85) >> 8) << 8) | (((b1 * 171 + b2 * 85) >> 8) << 0);
		icon_clut[3] = color;
		break;
	case LCD_COLOR_MODE_ARGB1555:
		r1 = (color & 0x7C00) >> 10;
		g1 = (color & 0x03E0) >> 5;
		b1 = (color & 0x001F) >> 0;
		r2 = (bgcolor & 0x7C00) >> 10;
		g2 = (bgcolor & 0x03E0) >> 5;
		b2 = (bgcolor & 0x001F) >> 0;

		icon_clut[0] = bgcolor;
		icon_clut[1] = (1 << 15) | (((r1 * 85 + r2 * 171) >> 8) << 10) | (((g1 * 85 + g2 * 171) >> 8) << 5) | (((b1 * 85 + b2 * 171) >> 8) << 0);
		icon_clut[2] = (1 << 15) | (((r1 * 171 + r2 * 85) >> 8) << 10) | (((g1 * 171 + g2 * 85) >> 8) << 5) | (((b1 * 171 + b2 * 85) >> 8) << 0);
		icon_clut[3] = color;
		break;
	case LCD_COLOR_MODE_ARGB4444:
		a1 = (color & 0xF000) >> 12;
		r1 = (color & 0x0F00) >> 8;
		g1 = (color & 0x00F0) >> 4;
		b1 = (color & 0x000F) >> 0;
		a2 = (bgcolor & 0XF000) >> 12;
		r2 = (bgcolor & 0x0F00) >> 8;
		g2 = (bgcolor & 0x00F0) >> 4;
		b2 = (bgcolor & 0x000F) >> 0;

		icon_clut[0] = bgcolor;
		icon_clut[1] = (((a1 * 85 + a2 * 171) >> 8) << 12) | (((r1 * 85 + r2 * 171) >> 8) << 8) | (((g1 * 85 + g2 * 171) >> 8) << 4) | (((b1 * 85 + b2 * 171) >> 8) << 0);
		icon_clut[2] = (((a1 * 171 + a2 * 85) >> 8) << 12) | (((r1 * 171 + r2 * 85) >> 8) << 8) | (((g1 * 171 + g2 * 85) >> 8) << 4) | (((b1 * 171 + b2 * 85) >> 8) << 0);
		icon_clut[3] = color;
		break;
	case LCD_COLOR_MODE_AL88:
	case LCD_COLOR_MODE_L8:
		// No anty-aliasing in AL and L modes
		icon_clut[0] = bgcolor;
		icon_clut[1] = bgcolor;
		icon_clut[2] = color;
		icon_clut[3] = color;
		break;
	}

	// Calculating destination address
	uint32_t faddr = BSP_LCD_GetEditFrameAddr();

	// Decoding compressed icon data
	uint8_t *pdata;
	pdata = (uint8_t *)iconsource;

	uint16_t width = *(uint16_t *)(pdata++);
	pdata++;
	uint16_t height = *(uint16_t *)(pdata++);
	pdata++;
	int16_t xx = x;
	int16_t yy = y;

	// Waiting for DMA2D to stop
	BSP_LCD_DMA2D_Wait();

	while (yy < (y+height)) {
		uint8_t j = *(pdata++);
		uint8_t m = j >> 6;
		uint8_t r = j & 0x3F;
		for (uint32_t z = 0; z<r; z++) {
			BSP_LCD_UpdatePixel(faddr, xx, yy, icon_clut[m]);
			xx++;
			if (xx == (x+width)) {
				xx = x;
				yy++;
			}
		}
	}
}


void G2D_DrawIconC(uint32_t iconsource, int16_t x, int16_t y, uint32_t color, uint32_t bgcolor) {
	// Decoding width and height
	uint8_t *pdata;
	pdata = (uint8_t *)iconsource;
	uint16_t width = *(uint16_t *)(pdata++);
	pdata++;
	uint16_t height = *(uint16_t *)(pdata++);

	x -= width >> 1;
	y -= height >> 1;
	G2D_DrawIcon(iconsource, x, y, color, bgcolor);
}


void G2D_DrawIconBlend(uint32_t iconsource, int16_t x, int16_t y, uint32_t color) {
	// Calculating color array for anty-aliasing
	uint32_t icon_clut[4];
	uint32_t a1 = 0;

	switch (BSP_LCD_GetColorMode()) {
	case LCD_COLOR_MODE_ARGB8888:
	case LCD_COLOR_MODE_RGB888:
		a1 = (color & 0xFF000000) >> 24;
		icon_clut[0] = 0;
		icon_clut[1] = (((a1 * 85) >> 8) << 24) | (color & 0x00FFFFFF);
		icon_clut[2] = (((a1 * 171) >> 8) << 24) | (color & 0x00FFFFFF);
		icon_clut[3] = color;
		break;
	case LCD_COLOR_MODE_ARGB1555:
		icon_clut[0] = 0;
		icon_clut[1] = 0;
		icon_clut[2] = color;
		icon_clut[3] = color;
		break;
	case LCD_COLOR_MODE_ARGB4444:
		a1 = (color & 0xF000) >> 12;
		icon_clut[0] = 0;
		icon_clut[1] = (((a1 * 85) >> 8) << 12) | (color & 0x0FFF);
		icon_clut[2] = (((a1 * 171) >> 8) << 12) | (color & 0x0FFF);
		icon_clut[3] = color;
		break;

	case LCD_COLOR_MODE_AL88:
	case LCD_COLOR_MODE_L8:
		// No anty-aliasing in AL and L modes
		icon_clut[0] = 0;
		icon_clut[1] = 0;
		icon_clut[2] = color;
		icon_clut[3] = color;
		break;
	}

	// Calculating destination address
	uint32_t faddr = BSP_LCD_GetEditFrameAddr();

	// Decoding compressed icon data
	uint8_t *pdata;
	pdata = (uint8_t *)iconsource;

	uint16_t width = *(uint16_t *)(pdata++);
	pdata++;
	uint16_t height = *(uint16_t *)(pdata++);
	pdata++;
	int16_t xx = x;
	int16_t yy = y;

	// Waiting for DMA2D to stop
	BSP_LCD_DMA2D_Wait();

	while (yy < (y+height)) {
		uint8_t j = *(pdata++);
		uint8_t m = j >> 6;
		uint8_t r = j & 0x3F;
		for (uint32_t z = 0; z<r; z++) {
			BSP_LCD_UpdatePixelBlend(faddr, xx, yy, icon_clut[m]);
			xx++;
			if (xx == (x+width)) {
				xx = x;
				yy++;
			}
		}
	}
}


void G2D_DrawIconBlendC(uint32_t iconsource, int16_t x, int16_t y, uint32_t color) {
	// Decoding width and height
	uint8_t *pdata;
	pdata = (uint8_t *)iconsource;
	uint16_t width = *(uint16_t *)(pdata++);
	pdata++;
	uint16_t height = *(uint16_t *)(pdata++);

	x -= width >> 1;
	y -= height >> 1;
	G2D_DrawIconBlend(iconsource, x, y, color);
}


uint16_t G2D_GetIconHeight(uint32_t iconsource) {
	return *((uint16_t *)iconsource + 1);
}


uint16_t G2D_GetIconWidth(uint32_t iconsource) {
	return *((uint16_t *)iconsource);
}


void G2D_DrawJPEG(uint32_t jpeg_addr, uint32_t jpeg_size, int16_t x, int16_t y) {
	// TODO: Checking if JPEG is within borders of screen

	BSP_LCD_DecodeJPEG(jpeg_addr, jpeg_size);
	BSP_LCD_CopyBufJPEG((uint16_t)x, (uint16_t)y);
}

void G2D_DrawJPEGC(uint32_t jpeg_addr, uint32_t jpeg_size, int16_t x, int16_t y) {
	// TODO: Checking if JPEG is within borders of screen

	BSP_LCD_DecodeJPEG(jpeg_addr, jpeg_size);

	x -= BSP_STM32_JPEG_GetWidth(JPEG) >> 1;
	y -= BSP_STM32_JPEG_GetHeight(JPEG) >> 1;

	BSP_LCD_CopyBufJPEG((uint16_t)x, (uint16_t)y);
}

void G2D_DrawLastJPEG(int16_t x, int16_t y) {
	// TODO: Checking if JPEG is within borders of screen
	BSP_LCD_CopyBufJPEG((uint16_t)x, (uint16_t)y);
}

void G2D_DrawLastJPEGC(int16_t x, int16_t y) {
	// TODO: Checking if JPEG is within borders of screen

	x -= BSP_STM32_JPEG_GetWidth(JPEG) >> 1;
	y -= BSP_STM32_JPEG_GetHeight(JPEG) >> 1;

	BSP_LCD_CopyBufJPEG((uint16_t)x, (uint16_t)y);
}


void G2D_DecodeJPEG(uint32_t jpeg_addr, uint32_t jpeg_size) {

	BSP_LCD_DecodeJPEG(jpeg_addr, jpeg_size);

}

void G2D_DrawTile(uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y) {
	// Checking if tile is on the screen
	if ((int32_t)(x + tile_width) < 1) return;
	if (x >= LCD_WIDTH) return;
	if ((int32_t)(y + tile_height) < 1) return;
	if (y >= LCD_HEIGHT) return;

	// Calculating parameters for partial display on screen
	int16_t xx0 = MAX(x, 0);
	int16_t yy0 = MAX(y, 0);
	int16_t xx1 = MIN(x + tile_width, LCD_WIDTH);
	int16_t yy1 = MIN(y + tile_height, LCD_HEIGHT);
	uint16_t ww = xx1 - xx0;
	uint16_t hh = yy1 - yy0;
	uint16_t oscr = (tileset_cols * tile_width) - ww;
	uint16_t odest = LCD_WIDTH - ww;
	uint16_t sx = tile_col * tile_width;
	uint16_t sy = tile_row * tile_height;
	if (x < 0) sx -= x;
	if (y < 0) sy -= y;

	// Sending rendered tile to screen
	uint32_t src_addr = tileset_addr + ((sx + sy * (tile_width * tileset_cols)) * BSP_LCD_GetBytesPerPixel());
	BSP_LCD_CopyBuf(src_addr, oscr, xx0, yy0, odest, ww, hh);

}

void G2D_DrawTileC(uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y) {
	x -= tile_width >> 1;
	y -= tile_height >> 1;

	G2D_DrawTile(tileset_addr, tileset_cols, tile_width, tile_height, tile_col, tile_row, x, y);
}

void G2D_DrawTileBlend(uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y) {
	// Checking if tile is on the screen
	if ((int32_t)(x + tile_width) < 1) return;
	if (x >= LCD_WIDTH) return;
	if ((int32_t)(y + tile_height) < 1) return;
	if (y >= LCD_HEIGHT) return;

	// Calculating parameters for partial display on screen
	int16_t xx0 = MAX(x, 0);
	int16_t yy0 = MAX(y, 0);
	int16_t xx1 = MIN(x + tile_width, LCD_WIDTH);
	int16_t yy1 = MIN(y + tile_height, LCD_HEIGHT);
	uint16_t ww = xx1 - xx0;
	uint16_t hh = yy1 - yy0;
	uint16_t oscr = (tileset_cols * tile_width) - ww;
	uint16_t odest = LCD_WIDTH - ww;
	uint16_t sx = tile_col * tile_width;
	uint16_t sy = tile_row * tile_height;
	if (x < 0) sx -= x;
	if (y < 0) sy -= y;

	// Sending rendered tile to screen
	uint32_t src_addr = tileset_addr + ((sx + sy * (tile_width * tileset_cols)) * BSP_LCD_GetBytesPerPixel());
	BSP_LCD_CopyBufBlend(src_addr, oscr, xx0, yy0, odest, ww, hh, 255);

}

void G2D_DrawTileBlendC(uint32_t tileset_addr, uint32_t tileset_cols, uint32_t tile_width, uint32_t tile_height, uint32_t tile_col, uint32_t tile_row, int16_t x, int16_t y) {
	x -= tile_width >> 1;
	y -= tile_height >> 1;

	G2D_DrawTileBlend(tileset_addr, tileset_cols, tile_width, tile_height, tile_col, tile_row, x, y);
}

uint32_t G2D_Color(uint32_t color, uint8_t alpha) {
	return BSP_LCD_Color(color, alpha);
}

uint32_t G2D_Alpha(uint32_t color, uint8_t alpha) {
	return BSP_LCD_Alpha(color, alpha);
}

void G2D_CopyBuf(uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t width, uint16_t height) {
	BSP_LCD_CopyBuf(src_addr, offsline_src, x_dest, y_dest, (LCD_WIDTH - width), width, height);
}

void G2D_CopyBufBlend(uint32_t src_addr, uint16_t offsline_src, uint16_t x_dest, uint16_t y_dest, uint16_t width, uint16_t height, uint8_t alpha) {
	BSP_LCD_CopyBufBlend(src_addr, offsline_src, x_dest, y_dest, (LCD_WIDTH - width), width, height, alpha);
}

void G2D_CacheFrame(void) {
	BSP_LCD_CacheFrame();
}

void G2D_RestoreFrame(void) {
	BSP_LCD_RestoreFrame();
}

#endif

