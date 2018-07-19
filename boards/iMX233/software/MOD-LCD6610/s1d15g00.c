// **************************************************************************************************************
// lcd.c
//
// Nokia 6610 LCD Display Driver (Epson S1D15G00 Controller)
//
// Controller used for LCD Display is a Epson S1D15G00 driver
// Note: For Olimex SAM7-EX256 boards with the G8 decal or the Sparkfun Color LCD 128x128 Nokia Knock-Off
//
//
// We will use a 132 x 132 pixel matrix - each pixel has 12 bits of color information.
//
// _____
// | |
// ___________________|___|___
// | ====================== |
// ^ (131,0) |------------------------- |(131,131)
// | | |
// | | |
// | | |
// | | |
// | | |
// | Rows| Nokia 6610 Display | Note: In general, you can't view column 130 or column 131
// | | |
// X | |
// | | |
// | | |
// | | |
// | | |
// | | |
// | |--------------------------|
// (0,0) Columns (0,131)
//
// ------------Y-------------->
//
//
// 132 x 132 pixel matrix has two methods to specify the color info
//
// 1. 12 bits per pixel
// requires command and 1.5 data bytes to specify a single pixel
// (3 data bytes can specify 2 pixels)
//
// 2. 8 bits per pixel
// requires one command and one data byte to specify the single pixel\//
// note: pixel data byte converted by RGB table to 12-bit format above
//
// THIS IMPLEMENTATION USES THE 12 BITS PER PIXEL METHOD!
// -------------------------
//
//
//
//
// HARDWARE INTERFACE
// ------------------
//
// The Nokia 6610 display uses a SPI serial interface (9 bits)
//
// PA2 = LCD Reset (set to low to reset)
// PA12 = LCD chip select (set to low to select the LCD chip)
// PA16 = SPI0_MISO Master In - Slave Out (not used in Olimex SAM7-EX256 LCD interface)
// PA17 = SPI0_MOSI Master Out - Slave In pin (Serial Data to LCD slave)
// PA18 = SPI0_SPCK Serial Clock (to LCD slave)
//
// SPI baud rate set to MCK/2 = 48054841/3 = 16018280 baud
// (period = 62 nsec, OK since 50 nsec period is min for S1D15G00)
//
// The important thing to note is that you CANNOT read from the LCD!
//
//
// Author: James P Lynch August 30, 2007
// ***************************************************************************************************************

// **************************************
// Include Files
// **************************************

#include "s1d15g00.h"		 									// LCD-NOKIA6610 Epson:S1D1G00 Controller
#include "gpio-mmap.h"
#include "unistd.h"
#include "font.h"
#include "bmp.h"

// **************************************
// forward references
// **************************************
const unsigned char FONT6x8[97][8];
const unsigned char FONT8x8[97][8];
const unsigned char FONT8x16[97][16];



void SPI_Init(void)
{
	gpio_map();
	gpio_output(0, 20);
	GPIO_WRITE(0, 20, 0);
	gpio_input(0, 0);
	gpio_output(0, 24);
	gpio_output(0, 3);
	GPIO_WRITE(0, 24, 0);
	GPIO_WRITE(0, 3, 0);		
	
}
// *****************************************************************************
// WriteSpiCommand.c
//
// Writes 9-bit command to LCD display via SPI interface
//
// Inputs: data - Epson S1D15G00 controller/driver command
//
//
// Note: clears bit 8 to indicate command transfer
//
// Author: Olimex, James P Lynch August 30, 2007
// *****************************************************************************
void WriteSpiCommand(volatile unsigned int command) 
{
	char ch, j;
  
	GPIO_WRITE(0, 3, 0);
	
	
	GPIO_WRITE(0, 20, 0);
	GPIO_WRITE(0, 24, 1);
	GPIO_WRITE(0, 24, 0);
			
	


	for(j = 0; j < 8; j++)
	
	{
		ch = (command & 0x80) ? 1 : 0;
		command <<= 1;		
		
		GPIO_WRITE(0, 20, ch);
		GPIO_WRITE(0, 24, 1);
		GPIO_WRITE(0, 24, 0);
			
	}		
	
	GPIO_WRITE(0, 20, 0);
	GPIO_WRITE(0, 3, 1);											// Stop CS = Disable
}

