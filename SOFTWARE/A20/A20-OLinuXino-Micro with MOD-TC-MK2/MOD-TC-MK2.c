#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

// since transfer should be done without stop bit the regular read/write I2C functions doesn't work
// used the i2ctransfer-like algorithm to achieve it
// based on the code here: https://fossies.org/linux/i2c-tools/tools/i2ctransfer.c


// MOD-TC-MK2 defines
#define MOD_TC_MK2_ADDR 0x23
#define SET_ADDRESS     0xF0
#define GET_ID          0x20
#define GET_TEMP        0x21


#define ADDRESS         MOD_TC_MK2_ADDR
#define USED_I2C_BUS    2

// Simple delay function
void Delay (volatile unsigned long int Time)
{
    while (--Time);
}

// reading without a stop bit between writing the command and reading the data (repeated start)
int Read_I2C (int file, unsigned char Address, unsigned char Command, unsigned char Data[], unsigned char Bytes)
{
    struct i2c_rdwr_ioctl_data rdwr;
    struct i2c_msg msgs[2];
    unsigned char nmsgs_sent;

    // the fields of the first element of the structure consist the data to be send:
    // 1) address of the slave
    // 2) flags (in the case of writing no flags),
    // 3) number of bytes to send
    // 4) array with the data itself
    msgs[0].addr = Address;
    msgs[0].flags=0;
    msgs[0].len = 1;
    msgs[0].buf = &Command;

    // the fields of the second element of the structure consist the data to be received:
    // 1) address of the slave
    // 2) flags (in case of reading I2C_M_RD)
    // 3) number of bytes to read
    // 4) array where the data read will be stored
    msgs[1].addr = Address;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = Bytes;
    msgs[1].buf = Data;

    rdwr.msgs = msgs;
    rdwr.nmsgs = 2;

    nmsgs_sent = ioctl(file, I2C_RDWR, &rdwr);
    if (nmsgs_sent < 2)
    {
        printf ("I2C transfer error!\n");
        return 1;
    }
    return 0;
}

float Calculate_Inner_Temperature (unsigned char Data[])
{
    long int Temp;
    Temp = (Data[2]<<8) | Data[3];
    Temp = Temp>>4;
    //printf ("Inner = 0x%x\n", Temp);
    return (double) Temp / 16.0;
}

float Calculate_Outer_Temperature (unsigned char Data[])
{
    long int Temp;
    Temp = (Data[0]<<8) | Data[1];
    Temp = Temp>>2;
    //printf ("Outer = 0x%x\n", Temp);
    return (double) Temp / 4.0;
}


// in command line:
// transfering data on i2c bus 2, writing 1 byte to address 0x23, data 0x21 (for temperature) 0x20 for ID, reading 4 bytes (for temperature) or 1 byte (for ID)
// i2ctransfer -y 2 w1@0x23 0x21 r4     // reading data
// i2ctransfer -y 2 w1@0x23 0x20 r1     // reading ID
int main ()
{
    int file;
    char filename[20];
    unsigned char RcvData[4];
    float Temp1, Temp2;

    snprintf(filename, 19, "/dev/i2c-%d", USED_I2C_BUS);

    file = open(filename, O_RDWR);
    if (file < 0)
        printf ("Error with the I2C%d bus!\n", USED_I2C_BUS);

    Read_I2C (file, ADDRESS, GET_ID, RcvData, 1);
    if (RcvData[0] != 0x27)
        printf ("Wrong ID of the module!\n");

    while (1)
    {
        Read_I2C (file, ADDRESS, GET_TEMP, RcvData, 4);
        if (RcvData[1] & 0x01)
        {
            printf ("Fault: ");
            if (RcvData[3] & 0x1)
                printf ("Open Circuit; ");
            if (RcvData[3] & 0x2)
                printf ("Short to GND; ");
            if (RcvData[3] & 0x4)
                printf ("Short to VCC; ");
            printf ("\n");
        }
        else
        {
            Temp1 = Calculate_Inner_Temperature (RcvData);
            Temp2 = Calculate_Outer_Temperature (RcvData);
            printf ("\fInner temperature: %.4f C\nOuter temperature: %.2f C\n", Temp1, Temp2);
        }
        Delay (50000000);
    }
    return 0;
}
