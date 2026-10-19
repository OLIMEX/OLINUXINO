#!/usr/bin/env python3
"""Read and decode an Olimex OLinuXino board-information EEPROM."""

import argparse
import binascii
import glob
import os
import re
import sys


EEPROM_ADDRESS = 0x50
EEPROM_LENGTH = 256
EEPROM_DATA_LENGTH = 252
SMBUS_BLOCK_LENGTH = 32
EEPROM_MAGIC = b"\x55\xaa\x4c\x4f"

# Source of truth:
# https://github.com/OLIMEX/u-boot-olinuxino/blob/release-20250305/board/olimex/common/boards.c
# release-20250305 is the newest published u-boot-olinuxino release branch as
# of 2026-10-19. Keep the names identical to U-Boot so the tools agree.
BOARD_NAMES = {
    7739: "A20-OLinuXino-LIME",
    7743: "A20-OLinuXino-LIME-n4GB",
    8934: "A20-OLinuXino-LIME-n8G",
    9076: "A20-OLinuXino-LIME-s16MB",
    9160: "A20-OLinuXino-LIME-e4GB",
    9516: "A20-OLinuXino-LIME-e16Gs16M",
    9696: "A20-OLinuXino-LIME-e4Gs16M",
    9211: "T2-OLinuXino-LIME-IND",
    9215: "T2-OLinuXino-LIME-s16MB-IND",
    9219: "T2-OLinuXino-LIME-e4G-IND",
    9734: "T2-OLinuXino-LIME-e4Gs16M-IND",
    10481: "T2-OLinuXino-LIME-e8Gs16M-IND",
    11444: "T2-OLinuXino-LIME-e16Gs16M-IND",
    7701: "A20-OLinuXino-LIME2",
    8340: "A20-OLinuXino-LIME2-e4GB",
    9166: "A20-OLinuXino-LIME2-e16GB",
    7624: "A20-OLinuXino-LIME2-n4GB",
    8910: "A20-OLinuXino-LIME2-n8GB",
    8946: "A20-OLinuXino-LIME2-s16M",
    9604: "A20-OLinuXino-LIME2-e16Gs16M",
    9613: "A20-OLinuXino-LIME2-e4Gs16M",
    9905: "A20-OLinuXino-LIME2-G2",
    9239: "T2-OLinuXino-LIME2-IND",
    9247: "T2-OLinuXino-LIME2-s16M-IND",
    9243: "T2-OLinuXino-LIME2-e8Gs16M-IND",
    11439: "T2-OLinuXino-LIME2-e16Gs16M-IND",
    4614: "A20-OLinuXino-MICRO",
    4615: "A20-OLinuXino-MICRO-n4GB",
    8661: "A20-OLinuXino-MICRO-e4GB-IND",
    8828: "A20-OLinuXino-MICRO-IND",
    8832: "A20-OLinuXino-MICRO-e4GB",
    8918: "A20-OLinuXino-MICRO-n8G",
    9042: "A20-OLinuXino-MICRO-e16G",
    9231: "A20-OLinuXino-MICRO-s16M",
    9684: "A20-OLinuXino-MICRO-e4Gs16M",
    9689: "A20-OLinuXino-MICRO-e16Gs16M",
    9223: "T2-OLinuXino-MICRO-IND",
    9227: "T2-OLinuXino-MICRO-e4G-IND",
    9235: "T2-OLinuXino-MICRO-s16M-IND",
    9739: "T2-OLinuXino-MICRO-e4Gs16M-IND",
    9789: "T2-OLinuXino-MICRO-e8Gs16M-IND",
    11449: "T2-OLinuXino-MICRO-e16Gs16M-IND",
    4673: "A20-SOM-n4GB",
    7664: "A20-SOM",
    8849: "A20-SOM-IND",
    8922: "A20-SOM-n8GB",
    9155: "A20-SOM-e16GB",
    9148: "A20-SOM-e16GB-IND",
    9047: "A20-SOM-e16Gs16M",
    9259: "T2-SOM-IND",
    9827: "T2-SOM-e8Gs16M-IND",
    11454: "T2-SOM-e16Gs16M-IND",
    8991: "A20-SOM204-1G",
    8958: "A20-SOM204-1Gs16Me16G-MC",
    10257: "A20-SOM204-1G-M",
    10157: "T2-SOM204-1Gs16Me4G-C-I",
    10234: "T2-SOM204-1Gs16Me8G-MC-I",
    11458: "T2-SOM204-1Gs16Me16G-M-I",
    11462: "T2-SOM204-1Gs16Me16G-MC-I",
    10238: "T2-SOM204-1G-I",
    8861: "A64-OLinuXino-2Ge8G-IND",
    9065: "A64-OLinuXino-1Gs16M",
    8367: "A64-OLinuXino-1Ge4GW",
    8857: "A64-OLinuXino-1G",
    9849: "A64-OLinuXino-1Ge16GW",
    10728: "A64-OLinuXino-1Ge16GW-EA",
    4432: "A13-OLinuXino",
    4787: "A13-SOM-256",
    4788: "A13-SOM-512",
    10663: "A10-OLinuXino-LIME-e16Gs16M",
    8950: "A10-OLinuXino-LIME-n8GB",
    4746: "A10-OLinuXino-LIME",
    10469: "STMP157-OLinuXino-LIME2H-IND",
    10887: "STMP157-OLinuXino-LIME2H-EXT",
    10997: "STMP157-OLinuXino-LIME2-EXT",
}