// *****************************************************************************
// WriteSpiData.c
//
// Writes 9-bit command to LCD display via SPI interface
//
// Inputs: data - Epson S1D15G00 controller/driver command
//
//
// Note: Sets bit 8 to indicate data transfer
//
// Author: Olimex, James P Lynch August 30, 2007
// *****************************************************************************
void WriteSpiData(volatile unsigned int data) 
{
	char ch, j;
  
	GPIO_WRITE(0, 3, 0);
	

	GPIO_WRITE(0, 20, 1);
	GPIO_WRITE(0, 24, 1);
	GPIO_WRITE(0, 24, 0);
		
	
	for(j = 0; j < 8; j++)
	
	{
		ch = (data & 0x80) ? 1 : 0;
		data <<= 1;		
		
		GPIO_WRITE(0, 20, ch);
		GPIO_WRITE(0, 24, 1);
		GPIO_WRITE(0, 24, 0);
			
	}		
	
	GPIO_WRITE(0, 20, 0);
	GPIO_WRITE(0, 3, 1);											// Stop CS = Disable  
}


// *****************************************************************************
// InitLcd.c
//
// Initializes the Epson S1D15G00 LCD Controller
//
// Inputs: none
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************
void InitLcd(void) 
{

  // Display control
  WriteSpiCommand(DISCTL);
  WriteSpiData(0x00); // P1: 0x00 = 2 divisions, switching period=8 (default)
  WriteSpiData(0x20); // P2: 0x20 = nlines/4 - 1 = 132/4 - 1 = 32)
  WriteSpiData(0x00); // P3: 0x00 = no inversely highlighted lines

  // COM scan
  WriteSpiCommand(COMSCN);
  WriteSpiData(1); // P1: 0x01 = Scan 1->80, 160<-81

  // Internal oscilator ON
  WriteSpiCommand(OSCON);

  // Sleep out
  WriteSpiCommand(SLPOUT);

  // Power control
  WriteSpiCommand(PWRCTR);
  WriteSpiData(0x0f); // reference voltage regulator on, circuit voltage follower on, BOOST ON

  // Inverse display
  WriteSpiCommand(DISINV);

  // Data control
  WriteSpiCommand(DATCTL);
  WriteSpiData(0x01); // P1: 0x01 = page address inverted, column address normal, address scan in column direction
  WriteSpiData(0x00); // P2: 0x00 = RGB sequence (default value)
  WriteSpiData(0x02); // P3: 0x02 = Grayscale -> 16 (selects 12-bit color, type A)

  // Voltage control (contrast setting)
  WriteSpiCommand(VOLCTR);
  WriteSpiData(38); // P1 = 32 volume value (experiment with this value to get the best contrast)
  WriteSpiData(3); // P2 = 3 resistance ratio (only value that works)

  // allow power supply to stabilize
	sleep(1);

  // turn on the display
  WriteSpiCommand(DISON);
}

