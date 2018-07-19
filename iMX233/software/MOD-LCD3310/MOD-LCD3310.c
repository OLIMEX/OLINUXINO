#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "gpio-mmap.h"
#include "lcd3310.h"


//MOSI -> bank[0], pin[20]
//MISO -> bank[0], pin[0]
//SCK -> bank[0], pin[24]
//cs - bank[0], pin[3]
//res - bank[1], pin[23]
//dc  - bank[1], ping[24]



#define LCD_CS_MAKE_OUT()		gpio_output(0, 3)
#define LCD_CS_HIGH()  			GPIO_WRITE(0, 3, 1)
#define LCD_CS_LOW()  			GPIO_WRITE(0, 3, 0)

#define LCD_RES_MAKE_OUT()		gpio_output(0, 23); 
#define LCD_RES_HIGH()			GPIO_WRITE(0, 23, 1);
#define LCD_RES_LOW()			GPIO_WRITE(0, 23, 0);

#define LCD_DC_MAKE_OUT()		gpio_output(0, 25);
#define LCD_DC_HIGH()			GPIO_WRITE(0, 25, 1);
#define LCD_DC_LOW()			GPIO_WRITE(0, 25, 0);

#define SEND_BYTE_SPI() 					\
	int i;									\
	{										\
		for(i = 0; i < 8; i++) {			\
			if(data & 0x80) {				\
				GPIO_WRITE(0, 20, 1);		\
			} else { 						\
				GPIO_WRITE(0, 20, 0);		\
			}								\
			nanosleep(1000);				\
			GPIO_WRITE(0, 24, 0);			\
			nanosleep(1000);				\
			GPIO_WRITE(0, 24, 1);			\
											\
			data <<= 1;						\
		}									\
	}

void Draw_4_Ellipse_Points (int CX, int CY, int X, int Y);
void Draw_Point (int x, int y);
void Draw_Ellipse (int CX, int CY, int XRadius, int YRadius);
void Draw_Circle (int x, int y, int r);
void Draw_Rectangle (int x1, int y1, int x2, int y2);
void Draw_Triangle (int x1, int y1, int x2, int y2, int x3, int y3);
void Draw_Line (int x1, int y1, int x2, int y2);
static void Initialize_SPI(void)
{
	gpio_map();
	// Initialie SPI Interface pins as GPIOs
	gpio_output(0, 20);
	gpio_output(0, 24);
	
	GPIO_WRITE(0, 24, 1);
	GPIO_WRITE(0, 20, 1);
}

/* END OF SECTION */


#define LCD_START_LINE_ADDR	(66-2)

#if LCD_START_LINE_ADDR	> 66
  #error "Invalid LCD starting line address"
#endif

// LCD memory index
unsigned int  LcdMemIdx;

// represent LCD matrix
unsigned char  LcdMemory[LCD_CACHE_SIZE];
unsigned char Temp_Buff[LCD_X_RES][LCD_Y_RES];

const unsigned char  FontLookup [][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0xc4, 0xc8, 0x10, 0x26, 0x46 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x08, 0x6C, 0x6A, 0x19, 0x08 },   // { (gramotevichka)
    { 0x0C, 0x12, 0x24, 0x12, 0x0C },   // | (sarce)
    { 0x7E, 0x7E, 0x7E, 0x7E, 0x7E },    // kvadratche
};


