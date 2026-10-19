#!/usr/bin/env python3
import sys
import subprocess
import importlib.util
import platform
import os

def get_linux_distro():
    """Identifies if the system is Debian/Ubuntu or AlmaLinux/RHEL."""
    try:
        # Read the OS identification file
        with open("/etc/os-release", "r") as f:
            os_info = f.read().lower()
            
        if "ubuntu" in os_info or "debian" in os_info:
            return "debian"
        elif "almalinux" in os_info or "rhel" in os_info or "centos" in os_info:
            return "almalinux"
    except FileNotFoundError:
        pass
    return None

def install_system_package(package_name, distro):
    """Runs the correct system package manager command using sudo."""
    if distro == "debian":
        print(f"System identified as Debian/Ubuntu. Using APT to install {package_name}...")
        # Update package lists first, then install
        subprocess.check_call(["sudo", "apt-get", "update", "-y"])
        subprocess.check_call(["sudo", "apt-get", "install", "-y", package_name])
        
    elif distro == "almalinux":
        print(f"System identified as AlmaLinux/RHEL. Using DNF to install {package_name}...")
        # Install directly using DNF
        subprocess.check_call(["sudo", "dnf", "install", "-y", package_name])

def import_or_install_system(package_name, import_name=None):
    """
    Checks for a Python module. 
    If missing, detects the Linux OS and installs it via apt or dnf.
    """
    if import_name is None:
        import_name = package_name

    # Step 1: Check if the module is already available
    spec = importlib.util.find_spec(import_name)
    if spec is not None:
        print(f"Success: '{import_name}' is already installed.")
        return

    print(f"'{import_name}' not found. Attempting system installation...")

    # Step 2: Detect the Linux distribution
    distro = get_linux_distro()
    if not distro:
        print("Error: Unsupported OS. This script requires Debian, Ubuntu, or AlmaLinux.")
        sys.exit(1)

    # Step 3: Map the module to the OS package format (usually python3-modulename)
    # Example: 'requests' becomes 'python3-requests'
    os_package_name = f"python3-{package_name}"

    try:
        # Step 4: Install using the native package manager
        install_system_package(os_package_name, distro)
        
        # Step 5: Verify the installation worked
        spec = importlib.util.find_spec(import_name)
        if spec is not None:
            print(f"Success: '{package_name}' was successfully installed via system manager.")
        else:
            raise ImportError("Module still not importable after installation.")
            
    except subprocess.CalledProcessError as e:
        print(f"Error: Failed to install package. (Command exited with error code {e.returncode})")
        print("Make sure this script is run with sudo permissions or as a root user.")
        sys.exit(1)
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        sys.exit(1)

# --- Example Usage ---
# We will use 'requests' as an example, which maps to 'python3-requests' on both platforms.
import_or_install_system("smbus")

# Now you can safely import it
import smbus
import subprocess

# Determine whether to use I2C bus 1 or bus 2 based on the active driver directory
driver_path = "/sys/bus/i2c/drivers/at24/"
if os.path.exists(driver_path) and "1-0050" in os.listdir(driver_path):
    device_id = "1-0050"
    bus_number = 1
else:
    device_id = "2-0050"
    bus_number = 2

print(f"Detected active configuration. Using device {device_id} on I2C bus {bus_number}.")

