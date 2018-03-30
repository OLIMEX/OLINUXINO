This program has been tested and works for A20-OLinuXino-Micro, A20-OLinuXino-LIME, A20-OLinuXino-LIME2 and their variations of external memory.

You must have the latest image for one of the boards mentioned above. When you login (by default username is: "root"; password is "olimex") first you must install the smbus package for python (if it's not installed already). To do so on the command line input this:
sudo apt-get install python-smbus
When it is installed you have to download the python program:
wget <link>
If this file is not executable you must make it executable:
chmod +x brd_info.py

Then run the python program:
./brd_info.py

As a result you will see the information about your board.

For example:
Board: A20-OLinuXino-LIME2-n8GB Rev.G2, Serial: 0x1494, ID: 8910
External memory: NAND 8 GB
RAM size = 1 GB
MAC address: 30:1F:9A:D0:06:10
Commercial grade

Stanimir Petev, Olimex
2018/03/30
