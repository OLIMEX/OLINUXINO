/*
 *
 * This file is part of pyA13.
 * mapping.h is python GPIO extension.
 *
 * Copyright (c) 2014 Stefan Mavrodiev @ OLIMEX LTD, <support@olimex.com>
 *
 * pyA13 is free software; you can redistribute it and/or modify
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
                {   "PD18", SUNXI_GPD(18),  5   },
                {   "PD18", SUNXI_GPD(18),  6   },
                {   "PD18", SUNXI_GPD(18),  7   },
                {   "PD19", SUNXI_GPD(19),  8   },
                {   "PD20", SUNXI_GPD(20),  9   },
                {   "PD21", SUNXI_GPD(21),  10  },
                {   "PD22", SUNXI_GPD(22),  11  },
                {   "PD23", SUNXI_GPD(23),  12  },
                {   "PD10", SUNXI_GPD(10),  13  },
                {   "PD10", SUNXI_GPD(10),  14  },
                {   "PD10", SUNXI_GPD(10),  15  },
                {   "PD11", SUNXI_GPD(11),  16  },
                {   "PD12", SUNXI_GPD(12),  17  },
                {   "PD13", SUNXI_GPD(13),  18  },
                {   "PD14", SUNXI_GPD(14),  19  },
                {   "PD15", SUNXI_GPD(15),  20  },
                {   "PD2",  SUNXI_GPD(2),   21  },
                {   "PD2",  SUNXI_GPD(2),   22  },
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
                {   "PB3",  SUNXI_GPB(3),   33  },
                {   "PB4",  SUNXI_GPB(4),   34  },
                {   "PB10", SUNXI_GPB(10),  35  },
                {   "PB2",  SUNXI_GPB(2),   36  },
                {
                    {   0,  0,  0}
                },
        }
    },
    {"gpio2",
        {
            {   "PB0",  SUNXI_GPB(0),   5   },
            {   "PG11", SUNXI_GPG(11),  6   },
            {   "PB1",  SUNXI_GPB(1),   7   },
            {   "PG10", SUNXI_GPG(10),  8   },
            {   "PB2",  SUNXI_GPB(2),   9   },
            {   "PG9",  SUNXI_GPG(9),   10  },
            {   "PB3",  SUNXI_GPB(3),   11  },
            {   "PE11", SUNXI_GPE(11),  12  },
            {   "PB4",  SUNXI_GPB(4),   13  },
            {   "PE10", SUNXI_GPE(10),  14  },
            {   "PB10", SUNXI_GPB(10),  15  },
            {   "PE9",  SUNXI_GPE(9),   16  },
            {   "PB15", SUNXI_GPB(15),  17  },
            {   "PE8",  SUNXI_GPE(8),   18  },
            {   "PB16", SUNXI_GPB(16),  19  },
            {   "PE7",  SUNXI_GPE(7),   20  },
            {   "PC0",  SUNXI_GPC(0),   21  },
            {   "PE6",  SUNXI_GPE(6),   22  },
            {   "PC1",  SUNXI_GPC(1),   23  },
            {   "PE5",  SUNXI_GPE(5),   24  },
            {   "PC2",  SUNXI_GPC(2),   25  },
            {   "PE4",  SUNXI_GPE(4),   26  },
            {   "PC3",  SUNXI_GPC(3),   27  },
            {   "PC19", SUNXI_GPC(19),  28  },
            {   "PC4",  SUNXI_GPC(4),   29  },
            {   "PC15", SUNXI_GPC(15),  30  },
            {   "PC5",  SUNXI_GPC(5),   31  },
            {   "PC14", SUNXI_GPC(14),  32  },
            {   "PC6",  SUNXI_GPC(6),   33  },
            {   "PC13", SUNXI_GPC(13),  34  },
            {   "PC7",  SUNXI_GPC(7),   35  },
            {   "PC12", SUNXI_GPC(12),  36  },
            {   "PC8",  SUNXI_GPC(8),   37  },
            {   "PC11", SUNXI_GPC(11),  38  },
            {   "PC9",  SUNXI_GPC(9),   39  },
            {   "PC10", SUNXI_GPC(10),  40  },
            {
                {   0,  0,  0}
            },
        }
    },
    {"uext",
        {
            {   "PG3",  SUNXI_GPG(3),   3   },
            {   "PG3",  SUNXI_GPG(4),   4   },
            {   "PB17", SUNXI_GPB(17),  5   },
            {   "PB18", SUNXI_GPB(18),  6   },
            {   "PE3",  SUNXI_GPE(3),   7   },
            {   "PE2",  SUNXI_GPE(2),   8   },
            {   "PE1",  SUNXI_GPE(1),   9   },
            {   "PE0",  SUNXI_GPE(0),   10  },
            {
                {   0,  0,  0}
            },
        }
    },
};



#endif