// *****************************************************************************
// LCDWrite130x130bmp.c
//
// Writes the entire screen from a bmp file
// Uses Olimex BmpToArray.exe utility
//
// Inputs: picture in bmp.h
//
// Author: Olimex, James P Lynch August 30, 2007
// *****************************************************************************
void LCDWrite130x130bmp(void) 
{
  long j; // loop counter

  // Data control (need to set "normal" page address for Olimex photograph)
  WriteSpiCommand(DATCTL);
  WriteSpiData(0x00); // P1: 0x00 = page address normal, column address normal, address scan in column direction
  WriteSpiData(0x00); // P2: 0x00 = RGB sequence (default value)
  WriteSpiData(0x02); // P3: 0x02 = Grayscale -> 16

  // Display OFF
  WriteSpiCommand(DISOFF);

  // Column address set (command 0x2A)
  WriteSpiCommand(CASET);
  WriteSpiData(0);
  WriteSpiData(131);

  // Page address set (command 0x2B)
  WriteSpiCommand(PASET);
  WriteSpiData(0);
  WriteSpiData(131);

  // WRITE MEMORY
  WriteSpiCommand(RAMWR);
  for(j = 0; j < 25740; j++) 
  {
    WriteSpiData(bmp[j]);
  }

  // Data control (return to "inverted" page address)
  WriteSpiCommand(DATCTL);
  WriteSpiData(0x01); // P1: 0x01 = page address inverted, column address normal, address scan in column direction
  WriteSpiData(0x00); // P2: 0x00 = RGB sequence (default value)
  WriteSpiData(0x02); // P3: 0x02 = Grayscale -> 16

  // Display On
  WriteSpiCommand(DISON);
}
// *****************************************************************************
// LCDClearScreen.c
//
// Clears the LCD screen to single color (BLACK)
//
// Inputs: none
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************
void LCDClearScreen(void) 
{
  long i; // loop counter

  // Row address set (command 0x2B)
  WriteSpiCommand(PASET);
  WriteSpiData(0);
  WriteSpiData(131);

  // Column address set (command 0x2A)
  WriteSpiCommand(CASET);
  WriteSpiData(0);
  WriteSpiData(131);

  // set the display memory to BLACK
  WriteSpiCommand(RAMWR);
  for(i = 0; i < ((131 * 131) / 2); i++) 
  {
    WriteSpiData((BLACK >> 4) & 0xFF);
    WriteSpiData(((BLACK & 0xF) << 4) | ((BLACK >> 8) & 0xF));
    WriteSpiData(BLACK & 0xFF);
  }
}

// *************************************************************************************
// LCDSetPixel.c
//
// Lights a single pixel in the specified color at the specified x and y addresses
//
// Inputs: x = row address (0 .. 131)
// y = column address (0 .. 131)
// color = 12-bit color value rrrrggggbbbb
// rrrr = 1111 full red
// :
// 0000 red is off
//
// gggg = 1111 full green
// :
// 0000 green is off
//
// bbbb = 1111 full blue
// :
// 0000 blue is off
//
// Returns: nothing
//
// Note: see lcd.h for some sample color settings
//
// Author: James P Lynch August 30, 2007
// *************************************************************************************
void LCDSetPixel(int x, int y, int color) 
{
  // Row address set (command 0x2B)
  WriteSpiCommand(PASET);
  WriteSpiData(x);
  WriteSpiData(x);

  // Column address set (command 0x2A)
  WriteSpiCommand(CASET);
  WriteSpiData(y);
  WriteSpiData(y);

  // Now illuminate the pixel (2nd pixel will be ignored)
  WriteSpiCommand(RAMWR);
  WriteSpiData((color >> 4) & 0xFF);
  WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
  WriteSpiData(color & 0xFF);
}

// *************************************************************************************************
// LCDSetLine.c
//
// Draws a line in the specified color from (x0,y0) to (x1,y1)
//
// Inputs: x = row address (0 .. 131)
// y = column address (0 .. 131)
// color = 12-bit color value rrrrggggbbbb
// rrrr = 1111 full red
// :
// 0000 red is off
//
// gggg = 1111 full green
// :
// 0000 green is off
//
// bbbb = 1111 full blue
// :
// 0000 blue is off
//
// Returns: nothing
//
// Note: good write-up on this algorithm in Wikipedia (search for Bresenham's line algorithm)
// see lcd.h for some sample color settings
//
// Authors: Dr. Leonard McMillan, Associate Professor UNC
// Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
//
// Note: taken verbatim from Professor McMillan's presentation:
// http://www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html
//
// *************************************************************************************************
void LCDSetLine(int x0, int y0, int x1, int y1, int color) 
{
  int dy = y1 - y0;
  int dx = x1 - x0;
  int stepx, stepy;
  if (dy < 0) { dy = -dy; stepy = -1; } else { stepy = 1; }
  if (dx < 0) { dx = -dx; stepx = -1; } else { stepx = 1; }
  dy <<= 1; // dy is now 2*dy
  dx <<= 1; // dx is now 2*dx
  LCDSetPixel(x0, y0, color);
  if (dx > dy) 
  {
    int fraction = dy - (dx >> 1); // same as 2*dy - dx
    while (x0 != x1) 
    {
      if (fraction >= 0) 
	  {
        y0 += stepy;
        fraction -= dx; // same as fraction -= 2*dx
      }
      x0 += stepx;
      fraction += dy; // same as fraction -= 2*dy
      LCDSetPixel(x0, y0, color);
    }
  } 
  else 
  {
    int fraction = dx - (dy >> 1);
    while (y0 != y1) 
	{
      if (fraction >= 0) 
	  {
        x0 += stepx;
        fraction -= dy;
      }
      y0 += stepy;
      fraction += dx;
      LCDSetPixel(x0, y0, color);
    }
  }
}

