/*
  * MOD-LED8x8RGB Arduino Drawing Library
  * color - drawing color
  * drawPixel(x,y) draws drawPixel at X,Y coordinates (1,1) is bottom left corner
  * drawLine(x1,y1,x2,y2) draws line
  * drawRectangle(x1,y1,x2,y2) draws rectangle
  * drawSolidRectangle(x1,y1,x2,y2) draws solid rectangle
  * drawElipse(x,y,rx,ry) draws ellipse
  * drawCircle(x,y,r) draws circle
  * drawTriangle(x1,y1,x2,y2,x3,y3) draws triangle
  * drawChar(c)  - draws char at cX, cY and updates cX, cY
  * drawString(s) - draws string at cX, cY and updates cX, cY
  * lScroll() - scrolls all screen left 1 column
  * rScroll() - scrolls all screen right 1 column
  * scrollCharLeft(c) - scrolls one character from the upper right matrix to the left
  * scrollCharRight(c) - scrolls one character from the bottom left matrix to the right
  * scrollString(c, dir) - scrolls string left or right
  *
*/

#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "gpio_lib.h"
#include "LED8x8RGB.h"
#include "font.h"


static uint8_t spiMode = 1;
static uint8_t spiBPW = 8;
static uint32_t spiSpeed = 20000;	// max speed 23435
static uint16_t spiDelay = 200;


