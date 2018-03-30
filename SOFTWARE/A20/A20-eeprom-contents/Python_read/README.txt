This program reads the EEPROM contents of Olimex A20 boards (written during production) and prints the informaton in a human-friendly way. The EEPROM stores some information about your board (ID, produciton number, MAC address, temperature grade). The software had been tested and works with A20-OLinuXino-Micro, A20-OLinuXino-LIME, A20-OLinuXino-LIME2 and their variants (with external flash memory).

You should use the latest official image suitable for your board (each board has own wiki article and own official image, found at the wiki article of the board). When you prepare a card and boot, login as super user (by default username is: "root"; password is "olimex"); then make sure to establish connection to Internet successfully (either via Ethernet jack or USB module). 

Then install the smbus package for Python (if it's not installed by default). To do so type the command:

sudo apt-get install python-smbus

When it is installed you have to download the Python program in the same folder. You can do it with this command:

wget https://github.com/OLIMEX/OLINUXINO/raw/master/SOFTWARE/A20/A20-eeprom-contents/Python_read/brd_info.py

Then execute it with:

./brd_info.py

If the above returns error about permissions expand its permissions with:

chmod +x brd_info.py

Then again run the python program:

./brd_info.py

As a result you will see the information stored in the EEPROM of your board.

For example:
Board: A20-OLinuXino-LIME2-n8GB Rev.G2, Serial: 0x1494, ID: 8910
External memory: NAND 8 GB
RAM size = 1 GB
MAC address: 30:1F:9A:D0:06:10
Commercial grade

Stanimir Petev, Olimex
2018/03/30
