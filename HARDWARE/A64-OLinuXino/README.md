## Information about the hardware schematics, design sources, and revision changes

PDF exports of the schematics are found in this folder. Deisgn files are found in the folders, each folder features all required files for a specific hardware revision as indicated by the folder name. To access the design files you need the latest beta version of KiCAD. Using the latest stable version usually will NOT suffice.

The initial release of A64-OLinuXino boards was revision C. Starting hardware revision D, each variant of A64-OLinuXino has own schematic. The three variants of the board so far are:



* A64-OLinuXino-1G has 1GB RAM, no extras (no flash memory, no WIFI/BLE module)

* A64-OLinuXino-1G16GW 

* A64-OLinuXino-1G4GW has 1GB RAM memory, 4GB eMMC flash memory, extra WIFI/BLE module

* A64-OLinuXino-2G16G-IND has 2GB RAM, 16GB extra flash but NO WiFi/BLE module; with components that work in the industrial -40+85C temperature range

### Hardware revision changes log:

#### Hardware revision C

* initial release

#### Hardware revision D

* 1. Adjusted white print of label C184.
* 2. Added bypass R66 NA(0R) near FET2.
* 3. Added white print for the UART connector at the bottom of the board (Tx,Rx,GND).
* 4. Fixed white print of R106 and R116 (labels were swapped)
* 5. R29 placed a bit further from C84
* 6. 1.5v test pad name was changed to "DDR-VCC" because the RAM memory can work at 1.3V.
* 7. Names of the jumpers lineinr/micin1p1 and lineinl/micin1n1 were changed to lineinr/micin1 and lineinl/micin2.
* 8. The jumper phoneoutr(l)/lineootr(l) was renamed to hphoneoutr(l)/lineoutr(l).

#### Hardware revision E

* 1. Changed the eMMC from eMMC (4GB,1.00mm pitch) to eMMC (16GB,0.50mm pitch). Some of the surrounding components changed too.
** 1.1 Removed RM1(RA1206_(4X0603)_4B8_10k), RM2(RA0805_(4X0402)_22R), RM3(RA0805_(4X0402)_22R), RM4(RA1206_(4X0603)_4B8_10k), RM5(RA0805_(4X0402)_22R)
* 2. C212 and C213 were changed from 27pF to 33pF and from 0603 to 0402 as component optimisation.
* 3. R66 value changed from NA(0R) to NA(22R)
* 4. RM15's package changed from RA1206_(4X0603)_4B8_22R to RA0805_(4X0402)_22R as component optimisation.
* 5. C119 and VR1 changed from PTH to SMT type in the KiCad settings. In reallity, C119 and VR1 did not change. 
* 6. PWRLED1 and CHGLED1 labels corrected in the white print.
* 7. WP_Enable1 jumper added to the SPI_Flash.
* 8. U4 changed from NA(H27UBG8T2BTR-BC(TSOP48)) to NA(MT29F64G08CBABA-WP(TSOP48)), i.e. when populated 8GB NAND instead of 4GB.
* 9. MICRO_SD(TFC-WPAPR-08) package change - corrected cream layer and origin.
* 10. PWR_PC1 jumper state changed to Soldered(2-3); Opened(1-2) so that 1.8V wire is connected to the eMMC by default.
* 11. Serial resistors added to the Ethernet PHY's Tx lines for better EMI supression and RLC filters to the clock lines.
* 12. C198 connected to the right side of the RGMII_RXCLK line.
* 13. The Wi-Fi's baloun now matched with the same "Pi" - filter like in Teres-I, so:
	C105 changed from 33pF to 10pF
	C106 changed from NA to 0.8pF
	R62 was exchanged with inductor 3.3nH
	C107 was changed from NA to 1.5pF
* 14. The WiFi's antenna mirrored


