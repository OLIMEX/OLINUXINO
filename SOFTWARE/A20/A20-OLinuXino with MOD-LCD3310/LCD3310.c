/*************************************************************************
 *
 *    Driver for the Nokia 3310 LCD
 *
 **************************************************************************/
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include "LCD3310.h"
#include "gpio_lib.h"

/* TO IMPLEMENT YOUR VERSION OF THE DRIVER YOU'LL HAVE TO EDIT THIS SECTION ONLY */

int spi_fd;
static const char *device = "/dev/spidev2.0";
static uint8_t spiMode = 3;
static uint8_t spiBPW = 8;
static uint32_t spiSpeed = 5000000;
static uint16_t spiDelay = 0;

// B20
#define LCD_RES_MAKE_OUT()      sunxi_gpio_set_cfgpin(SUNXI_GPB(20), SUNXI_GPIO_OUTPUT)
#define LCD_RES_HIGH()          sunxi_gpio_output(SUNXI_GPB(20), 1)
#define LCD_RES_LOW()           sunxi_gpio_output(SUNXI_GPB(20), 0)

// B21
#define LCD_CD_MAKE_OUT()       sunxi_gpio_set_cfgpin(SUNXI_GPB(21), SUNXI_GPIO_OUTPUT)
#define LCD_CD_HIGH()           sunxi_gpio_output(SUNXI_GPB(21), 1)
#define LCD_CD_LOW()            sunxi_gpio_output(SUNXI_GPB(21), 0)

static int SPI_Send(uint8_t *TxBuf, int iTxLen)
{
	int status;

	struct spi_ioc_transfer spi;
	memset( &spi, 0, sizeof spi);

	spi.tx_buf = (unsigned long) TxBuf;
	spi.rx_buf = 0;
	spi.len = iTxLen;
	spi.delay_usecs = spiDelay;
	spi.speed_hz = spiSpeed;
	spi.bits_per_word = spiBPW;

	// Send message
	status = ioctl (spi_fd, SPI_IOC_MESSAGE(1), &spi) ;
	if (status < 0)
	{
		perror("Can't send spi message!\n");
		return -1;
	}
	return status;
}

static void Initialize_SPI(void)
{
	int ret;
	spi_fd = open(device, O_RDWR);

	if (spi_fd < 0)
	{
		perror("can't open device");
		exit(1);
	}

	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &spiMode);
	if (ret == -1)
	{
		perror("can't set spi mode");
		exit(1);
	}

	ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &spiMode);
	if (ret == -1)
	{
		perror("can't get spi mode");
		exit(1);
	}

	ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW);
	if (ret == -1)
	{
		perror("can't set bits per word");
		exit(1);
	}

	ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &spiBPW);
	if (ret == -1)
	{
		perror("can't get bits per word");
		exit(1);
	}

	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed);
	if (ret == -1)
	{
		perror("can't set max speed hz");
		exit(1);
	}

	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed);
	if (ret == -1)
	{
		perror("can't get max speed hz");
		exit(1);
	}

	printf("spi mode: %d\n", spiMode);
	printf("bits per word: %d\n", spiBPW);
	printf("max speed: %d Hz (%d KHz)\n", spiSpeed, spiSpeed/1000);

	return;
}

/* END OF SECTION */


#define LCD_START_LINE_ADDR	(66-2)

#if LCD_START_LINE_ADDR	> 66
#error "Invalid LCD starting line address"
#endif

// LCD memory index
unsigned int LcdMemIdx;

// represent LCD matrix
unsigned char LcdMemory[LCD_CACHE_SIZE];

const unsigned char FontLookup [][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00}, // sp
    { 0x00, 0x00, 0x2f, 0x00, 0x00}, // !
    { 0x00, 0x07, 0x00, 0x07, 0x00}, // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14}, // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12}, // $
    { 0xc4, 0xc8, 0x10, 0x26, 0x46}, // %
    { 0x36, 0x49, 0x55, 0x22, 0x50}, // &
    { 0x00, 0x05, 0x03, 0x00, 0x00}, // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00}, // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00}, // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14}, // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    { 0x00, 0x00, 0x50, 0x30, 0x00}, // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10}, // -
    { 0x00, 0x60, 0x60, 0x00, 0x00}, // .
    { 0x20, 0x10, 0x08, 0x04, 0x02}, // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00}, // :
    { 0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00}, // <
    { 0x14, 0x14, 0x14, 0x14, 0x14}, // =
    { 0x00, 0x41, 0x22, 0x14, 0x08}, // >
    { 0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E}, // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    { 0x46, 0x49, 0x49, 0x49, 0x31}, // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    { 0x63, 0x14, 0x08, 0x14, 0x63}, // X
    { 0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00}, // [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55}, // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40}, // _
    { 0x00, 0x01, 0x02, 0x04, 0x00}, // '
    { 0x20, 0x54, 0x54, 0x54, 0x78}, // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    { 0x38, 0x44, 0x44, 0x44, 0x20}, // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    { 0x38, 0x54, 0x54, 0x54, 0x18}, // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E}, // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78}, // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00}, // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78}, // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78}, // n
    { 0x38, 0x44, 0x44, 0x44, 0x38}, // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08}, // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C}, // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    { 0x48, 0x54, 0x54, 0x54, 0x20}, // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20}, // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    { 0x44, 0x28, 0x10, 0x28, 0x44}, // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44}, // z
    { 0x08, 0x6C, 0x6A, 0x19, 0x08}, // { (lighting)
    { 0x0C, 0x12, 0x24, 0x12, 0x0C}, // | (heart)
    { 0x7E, 0x7E, 0x7E, 0x7E, 0x7E}, // square
};



