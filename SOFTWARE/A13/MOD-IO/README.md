This example works with all Olimex Allwinner boards (A10, A13, A20) using the official Debian images.

Make sure to edit ``i2c.c`` to set the proper i2c interface. 
This is done in function ``I2C_Open()``, where /dev/i2c-2 is set by default.

To compile install gcc and build-essentials and type:

  make

After that execute the program:

  ./mod-io [param] [address] [data]


1. Set states of the digital outputs on the board

  ./mod-io -relay 0x58 0x01 -> This will turn on only the first relay. 0x58 is the default address.
  ./mod-io -relay 0x58 0x02 -> This will turn on only the second relay.
  ./mod-io -relay 0x58 0x03 -> This will turn on relays one and two.

etc

  ./mod-io -relay 0x58 0x0F -> This will turn on all 4 relays.
  ./mod-io -relay 0x58 0x00 -> This will turn off all relays. 


2. Get the state of the digital inputs of the board. 

  ./mod-io -dig 0x58

3. Get the voltage applied to one of the analogue inputs of the board. 

Be careful with the hardware connections! The first pin of the connector is AVCC = 3.3V!

  ./mod-io -an1 0x58
  ./mod-io -an2 0x58
  ./mod-io -an3 0x58
  ./mod-io -an4 0x58

4. Set new slave address to the board.

  ./mod-io -address 0x58 <new> -> At the moment of sending this command the BTN must be pressed.