class ReaderError(Exception):
    """An expected EEPROM discovery or read failure."""


def _bus_from_path(path):
    match = re.search(r"(?:^|[/\\])(\d+)-0050(?:[/\\]|$)", path)
    return int(match.group(1)) if match else None


def _device_bus_from_path(path):
    match = re.search(r"(?:^|[/\\])i2c-(\d+)$", path)
    return int(match.group(1)) if match else None


def _little_endian_u32(data):
    return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24)


def _validate_eeprom(data):
    if len(data) < EEPROM_LENGTH:
        raise ValueError(
            "EEPROM data is too short: expected {} bytes, got {}".format(
                EEPROM_LENGTH, len(data)
            )
        )
    if data[:4] != EEPROM_MAGIC:
        raise ValueError(
            "invalid EEPROM header: expected {}, got {}".format(
                EEPROM_MAGIC.hex(), data[:4].hex()
            )
        )

    calculated_crc = binascii.crc32(data[:EEPROM_DATA_LENGTH]) & 0xFFFFFFFF
    stored_crc = _little_endian_u32(
        data[EEPROM_DATA_LENGTH:EEPROM_LENGTH]
    )
    if calculated_crc != stored_crc:
        raise ValueError(
            "invalid EEPROM CRC: stored 0x{:08X}, calculated 0x{:08X}".format(
                stored_crc, calculated_crc
            )
        )


def _read_sysfs(path):
    with open(path, "rb") as eeprom_file:
        return eeprom_file.read(EEPROM_LENGTH)


def _load_smbus():
    try:
        from smbus import SMBus
        return SMBus
    except ImportError:
        try:
            from smbus2 import SMBus
            return SMBus
        except ImportError as exc:
            raise ReaderError(
                "The EEPROM is not exposed through sysfs and no SMBus Python "
                "module is installed. On Debian/Ubuntu/Olimage, install it with: "
                "sudo apt-get install python3-smbus"
            ) from exc


def _read_smbus(bus_number, smbus_class):
    bus = smbus_class(bus_number)
    try:
        data = bytearray()
        for offset in range(0, EEPROM_LENGTH, SMBUS_BLOCK_LENGTH):
            block_length = min(SMBUS_BLOCK_LENGTH, EEPROM_LENGTH - offset)
            block = bus.read_i2c_block_data(
                EEPROM_ADDRESS, offset, block_length
            )
            if len(block) != block_length:
                raise IOError(
                    "short EEPROM read at offset {}: expected {} bytes, got {}"
                    .format(offset, block_length, len(block))
                )
            data.extend(block)
        return bytes(data)
    finally:
        close = getattr(bus, "close", None)
        if close is not None:
            close()


def _sysfs_candidates(bus_number=None):
    if bus_number is None:
        paths = glob.glob("/sys/bus/i2c/devices/*-0050/eeprom")
    else:
        paths = [
            "/sys/bus/i2c/devices/{}-0050/eeprom".format(bus_number)
        ]
    return sorted(paths, key=lambda path: (_bus_from_path(path) is None,
                                           _bus_from_path(path) or 0))


def _i2c_bus_candidates(bus_number=None):
    if bus_number is not None:
        return [bus_number]

    buses = []
    for path in glob.glob("/dev/i2c-*"):
        number = _device_bus_from_path(path)
        if number is not None:
            buses.append(number)
    return sorted(set(buses))


