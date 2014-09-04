/*
 *
 * This file is part of pyA20Lime.
 * mapping.h is python GPIO extension.
 *
 * Copyright (c) 2014 Stefan Mavrodiev @ OLIMEX LTD, <support@olimex.com>
 *
 * pyA20Lime is free software; you can redistribute it and/or modify
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
            {   "PD8",  SUNXI_GPD(8),   13  },
            {   "PD9",  SUNXI_GPD(9),   14  },
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
            {   "PH8",  SUNXI_GPH(8),   35  },
            {   "PB2",  SUNXI_GPB(2),   36  },
            {
                {   0,  0,  0}
            },
        }
    },

    {"gpio1",
        {
            {   "PG0",  SUNXI_GPG(0),   5   },
            {   "PG1",  SUNXI_GPG(1),   7   },
            {   "PG2",  SUNXI_GPG(2),   9   },
            {   "PG3",  SUNXI_GPG(3),   11  },
            {   "PG4",  SUNXI_GPG(4),   13  },
            {   "PG5",  SUNXI_GPG(5),   15  },
            {   "PG6",  SUNXI_GPG(6),   17  },
            {   "PG7",  SUNXI_GPG(7),   19  },
            {   "PG8",  SUNXI_GPG(8),   21  },
            {   "PG9",  SUNXI_GPG(9),   23  },
            {   "PG10", SUNXI_GPG(10),  25  },
            {   "PG11", SUNXI_GPG(11),  27  },
            {   "PC3",  SUNXI_GPC(3),   29  },
            {   "PC18", SUNXI_GPC(18),  31  },
            {   "PC19", SUNXI_GPC(19),  33  },
            {   "PC20", SUNXI_GPC(20),  35  },
            {   "PC21", SUNXI_GPC(21),  37  },
            {   "PC22", SUNXI_GPC(22),  39  },
            {   "PC23", SUNXI_GPC(23),  40  },
            {   "PC24", SUNXI_GPC(24),  38  },
            {   "PB18", SUNXI_GPB(18),  36  },
            {   "PB19", SUNXI_GPB(19),  34  },
            {   "PB20", SUNXI_GPB(20),  32  },
            {   "PB21", SUNXI_GPB(21),  30  },
            {
                {   0,  0,  0}
            },
        }
    },

    {"gpio2",
        {
            {   "PI0",  SUNXI_GPI(0),   9   },
            {   "PI1",  SUNXI_GPI(1),   11  },
            {   "PI2",  SUNXI_GPI(2),   13  },
            {   "PI3",  SUNXI_GPI(3),   15  },
            {   "PI4",  SUNXI_GPI(4),   17  },
            {   "PI5",  SUNXI_GPI(5),   19  },
            {   "PI6",  SUNXI_GPI(6),   21  },
            {   "PI7",  SUNXI_GPI(7),   23  },
            {   "PI8",  SUNXI_GPI(8),   25  },
            {   "PI9",  SUNXI_GPI(9),   27  },
            {   "PI10", SUNXI_GPI(10),  29  },
            {   "PI11", SUNXI_GPI(11),  31  },
            {   "PI12", SUNXI_GPI(12),  33  },
            {   "PI13", SUNXI_GPI(13),  35  },
            {   "PI14", SUNXI_GPI(14),  37  },
            {   "PI15", SUNXI_GPI(15),  39  },
            {   "PI16", SUNXI_GPI(16),  40  },
            {   "PI17", SUNXI_GPI(17),  38  },
            {   "PI18", SUNXI_GPI(18),  36  },
            {   "PI19", SUNXI_GPI(19),  34  },
            {   "PI20", SUNXI_GPI(20),  32  },
            {   "PI21", SUNXI_GPI(21),  30  },
            {   "PE0",  SUNXI_GPE(0),   6   },
            {   "PE1",  SUNXI_GPE(1),   8   },
            {   "PE2",  SUNXI_GPE(2),   10  },
            {   "PE3",  SUNXI_GPE(3),   12  },
            {   "PE4",  SUNXI_GPE(4),   14  },
            {   "PE5",  SUNXI_GPE(5),   16  },
            {   "PE6",  SUNXI_GPE(6),   18  },
            {   "PE7",  SUNXI_GPE(7),   20  },
            {   "PE8",  SUNXI_GPE(8),   22  },
            {   "PE9",  SUNXI_GPE(9),   24  },
            {   "PE10", SUNXI_GPE(10),  26  },
            {   "PE11", SUNXI_GPE(11),  28  },
            {
                {   0,  0,  0}
            },
        }
    },

    {"gpio3",
        {
            {   "PH0",  SUNXI_GPH(0),   7   },
            {   "PH7",  SUNXI_GPH(7),   9   },
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
            {   "PH24", SUNXI_GPH(24),  34  },
            {   "PH25", SUNXI_GPH(25),  36  },
            {   "PH26", SUNXI_GPH(26),  38  },
            {   "PH27", SUNXI_GPH(27),  40  },
            {   "PB3",  SUNXI_GPB(3),   6   },
            {   "PB4",  SUNXI_GPB(4),   8   },
            {   "PB5",  SUNXI_GPB(5),   10  },
            {   "PB6",  SUNXI_GPB(6),   12  },
            {   "PB7",  SUNXI_GPB(7),   14  },
            {   "PB8",  SUNXI_GPB(8),   16  },
            {   "PB10", SUNXI_GPB(10),  18  },
            {   "PB11", SUNXI_GPB(11),  20  },
            {   "PB12", SUNXI_GPB(12),  22  },
            {   "PB13", SUNXI_GPB(13),  24  },
            {   "PB14", SUNXI_GPB(14),  26  },
            {   "PB15", SUNXI_GPB(15),  28  },
            {   "PB16", SUNXI_GPB(16),  30  },
            {   "PB17", SUNXI_GPB(17),  32  },
            {
                {   0,  0,  0}
            },
        }
    },

    {"gpio4",
        {
            {   "PC7",  SUNXI_GPC(7),   16  },
            {   "PC16", SUNXI_GPC(16),  18  },
            {   "PC17", SUNXI_GPC(17),  20  },
            {
                {   0,  0,  0}
            },
        }
    },

    {"led",
        {
            {   "PH2",  SUNXI_GPH(2),   1},
            {
                {   0,  0,  0}
            },
        }
    },
};

#endif