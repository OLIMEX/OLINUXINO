/*
 *
 * This file is part of pyA20SOM.
 * mapping.h is python GPIO extension.
 *
 * Copyright (c) 2014 Stefan Mavrodiev @ OLIMEX LTD, <support@olimex.com>
 *
 * pyA20SOM is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef __MAPPING_H
#define __MAPPING_H

#include "gpio_lib.h"

/**
Structure that describe all gpio
*/
typedef struct _pin {
    char name[10];          // The processor pin
    int offset;             // Memory offset for the pin
    int pin_number;         // Number on the connector
}pin_t;

typedef struct _gpio {
    char connector_name[10];
    pin_t pins[41];
}gpio_t;


gpio_t gpio[] = {
    {"lcd",
        {
            {   "PD16", SUNXI_GPD(16),  5   },
            {   "PD17", SUNXI_GPD(17),  6   },
            {   "PD18", SUNXI_GPD(18),  7   },
            {   "PD19", SUNXI_GPD(19),  8   },
            {   "PD20", SUNXI_GPD(20),  9   },
            {   "PD21", SUNXI_GPD(21),  10  },
            {   "PD22", SUNXI_GPD(22),  11  },
            {   "PD23", SUNXI_GPD(23),  12  },
            {   "PD8", SUNXI_GPD(8),    13  },
            {   "PD9", SUNXI_GPD(9),    14  },
            {   "PD10", SUNXI_GPD(10),  15  },
            {   "PD11", SUNXI_GPD(11),  16  },
            {   "PD12", SUNXI_GPD(12),  17  },
            {   "PD13", SUNXI_GPD(13),  18  },
            {   "PD14", SUNXI_GPD(14),  19  },
            {   "PD15", SUNXI_GPD(15),  20  },
            {   "PD0",  SUNXI_GPD(0),   21  },
            {   "PD1",  SUNXI_GPD(1),   22  },
            {   "PD2",  SUNXI_GPD(2),   23  },
            {   "PD3",  SUNXI_GPD(3),   24  },
            {   "PD4",  SUNXI_GPD(4),   25  },
            {   "PD5",  SUNXI_GPD(5),   26  },
            {   "PD6",  SUNXI_GPD(6),   27  },
            {   "PD7",  SUNXI_GPD(7),   28  },
            {   "PD26", SUNXI_GPD(26),  29  },
            {   "PD27", SUNXI_GPD(27),  30  },
            {   "PD24", SUNXI_GPD(24),  31  },
            {   "PD25", SUNXI_GPD(25),  32  },
            {   "PH7",  SUNXI_GPH(7),   35  },
            {   "PB2",  SUNXI_GPB(2),   36  },
            {
                {   0,  0,  0}
            },
        }
    },
    {"gpio1",
        {
            {   "PB9",  SUNXI_GPB(9),   7   },
            {   "PH5",  SUNXI_GPH(5),   9   },
            {   "PH4",  SUNXI_GPH(4),   11  },
            {   "PH6",  SUNXI_GPH(6),   13  },
            {   "PH3",  SUNXI_GPH(3),   15  },
            {   "PI0",  SUNXI_GPI(0),   29  },
            {   "PI1",  SUNXI_GPI(1),   31  },
            {   "PI2",  SUNXI_GPI(2),   33  },
            {   "PI3",  SUNXI_GPI(3),   35  },
            {   "PI10", SUNXI_GPI(10),  37  },
            {   "PI11", SUNXI_GPI(11),  39  },
            {   "PI14", SUNXI_GPI(14),  40  },
            {
                {   0,  0,  0}
            },
        }
    },
    {"gpio2",
        {
            {   "PB22", SUNXI_GPB(22),  21  },
            {   "PB23", SUNXI_GPB(23),  22  },
            {   "PA0",  SUNXI_GPA(0),   29  },
            {   "PA1",  SUNXI_GPA(1),   27  },
            {   "PA2",  SUNXI_GPA(2),   25  },
            {   "PA3",  SUNXI_GPA(3),   23  },
            {   "PA4",  SUNXI_GPA(4),   30  },
            {   "PA5",  SUNXI_GPA(5),   28  },
            {   "PA6",  SUNXI_GPA(6),   26  },
            {   "PA7",  SUNXI_GPA(7),   24  },
            {   "PA8",  SUNXI_GPA(8),   33  },
            {   "PA9",  SUNXI_GPA(9),   35  },
            {   "PA10", SUNXI_GPA(10),  31  },
            {   "PA11", SUNXI_GPA(11),  37  },
            {   "PA12", SUNXI_GPA(12),  39  },
            {   "PA13", SUNXI_GPA(13),  32  },
            {   "PA14", SUNXI_GPA(14),  34  },
            {   "PA15", SUNXI_GPA(15),  36  },
            {   "PA16", SUNXI_GPA(16),  38  },
            {   "PA17", SUNXI_GPA(17),  40  },

            {
                {   0,  0,  0}
            },
        }
    },

    {"gpio3",
        {
            {   "PH0",  SUNXI_GPH(0),   5   },
            {   "PH2",  SUNXI_GPH(2),   7   },
            {   "PH8",  SUNXI_GPH(8),   9   },
            {   "PH9",  SUNXI_GPH(9),   11  },
            {   "PH10", SUNXI_GPH(10),  13  },
            {   "PH11", SUNXI_GPH(11),  15  },
            {   "PH12", SUNXI_GPH(12),  17  },
            {   "PH13", SUNXI_GPH(13),  19  },
            {   "PH14", SUNXI_GPH(14),  21  },
            {   "PH15", SUNXI_GPH(15),  23  },
            {   "PH16", SUNXI_GPH(16),  25  },
            {   "PH17", SUNXI_GPH(17),  27  },
            {   "PH18", SUNXI_GPH(18),  29  },
            {   "PH19", SUNXI_GPH(19),  31  },
            {   "PH20", SUNXI_GPH(20),  33  },
            {   "PH21", SUNXI_GPH(21),  35  },
            {   "PH22", SUNXI_GPH(22),  37  },
            {   "PH23", SUNXI_GPH(23),  39  },

            {   "PB3",  SUNXI_GPB(3),   6   },
            {   "PB4",  SUNXI_GPB(4),   8   },
            {   "PB5",  SUNXI_GPB(5),   10  },
            {   "PB6",  SUNXI_GPB(6),   12  },
            {   "PB7",  SUNXI_GPB(7),   14  },
            {   "PB8",  SUNXI_GPB(8),   16  },
            {   "PB10",  SUNXI_GPB(10), 18  },
            {   "PB11",  SUNXI_GPB(11), 20  },
            {   "PB12",  SUNXI_GPB(12), 22  },
            {   "PB13",  SUNXI_GPB(13), 24  },
            {   "PB14",  SUNXI_GPB(14), 26  },
            {   "PB15",  SUNXI_GPB(15), 28  },
            {   "PB16",  SUNXI_GPB(16), 30  },
            {   "PB17",  SUNXI_GPB(17), 32  },
            {   "PH24",  SUNXI_GPH(24), 34  },
            {   "PH25",  SUNXI_GPH(25), 36  },
            {   "PH26",  SUNXI_GPH(26), 38  },
            {   "PH27",  SUNXI_GPH(27), 40  },
            {
                {   0,  0,  0}
            },
        }
    },

    {"gpio4",
        {
            {   "PB0",  SUNXI_GPB(0),   5   },
            {   "PB1",  SUNXI_GPB(1),   7   },
            {   "PE0",  SUNXI_GPE(0),   6   },
            {   "PE1",  SUNXI_GPE(1),   8   },
            {   "PE2",  SUNXI_GPE(2),   10  },
            {   "PE3",  SUNXI_GPE(3),   12  },
            {   "PE4",  SUNXI_GPE(4),   14  },
            {   "PE5",  SUNXI_GPE(5),   16  },
            {   "PE6",  SUNXI_GPE(0),   18  },
            {   "PE7",  SUNXI_GPE(0),   9   },
            {   "PE8",  SUNXI_GPE(0),   11  },
            {   "PE9",  SUNXI_GPE(0),   13  },
            {   "PE10", SUNXI_GPE(0),   15  },
            {   "PE11", SUNXI_GPE(0),   17  },
            {   "PI20", SUNXI_GPI(20),  19  },
            {   "PI21", SUNXI_GPI(21),  20  },
            {   "PB18",  SUNXI_GPB(18), 21  },
            {   "PB19",  SUNXI_GPB(19), 22  },
            {   "PI16",  SUNXI_GPI(16), 23  },
            {   "PI17",  SUNXI_GPI(17), 24  },
            {   "PI18",  SUNXI_GPI(18), 25  },
            {   "PI19",  SUNXI_GPI(19), 26  },
            {   "PI12",  SUNXI_GPI(12), 27  },
            {   "PI13",  SUNXI_GPI(13), 28  },
            {   "PB20",  SUNXI_GPB(20), 29  },
            {   "PB21",  SUNXI_GPB(21), 30  },
            {   "PC22",  SUNXI_GPC(22), 31  },
            {   "PC21",  SUNXI_GPC(21), 32  },
            {   "PC20",  SUNXI_GPC(20), 33  },
            {   "PC19",  SUNXI_GPC(21), 34  },
            {   "PI15",  SUNXI_GPI(15), 36  },
            {
                {   0,  0,  0}
            },
        }
    },

    {"gpio5",
        {
            {   "PG0",  SUNXI_GPG(0),   1   },
            {   "PG1",  SUNXI_GPG(1),   3   },
            {   "PG2",  SUNXI_GPG(2),   5   },
            {   "PG3",  SUNXI_GPG(3),   7   },
            {   "PG4",  SUNXI_GPG(4),   9   },
            {   "PG5",  SUNXI_GPG(5),   11  },
            {   "PG6",  SUNXI_GPG(6),   13  },
            {   "PG7",  SUNXI_GPG(7),   15  },
            {   "PG8",  SUNXI_GPG(8),   17  },
            {   "PG9",  SUNXI_GPG(9),   19  },
            {   "PG10", SUNXI_GPG(10),  21  },
            {   "PG11", SUNXI_GPG(11),  23  },
            {   "PC8",  SUNXI_GPC(8),   25  },
            {   "PC9",  SUNXI_GPC(9),   27  },
            {   "PC10", SUNXI_GPC(10),  29  },
            {   "PC11", SUNXI_GPC(11),  31  },
            {   "PC12", SUNXI_GPC(12),  33  },
            {   "PC13", SUNXI_GPC(13),  35  },
            {   "PC14", SUNXI_GPC(14),  37  },
            {   "PC15", SUNXI_GPC(15),  39  },
            {   "PI6",  SUNXI_GPI(6),   4   },
            {   "PI7",  SUNXI_GPI(7),   6   },
            {   "PI8",  SUNXI_GPI(8),   8   },
            {   "PI9",  SUNXI_GPI(9),   10  },
            {   "PI4",  SUNXI_GPI(4),   12  },
            {   "PI5",  SUNXI_GPI(5),   14  },
            {   "PC16", SUNXI_GPC(16),  16  },
            {   "PC17", SUNXI_GPC(17),  18  },
            {   "PC18", SUNXI_GPC(18),  20  },
            {   "PC23", SUNXI_GPC(23),  22  },
            {   "PC24", SUNXI_GPC(24),  24  },
            {   "PC7",  SUNXI_GPC(7),   26  },
            {   "PC6",  SUNXI_GPC(6),   28  },
            {   "PC5",  SUNXI_GPC(5),   30  },
            {   "PC3",  SUNXI_GPC(3),   32  },
            {   "PC2",  SUNXI_GPC(2),   34  },
            {   "PC1",  SUNXI_GPC(1),   36  },
            {   "PC0",  SUNXI_GPC(0),   38  },
            {   "PC4",  SUNXI_GPC(4),   40  },
            {
                {   0,  0,  0}
            },
        }
    },

};



#endif