def read_eeprom(bus_number=None):
    """Return (256-byte EEPROM data, source description)."""
    failures = []
    valid = []

    for path in _sysfs_candidates(bus_number):
        if not os.path.isfile(path):
            continue
        try:
            data = _read_sysfs(path)
        except OSError as exc:
            failures.append("{}: {}".format(path, exc))
            continue
        try:
            _validate_eeprom(data)
        except ValueError as exc:
            failures.append("{}: {}".format(path, exc))
        else:
            valid.append((data[:EEPROM_LENGTH], path))

    if len(valid) == 1:
        return valid[0]
    if len(valid) > 1:
        buses = ", ".join(str(_bus_from_path(path)) for _, path in valid)
        raise ReaderError(
            "More than one valid Olimex EEPROM was found (I2C buses {}). "
            "Select one with --bus.".format(buses)
        )

    smbus_error = None
    bus_candidates = _i2c_bus_candidates(bus_number)
    if bus_candidates:
        try:
            smbus_class = _load_smbus()
        except ReaderError as exc:
            smbus_class = None
            smbus_error = str(exc)

        if smbus_class is not None:
            for number in bus_candidates:
                try:
                    data = _read_smbus(number, smbus_class)
                except (OSError, IOError) as exc:
                    failures.append(
                        "/dev/i2c-{} address 0x50: {}".format(number, exc)
                    )
                    continue
                try:
                    _validate_eeprom(data)
                except ValueError as exc:
                    failures.append(
                        "/dev/i2c-{} address 0x50: {}".format(number, exc)
                    )
                else:
                    source = "/dev/i2c-{} address 0x50".format(number)
                    valid.append((data[:EEPROM_LENGTH], source))

    if len(valid) == 1:
        return valid[0]
    if len(valid) > 1:
        buses = ", ".join(source for _, source in valid)
        raise ReaderError(
            "More than one valid Olimex EEPROM was found ({}). "
            "Select one with --bus.".format(buses)
        )

    details = failures[:]
    if smbus_error:
        details.append(smbus_error)
    if not details:
        details.append("no /sys EEPROM files or /dev/i2c-* adapters were found")
    raise ReaderError("Unable to read a valid Olimex EEPROM:\n  " + "\n  ".join(details))


def _revision(major_value, minor_value):
    if ord("A") <= major_value <= ord("Z"):
        major = chr(major_value)
    else:
        major = "?"
    minor = chr(minor_value) if ord("1") <= minor_value <= ord("9") else ""
    return major + minor


def _format_capacity(exponent):
    size = 1 << exponent
    for unit, shift in (("GB", 30), ("MB", 20), ("KB", 10)):
        unit_size = 1 << shift
        if size >= unit_size and size % unit_size == 0:
            return "{} {}".format(size // unit_size, unit)
    return "{} B".format(size)


def _format_mac(values):
    characters = []
    for value in values:
        if value == 0xFF:
            characters.append("F")
        elif chr(value) in "0123456789abcdefABCDEF":
            characters.append(chr(value).upper())
        else:
            characters.append("?")
    compact = "".join(characters)
    return ":".join(compact[index:index + 2] for index in range(0, 12, 2))


def decode_eeprom(data):
    _validate_eeprom(data)

    board_id = _little_endian_u32(data[4:8])
    storage_code = data[14]
    storage_names = {
        0x00: "None",
        ord("e"): "eMMC",
        ord("n"): "NAND",
        ord("s"): "SPI Flash",
    }
    storage_name = storage_names.get(
        storage_code, "Unknown type 0x{:02X}".format(storage_code)
    )
    if storage_code != 0x00:
        storage_name += " " + _format_capacity(data[15])

    temperature_names = {
        0: "Commercial grade (0-70 degrees Celsius)",
        1: "Industrial grade (-45 to +85 degrees Celsius)",
    }

    return {
        "board_id": board_id,
        "board_name": BOARD_NAMES.get(board_id, "Unknown board"),
        "revision": _revision(data[8], data[9]),
        "serial": _little_endian_u32(data[10:14]),
        "storage": storage_name,
        "ram": _format_capacity(data[16]),
        "mac": _format_mac(data[18:30]),
        "temperature": temperature_names.get(
            data[17], "Unknown temperature grade ({})".format(data[17])
        ),
    }


def _parse_arguments(argv):
    parser = argparse.ArgumentParser(
        description="Read the Olimex board-information EEPROM."
    )
    parser.add_argument(
        "--bus",
        type=int,
        metavar="N",
        help="read only I2C bus N instead of detecting the bus",
    )
    parser.add_argument(
        "--raw",
        action="store_true",
        help="also print the raw 256 EEPROM bytes",
    )
    parser.add_argument(
        "--list-ids",
        action="store_true",
        help="print the supported board IDs and exit",
    )
    return parser.parse_args(argv)


def main(argv=None):
    args = _parse_arguments(argv)

    if args.list_ids:
        for board_id in sorted(BOARD_NAMES):
            print("{}\t{}".format(board_id, BOARD_NAMES[board_id]))
        return 0

    if args.bus is not None and args.bus < 0:
        print("Error: --bus must be zero or greater.", file=sys.stderr)
        return 2

    try:
        data, source = read_eeprom(args.bus)
        info = decode_eeprom(data)
    except (ReaderError, ValueError) as exc:
        print("Error: {}".format(exc), file=sys.stderr)
        return 1

    print("EEPROM source: {}".format(source))
    if args.raw:
        print("Raw data: {}".format(list(data)))
    print(
        "Board: {} Rev.{}, Serial: {}, ID: {}".format(
            info["board_name"],
            info["revision"],
            hex(info["serial"]),
            info["board_id"],
        )
    )
    print("External memory: {}".format(info["storage"]))
    print("RAM size: {}".format(info["ram"]))
    print("MAC address: {}".format(info["mac"]))
    print(info["temperature"])
    return 0


if __name__ == "__main__":
    sys.exit(main())