# Runs the command completely silently to unbind
subprocess.run(f'echo "{device_id}" | sudo tee /sys/bus/i2c/drivers/at24/unbind', shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

bus = smbus.SMBus(bus_number)    # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1

block = bus.read_i2c_block_data(0x50, 0, 32)
print("Raw data: {}".format(block))
Header = (block[3] << 24) | (block[2] << 16) | (block[1] << 8) | block[0]
if (Header!=0x4f4caa55):
  print("Invalid EEPROM data")
  print("Header = {}; should be 0x4f4caa55".format(hex(Header)))
  # Re-bind before exit to avoid leaving the driver broken
  subprocess.run(f'echo "{device_id}" | sudo tee /sys/bus/i2c/drivers/at24/bind', shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
  exit()

BoardID = (block[7] << 24) | (block[6] << 16) | (block[5] << 8) | block[4]

BoardSerial = (block[13] << 24) | (block[12] << 16) | (block[11] << 8) | block[10]

BoardName = "Unknown Board"
# --- A20-OLinuXino-LIME Variants ---
if (BoardID==7739): BoardName = "A20-OLinuXino-LIME"
if (BoardID==7743): BoardName = "A20-OLinuXino-LIME-n4GB"
if (BoardID==8934): BoardName = "A20-OLinuXino-LIME-n8GB"
if (BoardID==9076): BoardName = "A20-OLinuXino-LIME-s16MB"
if (BoardID==9160): BoardName = "A20-OLinuXino-LIME-e4GB"
if (BoardID==9516): BoardName = "A20-OLinuXino-LIME-e16Gs16M"
if (BoardID==9696): BoardName = "A20-OLinuXino-LIME-e4Gs16M"

# --- T2-OLinuXino-LIME Industrial Variants ---
if (BoardID==9211): BoardName = "T2-OLinuXino-LIME-IND"
if (BoardID==9215): BoardName = "T2-OLinuXino-LIME-s16MB-IND"
if (BoardID==9219): BoardName = "T2-OLinuXino-LIME-e4GB-IND"
if (BoardID==9734): BoardName = "T2-OLinuXino-LIME-e4Gs16M-IND"
if (BoardID==10481): BoardName = "T2-OLinuXino-LIME-e8Gs16M-IND"
if (BoardID==11444): BoardName = "T2-OLinuXino-LIME-e16Gs16M-IND"

# --- A20-OLinuXino-LIME2 Variants ---
if (BoardID==7701): BoardName = "A20-OLinuXino-LIME2"
if (BoardID==7624): BoardName = "A20-OLinuXino-LIME2-n4GB"
if (BoardID==8910): BoardName = "A20-OLinuXino-LIME2-n8GB"
if (BoardID==8340): BoardName = "A20-OLinuXino-LIME2-e4GB"
if (BoardID==9166): BoardName = "A20-OLinuXino-LIME2-e16GB"
if (BoardID==9604): BoardName = "A20-OLinuXino-LIME2-e16Gs16M"
if (BoardID==9613): BoardName = "A20-OLinuXino-LIME2-e4Gs16M"
if (BoardID==9905): BoardName = "A20-OLinuXino-LIME2-G2"

# --- T2-OLinuXino-LIME2 Industrial Variants ---
if (BoardID==9223): BoardName = "T2-OLinuXino-LIME2-IND"
if (BoardID==9227): BoardName = "T2-OLinuXino-LIME2-s16MB-IND"
if (BoardID==9231): BoardName = "T2-OLinuXino-LIME2-e4GB-IND"
if (BoardID==9239): BoardName = "T2-OLinuXino-LIME2-IND"
if (BoardID==9247): BoardName = "T2-OLinuXino-LIME2-s16M-IND"
if (BoardID==9243): BoardName = "T2-OLinuXino-LIME2-e8Gs16M-IND"
if (BoardID==9538): BoardName = "T2-OLinuXino-LIME2-e16Gs16M-IND"
if (BoardID==11439): BoardName = "T2-OLinuXino-LIME2-e16Gs16M-IND"

# --- A20-OLinuXino-MICRO Variants ---
if (BoardID==4614): BoardName = "A20-OLinuXino-MICRO"
if (BoardID==4615): BoardName = "A20-OLinuXino-MICRO-n4GB"
if (BoardID==9042): BoardName = "A20-OLinuXino-MICRO-e4GB"
if (BoardID==8828): BoardName = "A20-OLinuXino-MICRO-IND"
if (BoardID==8661): BoardName = "A20-OLinuXino-MICRO-e4GB-IND"
if (BoardID==9231): BoardName = "A20-OLinuXino-MICRO-s16M"
if (BoardID==9684): BoardName = "A20-OLinuXino-MICRO-e4Gs16M"
if (BoardID==9689): BoardName = "A20-OLinuXino-MICRO-e16Gs16M"

# --- T2-OLinuXino-MICRO Industrial Variants ---
if (BoardID==9223): BoardName = "T2-OLinuXino-MICRO-IND"
if (BoardID==9227): BoardName = "T2-OLinuXino-MICRO-e4G-IND"
if (BoardID==9235): BoardName = "T2-OLinuXino-MICRO-s16M-IND"
if (BoardID==9739): BoardName = "T2-OLinuXino-MICRO-e4Gs16M-IND"
if (BoardID==9789): BoardName = "T2-OLinuXino-MICRO-e8Gs16M-IND"
if (BoardID==11449): BoardName = "T2-OLinuXino-MICRO-e16Gs16M-IND"

# --- A20-SOM (System on Module) Variants ---
if (BoardID==6317): BoardName = "A20-SOM"
if (BoardID==6515): BoardName = "A20-SOM-n4GB"
if (BoardID==8811): BoardName = "A20-SOM-e4GB"
if (BoardID==9100): BoardName = "A20-SOM-IND"
if (BoardID==8824): BoardName = "A20-SOM-e4GB-IND"
if (BoardID==4673): BoardName = "A20-SOM-n4GB"
if (BoardID==7664): BoardName = "A20-SOM"
if (BoardID==8849): BoardName = "A20-SOM-IND"
if (BoardID==8922): BoardName = "A20-SOM-n8GB"
if (BoardID==9155): BoardName = "A20-SOM-e16GB"
if (BoardID==9148): BoardName = "A20-SOM-e16GB-IND"
if (BoardID==9047): BoardName = "A20-SOM-e16Gs16M"

# --- T2-SOM Industrial Variants ---
if (BoardID==9259): BoardName = "T2-SOM-IND"
if (BoardID==9827): BoardName = "T2-SOM-e8Gs16M-IND"
if (BoardID==11454): BoardName = "T2-SOM-e16Gs16M-IND"

# --- A20-SOM204 Variants ---
if (BoardID==8958): BoardName = "A20-SOM204"
if (BoardID==8991): BoardName = "A20-SOM204-1G-e4GB"
if (BoardID==10257): BoardName = "A20-SOM204-IND"
if (BoardID==8958): BoardName = "A20-SOM204-1Gs16Me16G-MC"
if (BoardID==10257): BoardName = "A20-SOM204-1G-M"

# --- T2-SOM204 Industrial Variants ---
if (BoardID==10157): BoardName = "T2-SOM204-1Gs16Me4G-C-I"
if (BoardID==10234): BoardName = "T2-SOM204-1Gs16Me8G-MC-I"
if (BoardID==11458): BoardName = "T2-SOM204-1Gs16Me16G-M-I"
if (BoardID==11462): BoardName = "T2-SOM204-1Gs16Me16G-MC-I"
if (BoardID==10238): BoardName = "T2-SOM204-1G-I"

# --- A64-OLinuXino Variants ---
if (BoardID==8861): BoardName = "A64-OLinuXino-2Ge8G-IND"
if (BoardID==9065): BoardName = "A64-OLinuXino-1Gs16M"
if (BoardID==8367): BoardName = "A64-OLinuXino-1Ge4GW"
if (BoardID==8857): BoardName = "A64-OLinuXino-1G"
if (BoardID==9849): BoardName = "A64-OLinuXino-1Ge16GW"
if (BoardID==10728): BoardName = "A64-OLinuXino-1Ge16GW-EA"

# --- A13-OLinuXino / SOM Variants ---
if (BoardID==4432): BoardName = "A13-OLinuXino"
if (BoardID==4787): BoardName = "A13-SOM-256"
if (BoardID==4788): BoardName = "A13-SOM-512"

# --- A10-OLinuXino Variants ---
if (BoardID==10663): BoardName = "A10-OLinuXino-LIME-e16Gs16M"
if (BoardID==8950): BoardName = "A10-OLinuXino-LIME-n8GB"
if (BoardID==4746): BoardName = "A10-OLinuXino-LIME"

# --- STMP157-OLinuXino Variants ---
if (BoardID==10469): BoardName = "STMP157-OLinuXino-LIME2H-IND"
if (BoardID==10887): BoardName = "STMP157-OLinuXino-LIME2H-EXT"
if (BoardID==10997): BoardName = "STMP157-OLinuXino-LIME2-EXT"

# --- Additional LIME2 Specialized Variants ---
if (BoardID==8978): BoardName = "A20-OLinuXino-Lime2-Light-e4GB"
if (BoardID==8946): BoardName = "A20-OLinuXino-LIME2-s16MB"

# --- Additional MICRO Storage Variants ---
if (BoardID==8832): BoardName = "A20-OLinuXino-MICRO-e4GB"
if (BoardID==8918): BoardName = "A20-OLinuXino-MICRO-n8GB"

# --- Additional T2 Industrial grade Variants ---
if (BoardID==11439): BoardName = "T2-OLinuXino-LIME2-e16GB-IND"

# Safely convert the minor revision to a string, ignoring 255 (blank)
rev_minor = chr(block[9]) if block[9] != 255 else ""

print("Board: {} Rev.{}{}, Serial: {}, ID: {}".format(BoardName, chr(block[8]), rev_minor, hex(BoardSerial), BoardID))

if (block[14] == 0x00):
  print("External memory: None")
else:
  Type = ""
  if (block[14] == 0x65):  #0x65 = 'e'
    Type = "eMMC"
  if (block[14] == 0x6E):  #0x6E = 'n'
    Type = "NAND"
  if (block[14] == 0x73):  #0x73 = 's'
    Type = "SPI Flash"
  if (block[15] >= 30):
    Size = 1<<(block[15]-30)
    SizeMeasure = "GB"
  else:
    if (block[15] >= 20):
      Size = 1<<(block[15]-20)
      SizeMeasure = "MB"
    else:
      if (block[15] >= 10):
        Size = 1<<(block[15]-10)
        SizeMeasure = "KB"
      else:
        Size = 1<<block[15]
        SizeMeasure = "B"
  print("External memory: {} {} {}".format(Type, Size, SizeMeasure))

if (block[16] >= 30):
  print("RAM size = {} GB".format(1<<(block[16]-30)))
else:
  if (block[16] >= 20):
    print("RAM size = {} MB".format(1<<(block[16]-20)))
  else:
    if (block[16] >= 10):
      print("RAM size = {} KB".format(1<<(block[16]-10)))
    else:
      print("RAM size = {} B".format(1<<block[16]))

print("MAC address: {}{}:{}{}:{}{}:{}{}:{}{}:{}{}".format(chr(block[18]), chr(block[19]), chr(block[20]), chr(block[21]), chr(block[22]), chr(block[23]), chr(block[24]), chr(block[25]), chr(block[26]), chr(block[27]), chr(block[28]), chr(block[29])))


if (block[17] == 1):
  print("Industrial grade (-45+85) degrees Celsius")
else:
  print("Commercial grade (0-70) degrees Celsius")

# Runs the command completely silently to re-bind
subprocess.run(f'echo "{device_id}" | sudo tee /sys/bus/i2c/drivers/at24/bind', shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