void LCDInit(void)
{  	
	// Initialie SPI Interface
    Initialize_SPI();
	
	// set pin directions
	LCD_CS_MAKE_OUT();
	LCD_CS_HIGH();
	LCD_DC_MAKE_OUT();
	LCD_RES_MAKE_OUT();
	
	// Toggle reset pin
	LCD_RES_LOW();
	usleep(1000);
	LCD_RES_HIGH();		
	usleep(1000);	
	
	// Send sequence of command
	LCDSend( 0x21, SEND_CMD );  // LCD Extended Commands.
	LCDSend( 0xC8, SEND_CMD );  // Set LCD Vop (Contrast). 0xC8
	LCDSend( 0x04 | !!(LCD_START_LINE_ADDR&(1u<<6)), SEND_CMD );  // Set Temp S6 for start line
	LCDSend( 0x40 | (LCD_START_LINE_ADDR & ((1u<<6)-1)), SEND_CMD );  // Set Temp S[5:0] for start line
	//LCDSend( 0x13, SEND_CMD );  // LCD bias mode 1:48.
	LCDSend( 0x12, SEND_CMD );  // LCD bias mode 1:68.
	LCDSend( 0x20, SEND_CMD );  // LCD Standard Commands, Horizontal addressing mode.
	//LCDSend( 0x22, SEND_CMD );  // LCD Standard Commands, Vertical addressing mode.
	LCDSend( 0x08, SEND_CMD );  // LCD blank
	LCDSend( 0x0C, SEND_CMD );  // LCD in normal mode.
	
	// Clear and Update
	LCDClear();
	LCDUpdate();
}

void LCDReset(void)
{

	// Close SPI module - optional
	// NOT DONE
	
	LCD_RES_LOW();
}

void LCDUpdate ( void )
{
	int x,y;
	
	for(y=0; y<48 / 8; y++) {
		LCDSend(0x80, SEND_CMD );
		LCDSend(0x40 | y, SEND_CMD );	
		for(x=0; x < 84; x++) {
			LCDSend( LcdMemory[y * 84 + x], SEND_CHR );
		}	
	}
}

void LCDSend(unsigned char data, unsigned char cd)
{
	LCD_CS_LOW();

	if(cd == SEND_CHR) {
    	LCD_DC_HIGH();
  	}
  	else {
    	LCD_DC_LOW();
  	}

	// send data over SPI
	SEND_BYTE_SPI();
		
	LCD_CS_HIGH();
}

void LCDClear(void)
{
	int i;
	
	// loop all cashe array
	for (i=0; i<LCD_CACHE_SIZE; i++) {
		LcdMemory[i] = 0x00;
	}
}

void LCDChrXY (unsigned char x, unsigned char y, unsigned char ch )
{
    unsigned int    index   = 0;
    unsigned int    offset  = 0;
    unsigned int    i       = 0;

    // check for out off range
    if ( x > LCD_X_RES ) return;
    if ( y > LCD_Y_RES ) return;

	index=(unsigned int)x*6+(unsigned int)y*84;

    for ( i = 0; i < 6; i++ )
    {
		if(i == 5)
		{
			LcdMemory[index] = 0x00;
			break;
		}
	  offset = FontLookup[ch - 32][i];
	  LcdMemory[index] = offset;
      index++;
    }

}

void LCDChrXYInverse (unsigned char x, unsigned char y, unsigned char ch )
{
	unsigned int    index   = 0;
    unsigned int    i       = 0;

    // check for out off range
    if ( x > LCD_X_RES ) return;
    if ( y > LCD_Y_RES ) return;

	index=(unsigned int)x*5+(unsigned int)y*84;

    for ( i = 0; i < 5; i++ )
    {
      LcdMemory[index] = ~(FontLookup[ch - 32][i]);
      index++;
    }

}

void LCDContrast(unsigned char contrast) {

    //  LCD Extended Commands.
    LCDSend( 0x21, SEND_CMD );

    // Set LCD Vop (Contrast).
    LCDSend( 0x80 | contrast, SEND_CMD );

    //  LCD Standard Commands, horizontal addressing mode.
    LCDSend( 0x20, SEND_CMD );
}

