#include <stdio.h>
#include "i2c.h"

// I2C address of the module
#define DS3231_ADDRESS  0x68

// registers map
#define SECONDS         0x00
#define MINUTES         0x01
#define HOURS           0x02
#define DAY_OF_WEEK     0x03
#define DATE            0x04
#define MONTH           0x05
#define YEAR            0x06

#define ALARM1_SECONDS  0x07
#define ALARM1_MINUTES  0x08
#define ALARM1_HOURS    0x09
#define ALARM1_DAY_DATE 0x0A

#define ALARM2_MINUTES  0x0B
#define ALARM2_HOURS    0x0C
#define ALARM2_DAY_DATE 0x0D

#define CTRL_REGISTER   0x0E
#define STAT_REGISTER   0x0F

#define AGING_OFFSET    0x10
#define TEMPERATURE_H   0x11
#define TEMPERATURE_L   0x12


const char DaysOfTheWeek[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

unsigned char BCDtoInt(unsigned char BCD)
{
    unsigned char a, b;

    a = BCD & 0x0F;
    b = BCD >> 4;

    return b*10 + a;
}

unsigned char InttoBCD(unsigned char Int)
{
    unsigned char a, b;

    a = Int % 10;
    b = (Int / 10) % 10;

    return (b << 4) + a;
}

void Write_Register (int *fd, unsigned char Address, unsigned char Value)
{
    unsigned char Buff[2] = {Address, Value};
    I2C_Send (fd, Buff, 2);
}

void Read_Register (int *fd, unsigned char StartingAddress, unsigned char NumberOfBytes, unsigned char Destination[])
{
    I2C_Send(fd, &StartingAddress, 1);
    I2C_Read(fd, Destination, NumberOfBytes);       // reading all registers starting from 0x00-0x12
}

int main ()
{
    int fd;
    unsigned char Read_Buff[19];
    int Century=20, Year, Month, Date, Day, Hours, Minutes, Seconds, Prev_Sec=-1;
    double Temperature;

    /* Open I2C-BUS */
    I2C_Open(&fd, DS3231_ADDRESS);

    // all of the numbers in the DS3231 registers are stored as binary coded decimals (BCD)
    // set the time and date to 1999 December 31 (Friday) 23:59:57
    Write_Register (&fd, SECONDS    , 0x57);    // seconds
    Write_Register (&fd, MINUTES    , 0x59);    // minutes
    Write_Register (&fd, HOURS      , 0x23);    // hours
    Write_Register (&fd, DAY_OF_WEEK, 0x05);    // day of the week
    Write_Register (&fd, DATE       , 0x31);    // date
    Write_Register (&fd, MONTH      , 0x12);    // month
    Write_Register (&fd, YEAR       , 0x99);    // year (last 2 digits)

    // set Alarm 1 (which has seconds to set on top of hour, minutes and day/date)
    // MSB of the 4 registers determine when the alarm will be triggered (check datasheet for more details)
    // let's set it to trigger every day (of the week) at 00:00:05
    Write_Register (&fd, ALARM1_SECONDS , 0x05);    // seconds
    Write_Register (&fd, ALARM1_MINUTES , 0x00);    // minutes
    Write_Register (&fd, ALARM1_HOURS   , 0x00);    // hours
    Write_Register (&fd, ALARM1_DAY_DATE, 0x80);    // day of the week/date of the month

    // set Alarm 2 (has minutes, hour and day/date)
    // MSB of the 3 registers determine when the alarm will be triggered (check datasheet for more details)
    // let's set it to trigger every 1st date (of the month) at 00:00:00
    Write_Register (&fd, ALARM2_MINUTES , 0x00);    // minutes
    Write_Register (&fd, ALARM2_HOURS   , 0x00);    // hours
    Write_Register (&fd, ALARM2_DAY_DATE, 0xC1);    // day of the week/date of the month

    while (1)
    {
        Write_Register (&fd, CTRL_REGISTER, 0x3C);      // Control register -> Convert temperature enabled (this bit has to be raised constantly), 8.192 kHz

        // reading all the registers (0x00-0x12) into Read_Buff
        Read_Register (&fd, 0x00, 19, Read_Buff);
        Year    = BCDtoInt (Read_Buff[YEAR]        & 0xFF);
        Month   = BCDtoInt (Read_Buff[MONTH]       & 0x1F);
        Date    = BCDtoInt (Read_Buff[DATE]        & 0x3F);
        Day     = BCDtoInt (Read_Buff[DAY_OF_WEEK] & 0x07);
        Hours   = BCDtoInt (Read_Buff[HOURS]       & 0x3F);
        Minutes = BCDtoInt (Read_Buff[MINUTES]     & 0x7F);
        Seconds = BCDtoInt (Read_Buff[SECONDS]     & 0x7F);

        Temperature = (((signed int)((Read_Buff[TEMPERATURE_H]<<8) | Read_Buff[TEMPERATURE_L]))>>6)/4.0;

        if (Read_Buff[MONTH] & 0x80)    // bit 7 of the month register (address 0x05) store new century occurrence
        {
            Century++;
            printf ("New century!\n");
            Write_Register (&fd, MONTH, Read_Buff[MONTH] & 0x7F);   // clearing the new century flag
        }

        if (Read_Buff[STAT_REGISTER] & 0x01)
        {
            printf ("Alarm 1 triggered!\n");
            Write_Register (&fd, STAT_REGISTER, Read_Buff[STAT_REGISTER] & 0xFE);   // clearing the Alarm 1 flag
        }

        if (Read_Buff[STAT_REGISTER] & 0x02)
        {
            printf ("Alarm 2 triggered!\n");
            Write_Register (&fd, STAT_REGISTER, Read_Buff[STAT_REGISTER] & 0xFD);   // clearing the Alarm 2 flag
        }


        if (Seconds != Prev_Sec)
        {
            printf ("%2d/%2d/%2d (%s) %2d:%2d:%2d;  Temperature = %3.2lf C\n", (Century-1)*100+Year, Month, Date, DaysOfTheWeek[Day], Hours, Minutes, Seconds, Temperature);
            Prev_Sec = Seconds;
        }
    }
    /* Close I2C-BUS */
    I2C_Close(&fd);

    return 0;
}