// *****************************************************************************************
// LCDSetRect.c
//
// Draws a rectangle in the specified color from (x1,y1) to (x2,y2)
// Rectangle can be filled with a color if desired
//
// Inputs: x = row address (0 .. 131)
// y = column address (0 .. 131)
// fill = 0=no fill, 1-fill entire rectangle
// color = 12-bit color value for lines rrrrggggbbbb
// rrrr = 1111 full red
// :
// 0000 red is off
//
// gggg = 1111 full green
// :
// 0000 green is off
//
// bbbb = 1111 full blue
// :
// 0000 blue is off
//
// Returns: nothing
//
// Notes:
//
// The best way to fill a rectangle is to take advantage of the "wrap-around" featute
// built into the Epson S1D15G00 controller. By defining a drawing box, the memory can
// be simply filled by successive memory writes until all pixels have been illuminated.
//
// 1. Given the coordinates of two opposing corners (x0, y0) (x1, y1)
// calculate the minimums and maximums of the coordinates
//
// xmin = (x0 <= x1) ? x0 : x1;
// xmax = (x0 > x1) ? x0 : x1;
// ymin = (y0 <= y1) ? y0 : y1;
// ymax = (y0 > y1) ? y0 : y1;
//
// 2. Now set up the drawing box to be the desired rectangle
//
// WriteSpiCommand(PASET); // set the row boundaries
// WriteSpiData(xmin);
// WriteSpiData(xmax);
// WriteSpiCommand(CASET); // set the column boundaries
// WriteSpiData(ymin);
// WriteSpiData(ymax);
//
// 3. Calculate the number of pixels to be written divided by 2
//
// NumPixels = ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1)
//
// You may notice that I added one pixel to the formula.
// This covers the case where the number of pixels is odd and we
// would lose one pixel due to rounding error. In the case of
// odd pixels, the number of pixels is exact.
// in the case of even pixels, we have one more pixel than
// needed, but it cannot be displayed because it is outside
// the drawing box.
//
// We divide by 2 because two pixels are represented by three bytes.
// So we work through the rectangle two pixels at a time.
//
// 4. Now a simple memory write loop will fill the rectangle
//
// for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 1); i++) {
// WriteSpiData((color >> 4) & 0xFF);
// WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
// WriteSpiData(color & 0xFF);
// }
//
//
// In the case of an unfilled rectangle, drawing four lines with the Bresenham line
// drawing algorithm is reasonably efficient.
//
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************************
void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color) 
{
  int xmin, xmax, ymin, ymax;
  int i;

  // check if the rectangle is to be filled
  if (fill == FILL) 
  {
    // best way to create a filled rectangle is to define a drawing box
    // and loop two pixels at a time
    // calculate the min and max for x and y directions
    xmin = (x0 <= x1) ? x0 : x1;
    xmax = (x0 > x1) ? x0 : x1;
    ymin = (y0 <= y1) ? y0 : y1;
    ymax = (y0 > y1) ? y0 : y1;

    // specify the controller drawing box according to those limits
    // Row address set (command 0x2B)
    WriteSpiCommand(PASET);
    WriteSpiData(xmin);
    WriteSpiData(xmax);

    // Column address set (command 0x2A)
    WriteSpiCommand(CASET);
    WriteSpiData(ymin);
    WriteSpiData(ymax);

    // WRITE MEMORY
    WriteSpiCommand(RAMWR);

    // loop on total number of pixels / 2
    for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 130); i++) 
	{
      // use the color value to output three data bytes covering two pixels
      WriteSpiData((color >> 4) & 0xFF);
      WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF));
      WriteSpiData(color & 0xFF);
    }
  } 
  else 
  {
    // best way to draw un unfilled rectangle is to draw four lines
    LCDSetLine(x0, y0, x1, y0, color);
    LCDSetLine(x0, y1, x1, y1, color);
    LCDSetLine(x0, y0, x0, y1, color);
    LCDSetLine(x1, y0, x1, y1, color);
  }
}