LCD8x8RGB::LCD8x8RGB ()
{
	color = 1; // Starting color of LEDs
	sdelay = 100000;	// microseconds
	cX = 1;
	cY = 1;

	int ret;

	sunxi_gpio_init();

	CS_MAKE_OUT ();
	CS_HIGH ();
	FD = open (DEVICE, O_RDWR);
	if (FD < 0)
	{
		printf ("SPI init error!\n)");
		return;
	}
	else
		printf ("SPI init successful!\n");

	ret = ioctl(FD, SPI_IOC_WR_MODE, &spiMode);
	if (ret == -1)
	{
		perror("can't set spi mode");
		exit(1);
	}

	ret = ioctl(FD, SPI_IOC_RD_MODE, &spiMode);
	if (ret == -1)
	{
		perror("can't get spi mode");
		exit(1);
	}

	ret = ioctl(FD, SPI_IOC_WR_BITS_PER_WORD, &spiBPW);
	if (ret == -1)
	{
		perror("can't set bits per word");
		exit(1);
	}

	ret = ioctl(FD, SPI_IOC_RD_BITS_PER_WORD, &spiBPW);
	if (ret == -1)
	{
		perror("can't get bits per word");
		exit(1);
	}

	ret = ioctl(FD, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed);
	if (ret == -1)
	{
		perror("can't set max speed hz");
		exit(1);
	}

	ret = ioctl(FD, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed);
	if (ret == -1)
	{
		perror("can't get max speed hz");
		exit(1);
	}

	printf("spi mode: %d\n", spiMode);
	printf("bits per word: %d\n", spiBPW);
	printf("speed: %d Hz (%d KHz)\n", spiSpeed, spiSpeed/1000);
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::Transfer ()	//transfer the video buffer to MOD-LED8x8RGB
{
	int i;
	CS_LOW();
	
	int ret;

	struct spi_ioc_transfer spi;
	memset(&spi, 0, sizeof (spi));

	for (i=0; i<NumberX; i++)
	{
		spi.tx_buf = (unsigned long) videobuf + i*24;
		spi.rx_buf = 0;
		spi.len = 24;
		spi.delay_usecs = spiDelay;
		spi.speed_hz = spiSpeed;
		spi.bits_per_word = spiBPW;
	
		// Send message
		ret = ioctl (FD, SPI_IOC_MESSAGE(1), &spi) ;
		if (ret < 0)
		{
			perror("Can't send spi message!\n");
		}
	}
	
	CS_HIGH();
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::vClear ()	//clear the video buffer
{
	for(int i=0; i<NumberX*24; i++)
		videobuf[i]=0;
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::setColor (unsigned char NewColor)
{
	color = NewColor;
	return;
}

//----------------------------------------------------------------------------------------------	
void LCD8x8RGB::drawPixel (unsigned int X, unsigned int Y)	//draw drawPixel at x,y coordinates to MOD-LED8x8RGB 1,1 is bottom left corner
{
 	int p;
	if (Y<=bigY*8 && X<=bigX*8 && X>0 && Y>0)
	{
		if (Y>8)
			//X=(X+bigX*8)*((Y-1)/8);
			X=X+(bigX*8)*((Y-1)/8);
		Y=Y%8;
		if (Y==0)
			Y=8;

		p=NumberX-((X-1)/8)-1;

		videobuf[3*(Y-1)+24*p]&=~(1<<((X-1)%8));		//turn off chosen drawPixel
		videobuf[3*(Y-1)+1+24*p]&=~(1<<((X-1)%8));
		videobuf[3*(Y-1)+2+24*p]&=~(1<<((X-1)%8));

		if (color&1)
			videobuf[3*(Y-1)+24*p]|=(1<<((X-1)%8));		//set color to the drawPixel
		if (color&2)
			videobuf[3*(Y-1)+1+24*p]|=(1<<((X-1)%8));
		if (color&4)
			videobuf[3*(Y-1)+2+24*p]|=(1<<((X-1)%8));
	}
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::drawLine (int x1, int y1, int x2, int y2)		//draw a line from x1,y1 to x2,y2
{
	int dx, dy, sx, sy, err, e2;

	dx = abs (x2-x1);
	dy = abs (y2-y1);
	if (x1<x2) sx = 1;
		else sx = -1;
	if (y1<y2) sy = 1;
		else sy = -1;
	err = dx-dy;
	do
	{
		drawPixel (x1, y1);
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

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::drawRectangle (int x1, int y1, int x2, int y2)		//draw a rectangle from x1,y1 to x2,y2
{
	drawLine (x1, y1, x1, y2);
	drawLine (x1, y1, x2, y1);
	drawLine (x2, y1, x2, y2);
	drawLine (x1, y2, x2, y2);
	return;
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::drawSolidRectangle (int x1, int y1, int x2, int y2)	//draw a solid rectangle
{
	if (x2>x1)
		for (int i=x1; i<=x2;i++)
			drawLine (i, y1, i, y2);
	else
		for (int i=x2; i<=x1;i++)
			drawLine (i, y1, i, y2);
	return;
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::Draw_4_Ellipse_Points (int CX, int CY, int X, int Y)	//function needed for drawing an ellipse
{
	drawPixel (CX+X, CY+Y);
	drawPixel (CX-X, CY+Y);
	drawPixel (CX-X, CY-Y);
	drawPixel (CX+X, CY-Y);
	return;
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::drawEllipse (int CX, int CY, int XRadius, int YRadius)	//draw an ellipse & fix radius if negative
{
	int X, Y, XChange, YChange, EllipseError, TwoASquare, TwoBSquare, StoppingX, StoppingY;
	if (XRadius<0)
		XRadius=-XRadius;
	if (YRadius<0)
		YRadius=-YRadius;

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

	X = 0;
	Y = YRadius;
	XChange = YRadius*YRadius;
	YChange = XRadius*XRadius * (1-2*YRadius);
	EllipseError = 0;
	StoppingX = 0;
	StoppingY = TwoASquare * YRadius;

	while (StoppingX <= StoppingY)		// 2nd set of points, y'< -1
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

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::drawCircle (int x, int y, int r)	//draw a circle
{
	drawEllipse (x, y, r, r);
	return;
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3)	//draw a triangle
{
	drawLine (x1, y1, x2, y2);
	drawLine (x2, y2, x3, y3);
	drawLine (x3, y3, x1, y1);
	return;
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::drawChar(unsigned char c)		//draw static character  if within the Font limit
{
	unsigned char b,i;
	signed char k;
	if (c<32 || c>125)
		c=32;

	for(k=0;k<5;k++)
	{
		b = FontLookup[c-32][k];
		for(i=0;i<8;i++)
			if (b & (1<<(7-i)))
				drawPixel(k+cX,i+cY);	// SPP since our 0, 0 coordinates are in the top left corner instead of bottom left, the letters should be mirrored horizontally ==> (1<<7) replaced with (7-i)
	}
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::drawString( unsigned char c[])		//draw static string
{
	for(int i=0; c[i];i++)
	{
		drawChar(c[i]);
		cX +=6;
	}
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::lScroll()		//scroll everything left
{
	for (int i=NumberX*24-1;i>=0;i--)
	{
		videobuf[i]=videobuf[i]>>1;
		if (i>=24)
			videobuf[i]|=((videobuf[i-24]&1)<<7);
	}
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::rScroll()		//scroll everything right
{
	for (int i=0;i<NumberX*24;i++)
	{
		videobuf[i]=videobuf[i]<<1;
		if (i<(NumberX-1)*24)
			videobuf[i]|=(videobuf[i+24]>>7);
	}
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::scrollCharLeft(unsigned char c)		//scroll one character left if within the Font limit
{
	unsigned char b,i,k;
	if (c<32 || c>125)
		c=32;

	for(k=0;k<5;k++)
	{
		b = FontLookup[c-32][k];
		for(i=0;i<8;i++)
			if (b & (1<<(7-i)))
				drawPixel(bigX*8,i+1+(bigY-1)*8);	// SPP since our 0, 0 coordinates are in the top left corner instead of bottom left, the letters should be mirrored horizontally ==> (1<<7) replaced with (7-i)
		Transfer();
		lScroll();
		usleep(sdelay);
	}
	Transfer();
	lScroll();
	usleep(sdelay);
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::scrollCharRight(unsigned char c)		//scroll one character right if within the Font limit
{
	unsigned char b,i;
	signed char k;	// SPP - k must be able to take negative value so it should be signed char

	if (c<32 || c>125)
		c=32;

	rScroll();
	for(k=4;k>=0;k--)
	{
		b = FontLookup[c-32][k];
		for(i=0;i<8;i++)
			if (b & (1<<(7-i)))
				drawPixel(1,i+1);	// SPP since our 0, 0 coordinates are in the top left corner instead of bottom left, the letters should be mirrored horizontally ==> (1<<7) replaced with (7-i)
		Transfer();
		rScroll();
		usleep(sdelay);
	}
	Transfer();
	usleep(sdelay);
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::theEnder(Direction Dir)	//move everything left/right until it leaves the screen
{
	for (int i=0;i<NumberX*8-1;i++)
	{
		if (Dir == Right)
			rScroll();
		else
			lScroll();
		Transfer();
		usleep(sdelay);
	}
}

//----------------------------------------------------------------------------------------------
void LCD8x8RGB::scrollString(unsigned char c[], Direction Dir)	//draw a scrolling string
{
	int len;
	for(len=0;c[len];len++);
		if (Dir == Right)
		{
			for(int i=len-1; i>=0;i--)
			{
				scrollCharRight(c[i]);
				color++;
				if (color>7)
					color = 1;
			}
		}
		else
		{
			for(int i=0;c[i];i++)
			{
				scrollCharLeft(c[i]);
				color++;
				if (color>7)
					color = 1;
			}
		}
	theEnder(Dir);
}
