Datasheets of components which will be used in A13-OLinuXino

A13		CortexA8 MCU
RTL8188CU	WiFi 802.11n 150Mbit WIFI module
GL850G		USB hub 1:4
H5TQ2G63BFR-H9	2Gbit x16 data bus 1333Mhz DDR3 RAM

PLEASE BEWARE: ON A13 Reference Schematic we found trap - DDR RAM memory balls are shifted with +1, so according to Hynix datasheet A0 is ball K3 but on the schematic is K4 and so on.
We don't know if this is intentional or non-intentional, but will fix it in our own schematic.
