#!/usr/bin/python

import smbus;

bus = smbus.SMBus(1);    # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1

block = bus.read_i2c_block_data(0x50, 0, 32);
#print ("Raw data: {}").format(block);
Header = (block[3] << 24) | (block[2] << 16) | (block[1] << 8) | block[0];
if (Header!=0x4f4caa55):
  print ("Invalid EEPROM data");
  print ("Header = {}; should be 0x4f4caa55").format(hex(Header));
  exit();

BoardID = (block[7] << 24) | (block[6] << 16) | (block[5] << 8) | block[4];

BoardSerial = (block[13] << 24) | (block[12] << 16) | (block[11] << 8) | block[10];

if (BoardID==7739): BoardName = "A20-OLinuXino-LIME";
if (BoardID==7743): BoardName = "A20-OLinuXino-LIME-n4GB";
if (BoardID==8934): BoardName = "A20-OLinuXino-LIME-n8GB";
if (BoardID==9076): BoardName = "A20-OLinuXino-LIME-s16MB";
if (BoardID==7701): BoardName = "A20-OLinuXino-LIME2";
if (BoardID==8340): BoardName = "A20-OLinuXino-LIME2-e4GB";
if (BoardID==8978): BoardName = "A20-OLinuXino-Lime2-Light-e4GB";
if (BoardID==7624): BoardName = "A20-OLinuXino-LIME2-n4GB";
if (BoardID==8910): BoardName = "A20-OLinuXino-LIME2-n8GB";
if (BoardID==8946): BoardName = "A20-OLinuXino-LIME2-s16MB";
if (BoardID==4614): BoardName = "A20-OLinuXino-MICRO";
if (BoardID==9042): BoardName = "A20-OLinuXino-MICRO-e16GB";
if (BoardID==8832): BoardName = "A20-OLinuXino-MICRO-e4GB";
if (BoardID==8661): BoardName = "A20-OLinuXino-MICRO-e4GB-IND";
if (BoardID==8828): BoardName = "A20-OLinuXino-MICRO-IND";
if (BoardID==4615): BoardName = "A20-OLinuXino-MICRO-n4GB";
if (BoardID==8918): BoardName = "A20-OLinuXino-MICRO-n8GB";
if (BoardID==8991): BoardName = "A20-SOM204-1G";
if (BoardID==8958): BoardName = "A20-SOM204-1Gs16Me16G-MC";

if (block[9] == 255):
  block[9] = 0;

print ("Board: {} Rev.{}{}, Serial: {}, ID: {}").format(BoardName, chr(block[8]), chr(block[9]), hex(BoardSerial), BoardID);

if (block[14] == 0x00):
  print ("External memory: None");
else:
  Type = "";
  if (block[14] == 0x65):  #0x65 = 'e'
    Type = "eMMC";
  if (block[14] == 0x6E):  #0x6E = 'n'
    Type = "NAND";
  if (block[14] == 0x73):  #0x73 = 's'
    Type = "SPI Flash"
  if (block[15] >= 30):
    Size = 1<<(block[15]-30);
    SizeMeasure = "GB";
  else:
    if (block[15] >= 20):
      Size = 1<<(block[15]-20);
      SizeMeasure = "MB";
    else:
      if (block[15] >= 10):
        Size = 1<<(block[15]-10);
        SizeMeasure = "KB";
      else:
        Size = 1<<block[15];
        SizeMeasure = "B";
  print ("External memory: {} {} {}").format(Type, Size, SizeMeasure);

if (block[16] >= 30):
  print ("RAM size = {} GB").format(1<<(block[16]-30));
else:
  if (block[16] >= 20):
    print ("RAM size = {} MB").format(1<<(block[16]-20));
  else:
    if (block[16] >= 10):
      print ("RAM size = {} KB").format(1<<(block[16]-10));
    else:
      print ("RAM size = {} B").format(1<<block[16]);

print ("MAC address: {}{}:{}{}:{}{}:{}{}:{}{}:{}{}").format(chr(block[18]), chr(block[19]), chr(block[20]), chr(block[21]), chr(block[22]), chr(block[23]), chr(block[24]), chr(block[25]), chr(block[26]), chr(block[27]), chr(block[28]), chr(block[29]));


if (block[17] == 1):
  print ("Industrial grade");
else:
  print ("Commercial grade");