/****************************************************************************/
/*  Initialize LCD Controller                                               */
/*  Function : LCDInit                                                      */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDInit(void)
{
	// A20 
	sunxi_gpio_init();
	// Initialie SPI Interface
	Initialize_SPI();

	// set pin directions
	LCD_CD_MAKE_OUT();
	LCD_RES_MAKE_OUT();

	// Toggle reset pin
	LCD_RES_LOW();
	usleep(1000);
	LCD_RES_HIGH();
	usleep(1000);

    // Send sequence of command
    LCDSend(0x21, SEND_CMD); // LCD Extended Commands.
   	LCDSend(0xC8, SEND_CMD); // Set LCD Vop (Contrast). 0xC8
	LCDSend(0x04 | !!(LCD_START_LINE_ADDR & (1u << 6)), SEND_CMD);
	LCDSend(0x40 | (LCD_START_LINE_ADDR & ((1u << 6) - 1)), SEND_CMD); 
    LCDSend(0x12, SEND_CMD); // LCD bias mode 1:68.
	LCDSend(0x20, SEND_CMD); // LCD Standard Commands, Horizontal addressing mode.
    LCDSend(0x08, SEND_CMD); // LCD blank
    LCDSend(0x0C, SEND_CMD); // LCD in normal mode.

    // Clear and Update
	LCDClear();
	LCDUpdate();
}

/****************************************************************************/
/*  Reset LCD 	                                                            */
/*  Function : LCDReset                                                     */
/*      Parameters                                                          */
/*          Input   :  Resets the LCD module		                    */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDReset(void)
{

    // Close SPI module - optional
    // NOT DONE

    LCD_RES_LOW();
}

/****************************************************************************/
/*  Update LCD                                                              */
/*  Function : LCDUpdate                                                    */
/*      Parameters                                                          */
/*          Input   :  sends buffer data to the LCD                         */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDUpdate(void)
{
    int x, y;

    for (y = 0; y < 48 / 8; y++)
    {
        LCDSend(0x80, SEND_CMD);
        LCDSend(0x40 | y, SEND_CMD);
        for (x = 0; x < 84; x++)
            LCDSend(LcdMemory[y * 84 + x], SEND_CHR);
    }
}
/****************************************************************************/
/*  Send to LCD                                                             */
/*  Function : LCDSend                                                      */
/*      Parameters                                                          */
/*          Input   :  data and  SEND_CHR or SEND_CMD                       */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDSend(unsigned char data, unsigned char cd)
{

    if (cd == SEND_CHR)
        LCD_CD_HIGH();
    else
        LCD_CD_LOW();

    // send data over SPI
    SPI_Send(&data, 1);
}


/****************************************************************************/
/*  Clear LCD                                                               */
/*  Function : LCDClear                                                     */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDClear(void)
{
    int i;

    // loop all cache array
    for (i = 0; i < LCD_CACHE_SIZE; i++)
    {
        LcdMemory[i] = 0x00;
    }
}

/****************************************************************************/
/*  Write char at x position on y row                                       */
/*  Function : LCDChrXY                                                     */
/*      Parameters                                                          */
/*          Input   :  position, row, char                                  */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDChrXY(unsigned char x, unsigned char y, unsigned char ch)
{
    unsigned int index = 0;
    unsigned int i = 0;

    // check for out off range
    if (x > LCD_X_RES) return;
    if (y > LCD_Y_RES) return;

    index = ((unsigned int) x) * 6 + ((unsigned int) y) * 84;

    for (i = 0; i < 6; i++)
    {
        if (i==5)
          LcdMemory[index++] = 0x00;
        else
          LcdMemory[index++] = FontLookup[ch - 32][i];
    }

}

/****************************************************************************/
/*  Write negative char at x position on y row                              */
/*  Function : LCDChrXYInverse                                              */
/*      Parameters                                                          */
/*          Input   :  position, row, char                                  */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDChrXYInverse(unsigned char x, unsigned char y, unsigned char ch)
{
    unsigned int index = 0;
    unsigned int i = 0;

    // check for out off range
    if (x > LCD_X_RES) return;
    if (y > LCD_Y_RES) return;

    index = ((unsigned int) x) * 6 + ((unsigned int) y) * 84;

    for (i = 0; i < 6; i++)
    {
       if (i==5)
          LcdMemory[index++] = 0xFF;
       else
          LcdMemory[index++] = ~(FontLookup[ch - 32][i]);
    }

}

/****************************************************************************/
/*  Set LCD Contrast                                                        */
/*  Function : LcdContrast                                                  */
/*      Parameters                                                          */
/*          Input   :  contrast                                             */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDContrast(unsigned char contrast)
{

    //  LCD Extended Commands.
    LCDSend(0x21, SEND_CMD);

    // Set LCD Vop (Contrast).
    LCDSend(0x80 | contrast, SEND_CMD);

    //  LCD Standard Commands, horizontal addressing mode.
    LCDSend(0x20, SEND_CMD);
}


/****************************************************************************/
/*  Send string to LCD                                                      */
/*  Function : LCDStr                                                       */
/*      Parameters                                                          */
/*          Input   :  row, text, inversion                                 */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDStr(unsigned char row, const unsigned char *dataPtr, unsigned char inv)
{

    // variable for X coordinate
    unsigned char x = 0;

    // loop to the and of string
    while (*dataPtr)
    {
        if (inv)
        {
            LCDChrXYInverse(x, row, *dataPtr);
        }
        else
        {
            LCDChrXY(x, row, *dataPtr);
        }
        x++;
        dataPtr++;
    }

    LCDUpdate();
}