void Draw_Point (int x, int y)
{
	int i, row;
	if (x >= 0 && x < LCD_X_RES && y >= 0 && y < LCD_Y_RES)
{
		
		row = y / 8; 
		i = x + row * 84;
		LcdMemory[i] |= 1 << (y % 8);
	return;
}
}
// Using Bresenham's line algorithm:
// Algorithm description and Pseudo Code here:
// http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void Draw_Line (int x1, int y1, int x2, int y2)
{
	int dx, dy, sx, sy, err, e2;
	dx = abs (x2-x1);
	dy = abs (y2-y1);
	if (x1<x2)
		sx = 1;
	else
		sx = -1;
	if (y1<y2)
		sy = 1;
	else
		sy = -1;
	err = dx-dy;
	do
	{
		Draw_Point (x1, y1);
		if ((x1 == x2) && (y1 == y2))
			break;
		e2 = 2*err;
		if (e2 > -dy)
		{
			err = err - dy;
			x1 = x1+sx;
		}
		if (e2 < dx)
		{
			err = err + dx;
			y1 = y1 + sy;
		}
	}
	while (1);

	return;
}

void Draw_Triangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
	Draw_Line (x1, y1, x2, y2);
	Draw_Line (x2, y2, x3, y3);
	Draw_Line (x3, y3, x1, x1);
	return;
}

void Draw_Rectangle (int x1, int y1, int x2, int y2)
{
	Draw_Line (x1, y1, x1, y2);
	Draw_Line (x1, y1, x2, y1);
	Draw_Line (x2, y1, x2, y2);
	Draw_Line (x1, y2, x2, y2);
	return;
}

// Using Bresenham's ( You're the MAN! :) ) ellipse algorithm (It's AWESOME!)
// Pascal realization here:
// http://homepage.smc.edu/kennedy_john/belipse.pdf
void Draw_Ellipse (int CX, int CY, int XRadius, int YRadius)
{
	int X, Y, XChange, YChange, EllipseError, TwoASquare, TwoBSquare, StoppingX, StoppingY;
	TwoASquare = 2 * XRadius*XRadius;
	TwoBSquare = 2 * YRadius*YRadius;
	X = XRadius;
	Y = 0;
	XChange = YRadius*YRadius * (1-2*XRadius);
	YChange = XRadius*XRadius;
	EllipseError = 0;
	StoppingX = TwoBSquare*XRadius;
	StoppingY = 0;

	while (StoppingX >= StoppingY)	// 1st set of points, y'> -1
	{
		Draw_4_Ellipse_Points (CX, CY, X, Y);
		Y++;
		StoppingY = StoppingY + TwoASquare;
		EllipseError = EllipseError + YChange;
		YChange = YChange + TwoASquare;
		if ((2*EllipseError + XChange) > 0)
		{
			X--;
			StoppingX = StoppingX - TwoBSquare;
			EllipseError = EllipseError + XChange;
			XChange = XChange + TwoBSquare;
		}
	}

	// 1st point set is done; start the 2nd set of points

	X = 0;
	Y = YRadius;
	XChange = YRadius*YRadius;
	YChange = XRadius*XRadius * (1-2*YRadius);
	EllipseError = 0;
	StoppingX = 0;
	StoppingY = TwoASquare * YRadius;

	while (StoppingX <= StoppingY)	// 2nd set of points, y'< -1
	{
		Draw_4_Ellipse_Points (CX, CY, X, Y);
		X++;
		StoppingX = StoppingX + TwoBSquare;
		EllipseError = EllipseError + XChange;
		XChange = XChange + TwoBSquare;
		if ((2*EllipseError + YChange) > 0)
		{
			Y--;
			StoppingY = StoppingY - TwoASquare;
			EllipseError = EllipseError + YChange;
			YChange = YChange + TwoASquare;
		}	
	}

	return;
}
void Draw_4_Ellipse_Points (int CX, int CY, int X, int Y)
{
	Draw_Point (CX+X, CY+Y);
	Draw_Point (CX-X, CY+Y);
	Draw_Point (CX-X, CY-Y);
	Draw_Point (CX+X, CY-Y);
	return;
}
void Draw_Circle (int x, int y, int r)
{
	Draw_Ellipse (x, y, r, r);
	return;
}

