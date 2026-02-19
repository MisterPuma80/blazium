#!/usr/bin/env python3

# Script used to dump case mappings from
# the Unicode Character Database to the `ucaps_tables.h` file.
# NOTE: This script is deliberately not integrated into the build system;
# you should run it manually whenever you want to update the data.

import os
import sys
from ctypes import c_uint32
from typing import Final, List, Tuple
from urllib.request import urlopen

if __name__ == "__main__":
    sys.path.insert(1, os.path.join(os.path.dirname(__file__), "../../"))

from methods import generate_copyright_header

URL: Final[str] = "https://www.unicode.org/Public/16.0.0/ucd/UnicodeData.txt"


lower_to_upper: List[Tuple[str, str]] = []
upper_to_lower: List[Tuple[str, str]] = []


def parse_unicode_data() -> None:
    lines: List[str] = [line.decode("utf-8") for line in urlopen(URL)]

    for line in lines:
        split_line: List[str] = line.split(";")

        code_value: str = split_line[0].strip()
        uppercase_mapping: str = split_line[12].strip()
        lowercase_mapping: str = split_line[13].strip()

        if uppercase_mapping:
            lower_to_upper.append((f"0x{code_value}", f"0x{uppercase_mapping}"))
        if lowercase_mapping:
            upper_to_lower.append((f"0x{code_value}", f"0x{lowercase_mapping}"))


class IntEntry:
    key: c_uint32 = c_uint32(0xFFFFFFFF)
    value: c_uint32 = c_uint32(0)

    def __init__(self, key: c_uint32, value: c_uint32):
        self.key = key
        self.value = value


class IntHashTableMaker:
    data: List[IntEntry] = []
    capacity: int = 0

    def __init__(self, capacity: int):
        self.capacity = capacity

    def make(self, in_data: List[Tuple[str, str]]):
        self.data.clear()
        for i in range(self.capacity):
            self.data.append(IntEntry(c_uint32(0xFFFFFFFF), c_uint32(0)))

        for first, second in in_data:
            self.insert(c_uint32(int(first, 16)), c_uint32(int(second, 16)))

    def _hash(self, key: c_uint32) -> int:
        return (key.value ^ (key.value >> 16)) % self.capacity

    def insert(self, key: c_uint32, value: c_uint32) -> bool:
        idx: int = self._hash(key)
        for i in range(self.capacity):
            current: int = (idx + i) % self.capacity
            if self.data[current].key.value == 0xFFFFFFFF or self.data[current].key.value == key.value:
                self.data[current] = IntEntry(key, value)
                return True

        return False


def generate_ucaps_fetch() -> None:
    parse_unicode_data()

    source: str = generate_copyright_header("ucaps_tables.h")

    source += """
#pragma once

// This file was generated using the `misc/scripts/ucaps_fetch.py` script.

"""

    # Set hash table capacity as 2x data size rounded up to power of 2
    capacity: int = 4096
    source += f"static const int UCAPS_TABLES_CAPACITY = {capacity};\n\n"

    source += """
struct Entry {
\tuint32_t key = 0xFFFFFFFF;
\tuint32_t value = 0;
};

"""

    # Make reverse_caps_table
    reverse_caps_table_maker = IntHashTableMaker(capacity)
    reverse_caps_table_maker.make(upper_to_lower)
    source += "static const Entry reverse_caps_table[UCAPS_TABLES_CAPACITY] = {\n"
    for entry in reverse_caps_table_maker.data:
        source += f"\t{{ {entry.key.value:#X}, {entry.value.value:#X} }},\n"
    source += "};\n\n"

    # Make caps_table
    caps_table_maker = IntHashTableMaker(capacity)
    caps_table_maker.make(lower_to_upper)
    source += "static const Entry caps_table[UCAPS_TABLES_CAPACITY] = {\n"
    for entry in caps_table_maker.data:
        source += f"\t{{ {entry.key.value:#X}, {entry.value.value:#X} }},\n"
    source += "};\n\n"

    # Write to file
    ucaps_path: str = os.path.join(os.path.dirname(__file__), "../../core/string/ucaps_tables.h")
    with open(ucaps_path, "w", newline="\n") as f:
        f.write(source)

    print("`ucaps_tables.h` generated successfully.")


if __name__ == "__main__":
    generate_ucaps_fetch()