// *************************************************************************************
// LCDSetCircle.c
//
// Draws a line in the specified color at center (x0,y0) with radius
//
// Inputs: x0 = row address (0 .. 131)
// y0 = column address (0 .. 131)
// radius = radius in pixels
// color = 12-bit color value rrrrggggbbbb
//
// Returns: nothing
//
// Author: Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
//
// Note: taken verbatim Wikipedia article on Bresenham's line algorithm
// http://www.wikipedia.org
//
// *************************************************************************************
void LCDSetCircle(int x0, int y0, int radius, int color) 
{
  int f = 1 - radius;
  int ddF_x = 0;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
  LCDSetPixel(x0, y0 + radius, color);
  LCDSetPixel(x0, y0 - radius, color);
  LCDSetPixel(x0 + radius, y0, color);
  LCDSetPixel(x0 - radius, y0, color);
  while(x < y) 
  {
    if(f >= 0) 
	{
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x + 1;
    LCDSetPixel(x0 + x, y0 + y, color);
    LCDSetPixel(x0 - x, y0 + y, color);
    LCDSetPixel(x0 + x, y0 - y, color);
    LCDSetPixel(x0 - x, y0 - y, color);
    LCDSetPixel(x0 + y, y0 + x, color);
    LCDSetPixel(x0 - y, y0 + x, color);
    LCDSetPixel(x0 + y, y0 - x, color);
    LCDSetPixel(x0 - y, y0 - x, color);
  }
}

// *****************************************************************************
// LCDPutChar.c
//
// Draws an ASCII character at the specified (x,y) address and color
//
// Inputs: c = character to be displayed
// x = row address (0 .. 131)
// y = column address (0 .. 131)
// size = font pitch (SMALL, MEDIUM, LARGE)
// fcolor = 12-bit foreground color value rrrrggggbbbb
// bcolor = 12-bit background color value rrrrggggbbbb
//
// Returns: nothing
//
// Notes: Here's an example to display "E" at address (20,20)
//
// LCDPutChar('E', 20, 20, MEDIUM, WHITE, BLACK);
//
// (27,20) (27,27)
// | |
// | |
// ^ V V
// : _ # # # # # # # 0x7F
// : _ _ # # _ _ _ # 0x31
// : _ _ # # _ # _ _ 0x34
// x _ _ # # # # _ _ 0x3C
// : _ _ # # _ # _ _ 0x34
// : _ _ # # _ _ _ # 0x31
// : _ # # # # # # # 0x7F
// : _ _ _ _ _ _ _ _ 0x00
// ^ ^
// | |
// | |
// (20,20) (20,27)
//
// ------y----------->
//
//
// The most efficient way to display a character is to make use of the "wrap-around" feature
// of the Epson S1D16G00 LCD controller chip.
//
// Assume that we position the character at (20, 20) that's a (row, col) specification.
// With the row and column address set commands, you can specify an 8x8 box for the SMALL and MEDIUM
// characters or a 16x8 box for the LARGE characters.
//
// WriteSpiCommand(PASET); // set the row drawing limits
// WriteSpiData(20); //
// WriteSpiData(27); // limit rows to (20, 27)
//
// WriteSpiCommand(CASET); // set the column drawing limits
// WriteSpiData(20); //
// WriteSpiData(27); // limit columns to (20,27)
//
// When the algorithm completes col 27, the column address wraps back to 20
// At the same time, the row address increases by one (this is done by the controller)
//
// We walk through each row, two pixels at a time. The purpose is to create three
// data bytes representing these two pixels in the following format
//
// Data for pixel 0: RRRRGGGGBBBB
// Data for Pixel 1: RRRRGGGGBBBB
//
// WriteSpiCommand(RAMWR); // start a memory write (96 data bytes to follow)
//
// WriteSpiData(RRRRGGGG); // first pixel, red and green data
// WriteSpiData(BBBBRRRR); // first pixel, blue data; second pixel, red data
// WriteSpiData(GGGGBBBB); // second pixel, green and blue data
// :
// and so on until all pixels displayed!
// :
// WriteSpiCommand(NOP); // this will terminate the RAMWR command
//
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************
void LCDPutChar(char c, int x, int y, int size, int fColor, int bColor) 
{
  extern const unsigned char FONT6x8[97][8];
  extern const unsigned char FONT8x8[97][8];
  extern const unsigned char FONT8x16[97][16];
  int i,j;
  unsigned int nCols;
  unsigned int nRows;
  unsigned int nBytes;
  unsigned char PixelRow;
  unsigned char Mask;
  unsigned int Word0;
  unsigned int Word1;
  unsigned char *pFont;
  unsigned char *pChar;
  unsigned char *FontTable[] = {(unsigned char *)FONT6x8, (unsigned char *)FONT8x8,
                                (unsigned char *)FONT8x16};
  // get pointer to the beginning of the selected font table
  pFont = (unsigned char *)FontTable[size];
  // get the nColumns, nRows and nBytes
  nCols = *pFont;
  nRows = *(pFont + 1);
  nBytes = *(pFont + 2);

  // get pointer to the last byte of the desired character
  pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1;

  // Row address set (command 0x2B)
  WriteSpiCommand(PASET);
  WriteSpiData(x);
  WriteSpiData(x + nRows - 1);

  // Column address set (command 0x2A)
  WriteSpiCommand(CASET);
  WriteSpiData(y);
  WriteSpiData(y + nCols - 1);

  // WRITE MEMORY
  WriteSpiCommand(RAMWR);

  // loop on each row, working backwards from the bottom to the top
  for (i = nRows - 1; i >= 0; i--) 
  {
    // copy pixel row from font table and then decrement row
    PixelRow = *pChar--;

    // loop on each pixel in the row (left to right)
    // Note: we do two pixels each loop
    Mask = 0x80;
    for (j = 0; j < nCols; j += 2) 
	{
      // if pixel bit set, use foreground color; else use the background color
      // now get the pixel color for two successive pixels
      if ((PixelRow & Mask) == 0)
        Word0 = bColor;
      else
        Word0 = fColor;
      Mask = Mask >> 1;

      if ((PixelRow & Mask) == 0)
        Word1 = bColor;
      else
        Word1 = fColor;
      Mask = Mask >> 1;

      // use this information to output three data bytes
      WriteSpiData((Word0 >> 4) & 0xFF);
      WriteSpiData(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
      WriteSpiData(Word1 & 0xFF);
    }
  }

  // terminate the Write Memory command
  WriteSpiCommand(NOP);
}

// *************************************************************************************************
// LCDPutStr.c
//
// Draws a null-terminates character string at the specified (x,y) address, size and color
//
// Inputs: pString = pointer to character string to be displayed
// x = row address (0 .. 131)
// y = column address (0 .. 131)
// Size = font pitch (SMALL, MEDIUM, LARGE)
// fColor = 12-bit foreground color value rrrrggggbbbb
// bColor = 12-bit background color value rrrrggggbbbb
//
//
// Returns: nothing
//
// Notes: Here's an example to display "Hello World!" at address (20,20)
//
// LCDPutChar("Hello World!", 20, 20, LARGE, WHITE, BLACK);
//
//
// Author: James P Lynch August 30, 2007
// *************************************************************************************************
void LCDPutStr(char *pString, int x, int y, int Size, int fColor, int bColor) 
{
  // loop until null-terminator is seen
  while (*pString != 0x00) 
  {
    // draw the character
    LCDPutChar(*pString++, x, y, Size, fColor, bColor);

    // advance the y position
    if (Size == SMALL)
      y = y + 6;
    else if (Size == MEDIUM)
      y = y + 8;
    else
      y = y + 8;

    // bail out if y exceeds 131
    if (y > 131) break;
  }
}



