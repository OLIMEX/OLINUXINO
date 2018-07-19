#include "LCD3310.h"
#include "gpio_lib.h"

int main ()
{
	LCDInit();
	LCDContrast(0x70);
	LCDStr(0, (unsigned char *)"*** OLIMEX ***", 1);
	LCDStr(1, (unsigned char *)"  OLinuXINO   ", 0);
	LCDStr(2, (unsigned char *)"  Micro and   ", 1);
	LCDStr(3, (unsigned char *)"  MOD-LCD3310 ", 0);
	LCDStr(4, (unsigned char *)"  using SPI   ", 1);
	LCDStr(5, (unsigned char *)"  interface   ", 0);
	return 0;
}


