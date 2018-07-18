# Information about the hardware schematics, design sources, and revision changes

PDF exports of the schematics are found in this folder.
Design files are found in the sub-folders.
Each folder features all required files for a specific hardware revision as indicated by the folder name.
To access the design files you need the latest beta version of KiCAD.
Using the latest stable version usually will NOT suffice.

The initial release of A64-OLinuXino boards was revision C.
Starting hardware revision D, each variant of A64-OLinuXino has its own schematic.
The three variants of the board so far are:

- A64-OLinuXino-1G0G has 1GB RAM, no extras (no flash memory, no WIFI/BLE module)
- A64-OLinuXino-1G4GW has 1GB RAM memory, 4GB eMMC flash memory, extra WIFI/BLE module
- A64-OLinuXino-2G16G-IND has 2GB RAM, 16GB extra flash but NO WiFi/BLE module; with components that work in the industrial -40+85C temperature range

## Hardware revision changes log:

### Hardware revision C

- initial release

### Hardware revision D

- Adjusted white print of label C184.
- Added bypass R66 NA(0R) near FET2.
- Added white print for the UART connector at the bottom of the board (Tx,Rx,GND).
- Fixed white print of R106 and R116 (labels were swapped)
- R29 placed a bit further from C84
- 1.5v test pad name was changed to "DDR-VCC" because the RAM memory can work at 1.3V.
- Names of the jumpers lineinr/micin1p1 and lineinl/micin1n1 were changed to lineinr/micin1 and lineinl/micin2.
- The jumper phoneoutr(l)/lineootr(l) was renamed to hphoneoutr(l)/lineoutr(l).