void LCDStr(unsigned char row, const unsigned char *dataPtr, unsigned char inv ) {

	// variable for X coordinate
	unsigned char x = 0;
	
	// loop to the and of string
	while ( *dataPtr ) {
		if(inv) {
			LCDChrXYInverse(x, row, *dataPtr);
		} else {
			LCDChrXY( x, row, *dataPtr);
		}	
		x++;
		dataPtr++;
	}
	LCDUpdate();
}



int main(int argc, char **argv)
{
	Initialize_SPI();
	LCDInit();
	LCDContrast(0xFF);
	LCDClear();

	
	
	

	if(argc < 2)
	{
		printf("Too few arguments!\n");
		exit(1);
	}
	if(!strcmp(argv[1], "-animate"))
	{
		int i,j = 20;
		do
		{
			for(i = 12; i ; i--)
			{
				LCDClear();
				Draw_Rectangle(i,i,83-i,47-i);
				Draw_Rectangle(12+i,12+i,83-i-12,47-i-12);
				LCDUpdate();
				usleep(50000);
			}
			j--;
		}
		while(j);
		LCDClear();	
		LCDUpdate();	
	}
	
	else if(!strcmp(argv[1], "-clear"))
	{
		if(argc != 2)
		{
			printf("Invalid number of arguments!\n");
			exit(1);
		}
		
		printf("Clear...");
		LCDClear();		
		printf("Done\n");
	}

	else if(!strcmp(argv[1], "-text"))
	{
		if(argc != 4)
		{
			printf("Invalid number of arguments!\n");
			exit(1);
		}
		unsigned char row = (unsigned char)strtol(argv[3], NULL, 0);
		if(row < 0 || row > 5)
		{
			printf("Row must be between 0 and 5!\n");
			exit(1);
		}
		printf("Printing...");
		LCDStr(row, argv[2], 0);
		printf("Done\n");
	}
	else if(!strcmp(argv[1], "-point"))
	{
		if(argc < 5)
		{
			printf("Too few arguments!\n");
			exit(1);
		}
		
		unsigned char n;
		n = (unsigned char)strtol(argv[2], NULL, 0);

		if(argc != 3 + n*2)
		{
			printf("Invalid number of arguments [%d] - should be [%d]!\n", (int)argc, (int)n);
			exit(1);
		}
		
		unsigned char x, y;
		int i;
	
		for(i = 0 ; i < n; i++)
		{
			x = (unsigned char)strtol(argv[3 + i*2], NULL, 0);
			y = (unsigned char)strtol(argv[4 + i*2], NULL, 0);		
			Draw_Point(x, y);
		}
		printf("Printing...");
		LCDUpdate();
		printf("Done\n");
	}
	else if(!strcmp(argv[1], "-line"))
	{
		if(argc != 6)
		{
			printf("Invalid number of arguments [%d] - should be [%d]!\n", (int)argc, 6);
			exit(1);
		}
		
		unsigned char x1,y1,x2,y2;
		x1 = (unsigned char)strtol(argv[2], NULL, 0);
		y1 = (unsigned char)strtol(argv[3], NULL, 0);
		x2 = (unsigned char)strtol(argv[4], NULL, 0);
		y2 = (unsigned char)strtol(argv[5], NULL, 0);

		Draw_Line(x1, y1, x2, y2);
		
		printf("Printing...");
		LCDUpdate();
		printf("Done\n");
		
	}
	else if(!strcmp(argv[1], "-rectangle"))
	{
		if(argc != 6)
		{
			printf("Invalid number of arguments [%d] - should be [%d]!\n", (int)argc, 6);
			exit(1);
		}
		
		unsigned char x1,y1,x2,y2;
		x1 = (unsigned char)strtol(argv[2], NULL, 0);
		y1 = (unsigned char)strtol(argv[3], NULL, 0);
		x2 = (unsigned char)strtol(argv[4], NULL, 0);
		y2 = (unsigned char)strtol(argv[5], NULL, 0);

		Draw_Rectangle(x1, y1, x2, y2);
		
		printf("Printing...");
		LCDUpdate();
		printf("Done\n");
		
	}
	else if(!strcmp(argv[1], "-triangle"))
	{
		if(argc != 8)
		{
			printf("Invalid number of arguments [%d] - should be [%d]!\n", (int)argc, 8);
			exit(1);
		}
		
		unsigned char x1,y1,x2,y2,x3,y3;
		x1 = (unsigned char)strtol(argv[2], NULL, 0);
		y1 = (unsigned char)strtol(argv[3], NULL, 0);
		x2 = (unsigned char)strtol(argv[4], NULL, 0);
		y2 = (unsigned char)strtol(argv[5], NULL, 0);
		x3 = (unsigned char)strtol(argv[6], NULL, 0);
		y3 = (unsigned char)strtol(argv[7], NULL, 0);

		Draw_Triangle(x1, y1, x2, y2, x3, y3);
		
		printf("Printing...");
		LCDUpdate();
		printf("Done\n");
		
	}
	else if(!strcmp(argv[1], "-ellipse"))
	{
		if(argc != 6)
		{
			printf("Invalid number of arguments [%d] - should be [%d]!\n", (int)argc, 6);
			exit(1);
		}
		
		unsigned char cx,cy,xr,yr;
		cx = (unsigned char)strtol(argv[2], NULL, 0);
		cy = (unsigned char)strtol(argv[3], NULL, 0);
		xr = (unsigned char)strtol(argv[4], NULL, 0);
		yr = (unsigned char)strtol(argv[5], NULL, 0);

		Draw_Ellipse(cx, cy, xr, yr);
		
		printf("Printing...");
		LCDUpdate();
		printf("Done\n");
		
	}

	else if(!strcmp(argv[1], "-circle"))
	{
		if(argc != 5)
		{
			printf("Invalid number of arguments [%d] - should be [%d]!\n", (int)argc, 5);
			exit(1);
		}
		
		unsigned char x,y,r;
		x = (unsigned char)strtol(argv[2], NULL, 0);
		y = (unsigned char)strtol(argv[3], NULL, 0);
		r = (unsigned char)strtol(argv[4], NULL, 0);

		Draw_Circle(x,y,r);
		
		printf("Printing...");
		LCDUpdate();
		printf("Done\n");
		
	}
	else if(!strcmp(argv[1], "-help") || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--h"))
	{
		printf("USAGE: %s [param] [arguments]\n", argv[0]);
		printf("\n\t-clear - Clear the screen\n");

		printf("\n\t-text - Print text on the display\n");
		printf("\t\tExample: %s -text [text] [row]\n", argv[0]);

		printf("\n\t-point - Draw [n] points at [X;Y]\n");
		printf("\t\tExample: %s -point [n] [x1] [y1] [x2] [y2] ...\n", argv[0]);

		printf("\n\t-line - Draw line on the display\n");
		printf("\t\tExample: %s -line [x1] [y1] [x2] [y2]\n", argv[0]);

		printf("\n\t-rectangle - Draw rectangle on the display\n");
		printf("\t\tExample: %s -rect [x1] [y1] [x2] [y2]\n", argv[0]);

		printf("\n\t-triangle - Draw triangle on the display\n");
		printf("\t\tExample: %s -rect [x1] [y1] [x2] [y2] [x3] [y3]\n", argv[0]);

		printf("\n\t-ellipse - Draw ellipse on the display\n");
		printf("\t\tExample: %s -rect [CX] [CY] [xr] [yr]\n", argv[0]);

		printf("\n\t-circle - Draw circle on the display\n");
		printf("\t\tExample: %s -rect [x] [y] [r]\n", argv[0]);

		printf("\n\t-animate - Play simple animation\n");		
	}
	else
	{
		printf("Invalid argument!\nUse \"-help\" for usage information.\n");
	}

	return 0;

}
