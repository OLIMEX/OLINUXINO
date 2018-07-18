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

#ifndef	_LED8x8RGB
#define	_LED8x8RGB

#define bigX 1                  // Number of MOD-LED8x8RGB in columns
#define bigY 1                  // Number of MOD-LED8x8RGB in rows
#define NumberX bigX*bigY       // Total number of MOD-LED8x8RGBs connected together

// SPI selection +
#define	DEVICE		"/dev/spidev1.0"
//#define	DEVICE		"/dev/spidev2.0"

// CS - by default PortH.0
#define CS_MAKE_OUT()	sunxi_gpio_set_cfgpin(SUNXI_GPH(0), SUNXI_GPIO_OUTPUT)
#define CS_HIGH()	sunxi_gpio_output(SUNXI_GPH(0), 1)
#define CS_LOW()	sunxi_gpio_output(SUNXI_GPH(0), 0)
// SPI selection -

enum Direction
{
	Left = 0,
	Right
};

class LCD8x8RGB
{
private:
	unsigned int sdelay;
	unsigned char color, cX, cY;
	unsigned char videobuf[NumberX*24];   //video buffer
	int FD;

public:
	LCD8x8RGB ();
	void vClear();
	void Transfer ();
	void setColor (unsigned char NewColor);
	void drawPixel (unsigned int X, unsigned int Y);
	void drawLine (int x1, int y1, int x2, int y2);
	void drawRectangle (int x1, int y1, int x2, int y2);
	void drawSolidRectangle (int x1, int y1, int x2, int y2);
	void Draw_4_Ellipse_Points (int CX, int CY, int X, int Y);
	void drawEllipse (int CX, int CY, int XRadius, int YRadius);
	void drawCircle (int x, int y, int r);
	void drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3);
	void drawChar (unsigned char c);
	void drawString (unsigned char c[]);
	void lScroll();
	void rScroll();
	void scrollCharLeft (unsigned char c);
	void scrollCharRight(unsigned char c);
	void theEnder(Direction Dir);
	void scrollString(unsigned char c[], Direction Dir);
};

#endif
