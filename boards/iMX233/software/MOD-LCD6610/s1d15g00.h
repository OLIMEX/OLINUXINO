#ifndef s1d15g00_h
#define s1d15g00_h

// *****************************************************************************
// lcd.h
//
// include file for Epson S1D15G00 LCD Controller
//
//
// Author: James P Lynch August 30, 2007
// *****************************************************************************
#define DISON 0xAF // Display on
#define DISOFF 0xAE // Display off
#define DISNOR 0xA6 // Normal display
#define DISINV 0xA7 // Inverse display
#define COMSCN 0xBB // Common scan direction
#define DISCTL 0xCA // Display control
#define SLPIN 0x95 // Sleep in
#define SLPOUT 0x94 // Sleep out
#define PASET 0x75 // Page address set
#define CASET 0x15 // Column address set
#define DATCTL 0xBC // Data scan direction, etc.
#define RGBSET8 0xCE // 256-color position set
#define RAMWR 0x5C // Writing to memory
#define RAMRD 0x5D // Reading from memory
#define PTLIN 0xA8 // Partial display in
#define PTLOUT 0xA9 // Partial display out
#define RMWIN 0xE0 // Read and modify write
#define RMWOUT 0xEE // End
#define ASCSET 0xAA // Area scroll set
#define SCSTART 0xAB // Scroll start set
#define OSCON 0xD1 // Internal oscillation on
#define OSCOFF 0xD2 // Internal oscillation off
#define PWRCTR 0x20 // Power control
#define VOLCTR 0x81 // Electronic volume control
#define VOLUP 0xD6 // Increment electronic control by 1
#define VOLDOWN 0xD7 // Decrement electronic control by 1
#define TMPGRD 0x82 // Temperature gradient set
#define EPCTIN 0xCD // Control EEPROM
#define EPCOUT 0xCC // Cancel EEPROM control
#define EPMWR 0xFC // Write into EEPROM
#define EPMRD 0xFD // Read from EEPROM
#define EPSRRD1 0x7C // Read register 1
#define EPSRRD2 0x7D // Read register 2
#define NOP 0x25 // NOP instruction

#define BKLGHT_LCD_ON 1
#define BKLGHT_LCD_OFF 2

// backlight control
#define BKLGHT_LCD_ON 1
#define BKLGHT_LCD_OFF 2

// Booleans
#define NOFILL 0
#define FILL 1

// 12-bit color definitions
#define WHITE 0xFFF
#define BLACK 0x000
#define RED 0xF00
#define GREEN 0x0F0
#define BLUE 0x00F
#define CYAN 0x0FF
#define MAGENTA 0xF0F
#define YELLOW 0xFF0
#define BROWN 0xB22
#define ORANGE 0xFA0
#define PINK 0xF6A

// Font sizes
#define SMALL 0
#define MEDIUM 1
#define LARGE 2

// Define LCD-NOKIA6610 PinIO Interface Mask Bit 
#define  LCD_BL_PIN 		    0x00000001   					// P2.0  (0000 0000 0000 0000 0000 0000 0000 000x)
#define  LCD_CS_PIN		        0x00000002   					// P2.1  (0000 0000 0000 0000 0000 0000 0000 00x0)
#define  LCD_SCLK_PIN		    0x00000004						// P2.2  (0000 0000 0000 0000 0000 0000 0000 0x00)
#define  LCD_SDATA_PIN			0x00000008						// P2.3  (0000 0000 0000 0000 0000 0000 0000 x000)
#define  LCD_RESET_PIN			0x00000010						// P2.4  (0000 0000 0000 0000 0000 0000 000x 0000)

#define  LCD_BL_DIR()			FIO2DIR |= LCD_BL_PIN			// BL       = Output
#define  LCD_CS_DIR()			FIO2DIR |= LCD_CS_PIN			// CS#      = Output 
#define  LCD_SCLK_DIR()			FIO2DIR |= LCD_SCLK_PIN			// SCLK     = Output 
#define  LCD_SDATA_DIR()		FIO2DIR |= LCD_SDATA_PIN		// SDATA    = Output
#define  LCD_RESET_DIR()		FIO2DIR |= LCD_RESET_PIN		// RESET#   = Output

#define  LCD_BL_HIGH()  		FIO2SET  = LCD_BL_PIN			// BL     = '1' 
#define  LCD_BL_LOW()  			FIO2CLR  = LCD_BL_PIN			// BL     = '0'
#define  LCD_CS_HIGH()  		FIO2SET  = LCD_CS_PIN			// CS#    = '1' 
#define  LCD_CS_LOW()  			FIO2CLR  = LCD_CS_PIN			// CS#    = '0'
#define  LCD_SCLK_HIGH() 		FIO2SET  = LCD_SCLK_PIN			// SCLK   = '1' 
#define  LCD_SCLK_LOW() 		FIO2CLR  = LCD_SCLK_PIN			// SCLK   = '0'
#define  LCD_SDATA_HIGH() 		FIO2SET  = LCD_SDATA_PIN		// SDATA  = '1' 
#define  LCD_SDATA_LOW() 		FIO2CLR  = LCD_SDATA_PIN		// SDATA  = '0'
#define  LCD_RESET_HIGH() 		FIO2SET  = LCD_RESET_PIN		// RESET# = '1' 
#define  LCD_RESET_LOW() 		FIO2CLR  = LCD_RESET_PIN		// RESET# = '0'
// End of Define For LCD-NOKIA6610

// mask definitions
#define BIT0  0x00000001
#define BIT1  0x00000002
#define BIT2  0x00000004
#define BIT3  0x00000008
#define BIT4  0x00000010
#define BIT5  0x00000020
#define BIT6  0x00000040
#define BIT7  0x00000080
#define BIT8  0x00000100
#define BIT9  0x00000200
#define BIT10 0x00000400
#define BIT11 0x00000800
#define BIT12 0x00001000
#define BIT13 0x00002000
#define BIT14 0x00004000
#define BIT15 0x00008000
#define BIT16 0x00010000
#define BIT17 0x00020000
#define BIT18 0x00040000
#define BIT19 0x00080000
#define BIT20 0x00100000
#define BIT21 0x00200000
#define BIT22 0x00400000
#define BIT23 0x00800000
#define BIT24 0x01000000
#define BIT25 0x02000000
#define BIT26 0x04000000
#define BIT27 0x08000000
#define BIT28 0x10000000
#define BIT29 0x20000000
#define BIT30 0x40000000
#define BIT31 0x80000000

void SPI_Init (void);
void WriteSpiCommand(unsigned int data);
void WriteSpiData(unsigned int data);
void Backlight(unsigned char state);
void InitLcd(void);
void LCDWrite130x130bmp(void);
void LCDClearScreen(void);
void LCDSetPixel(int x, int y, int color);
void LCDSetLine(int x1, int y1, int x2, int y2, int color);
void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color);
void LCDSetCircle(int x0, int y0, int radius, int color);
void LCDPutChar(char c, int x, int y, int size, int fcolor, int bcolor);
void LCDPutStr(char *pString, int x, int y, int Size, int fColor, int bColor);
#endif // Lcd_h
