## Information about the hardware schematics, design sources, and revision changes

PDF exports of the schematics are found in this folder. Deisgn files are found in the folders, each folder features all required files for a specific hardware revision as indicated by the folder name. To access the design files you need the latest beta version of KiCAD. Using the latest stable version usually will NOT suffice.

The initial release of A64-OLinuXino boards was revision C. The six variants of the board so far are:

* A64-OLinuXino-1G with 1GB RAM but NO flash and NO WiFi/BLE connectivity

* A64-OLinuXino-1Ge16GW with 1GB RAM and 16GB extra eMMC flash and extra WiFi/BLE module

* A64-OLinuXino-1Ge16GW-EA with 1GB RAM and 16GB extra eMMC flash and extra WiFi/BLE module + external antenna

* A64-OLinuXino-1Ge4GW with 1GB RAM and 4GB extra eMMC flash and extra WiFi/BLE module

* A64-OLinuXino-1Gs16M with 1GB RAM, no NAND or eMMC flash, no WiFi/BLE, extra 16MB SPI flash

* A64-OLinuXino-2Ge8G-IND with 2GB RAM and 8GB extra flash but NO WiFi/BLE connectivity; with components that work in the industrial -40+85C temperature range

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

#### Hardware revision E1

* 1. eMMC KLMAG2GEND-B031(FBGA153)(16G) changed with new eMMC KLMAG1JETD-B041(FBGA153)(16G)

#### Hardware revision F

* 1. Added R62(10k/0402) pull-down to the eMMC's data strobe line;
* 2. RM14, RA0805_(4X0402)_22R replaced by 4 resistors 22R/0402, i.e. R121-R124; R121 was placed away from UEXT for to make shorter eMMC's data strobe line length;
* 3. R73[NA(22R/0402)] was added to enable SPI0 software CS option to the UEXT via PH10, when the SPI_Flash is populated. By defalut, it will reamin as it was, i.e. R124(22R/0402) populated!, while R73(NA)!;
* 4. L9 was changed from 2.7nH/L0402 to NA(2.7nH/L0402), and C88 from 24pF to NA(24pF);
* 5. MIPI-DSI1 changed: MIPI-DSI1 connector's pinout completely changed for Raspberry-Pi compatability; added the resistors: R125 to R132;
* 6. In the DRC drill to drill clerance was set to 12mils and all related errors were corrected;
* 7. UEXT1 was changed from NA(HN2x5) to HN2x5;
* 8. All 3Ds were assigned;
* 9. It was added C94(NA/0402) to the eMMC's clock line just in case;
* 10. NAND and related components removed forever;
* 11. In the schematic all "/" signs were exchanged with "\";
* 12. Improvements around the placement of a number of resistors related to the PHY signals to reduce EMI emissions;
* 13. Added U4 EEPROM - AT24C16C-SSHM-T(SOIC-8_150mil) and also C39, 100nF/10V/10% to the UEXT's I2C to avoid conflict with the same EEPROM on the LCD-Driver board. It is placed by default;
* 14. R15[NA(22R/R0402)] added in serial to the eMMC's datastrobe signal to be able to disconnect when no HS400 mode or to add a serial filter to reduce EMI;
* 15. UBOOT1, PWRON1 and RESET1 packages changed from OLIMEX_Buttons-FP:uTACTIL to OLIMEX_Buttons-FP:IT1185AU2_V2;
* 16. U2 and U3's packages improved;
* 17. LAN1's package improved;
* 18. D1 moved a little bit away from the PWR-JACK's + pad;
* 19. Q2 package changed from Q32.768kHz\12.5pF/20ppm/4P/SMD8038 to Q32.768kHz/12.5pF/2P/SMD1206;
* 20. L7 package changed from 2.7nH\L0402 to NA(2.7nH\L0402), and C198 changed from 24pF to NA(24pF);
* 21. R100 changed from 22R/0402 to FB1, FB0402/PZ1005U601-R45TF-1;
* 22. R69 changed from 22R/0402 to FB2, FB0402/PZ1005U601-R45TF-1;
* 23. The GND polygons in the top and bottom opened to allow for metal cover soldering.

#### Hardware revision G

1. Added antenna ANT2(NA(U.FL-R-SMT-1)), R16(0R) and R17(NA/0R);
2. Removed FB1(FB0402/PZ1005U601-R45TF-1) and FB2(FB0402/PZ1005U601-R45TF-1) and placed R18(22R/0402) and R19(22R/0402) resistors instead. Using FB1 and FB2 was meant to improve the EMI, but it yielded to results.
3. Adjusted the lengths of some eMMC wires.
4. C93 changed from 24pF to 24pF\50V/5%. C88 and C198 changed from NA/24pF to NA(24pF\50V/5%);
5. Placing 5.1 eMMC to work at 3.3V to work more reliably, PWR_PC1 has to be set to position 1-2 for 3.3V operation.

