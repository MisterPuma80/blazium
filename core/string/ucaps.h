/**************************************************************************/
/*  ucaps.h                                                               */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "core/string/ucaps_tables.h"
#include <cstdint>

static constexpr size_t hash(uint32_t key) {
	return (key ^ (key >> 16)) % UCAPS_TABLES_CAPACITY;
}

static constexpr uint32_t caps_table_get(const uint32_t key) {
	const size_t idx = hash(key);
	size_t current = idx;
	for (size_t i = 1; i < UCAPS_TABLES_CAPACITY; ++i) {
		if (caps_table[current].key == key) {
			return caps_table[current].value;
		}
		if (caps_table[current].key == 0xFFFFFFFF) {
			break;
		}
		current = (current + i) & (UCAPS_TABLES_CAPACITY - 1);
	}

	return key;
}

static constexpr uint32_t reverse_caps_table_get(const uint32_t key) {
	const size_t idx = hash(key);
	size_t current = idx;
	for (size_t i = 1; i < UCAPS_TABLES_CAPACITY; ++i) {
		if (reverse_caps_table[current].key == key) {
			return reverse_caps_table[current].value;
		}
		if (reverse_caps_table[current].key == 0xFFFFFFFF) {
			break;
		}
		current = (current + i) & (UCAPS_TABLES_CAPACITY - 1);
	}

	return key;
}

static constexpr int _find_upper(const int ch) {
	return caps_table_get(ch);
}

static constexpr int _find_lower(const int ch) {
	return reverse_caps_table_get(ch);
}
