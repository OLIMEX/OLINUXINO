Rev.A (early proto)

RT5350 rev.B (production)
=========================
1. Add external power 1.2V to decrease heating from the internal LDO
2. Add connector for nused signals and Flash memory
3. Add Jumper for 1.2V to switch source
4. Pull-up to LDOSEL
5. Add connector for external antenna
6. Add CS1 from RT5350 to memory
7. remap LAN connector signals 

Rev.C (production)
==================
1. Add PWR LED to 3.3V
2. Z1 -> DDZ9678-7(SOD-123)_Farnell_PN-1858631 was add to protect the MCU in case of DCDC failure/short-circuite between the Input and Output.
3. Add 5.6V zener to +5V for 1.2V DCDC protection
4. Crystal changed from 20ppm to 10 ppm E4SB20.000F20E15(20MHz/10pF/10ppm/5x3.2mm)
5. C49 change from NA to 6.8 pF, C50 change from 27pF to 22pF. 

Rev.D (production)
==================
1. Add oscilator 20MHz option.
2. Add pull up to EPHY_LED0_N 4.7k(R42), R7 change to NA, as sometimes at boot RAM size is not recognized correctly 

Rev.E (internal)
================
1. crystal and oscillator moved away
2. R42 pull up to EPHY_LED2_N change from 4.7K to NA
3. R9 change from NA to 4.7K
4. Updated Flash memory and SOC packages.

