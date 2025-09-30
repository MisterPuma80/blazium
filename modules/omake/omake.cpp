/**************************************************************************/
/*  omake.cpp                                                             */
/**************************************************************************/
/*                         This file is part of:                          */
/*                            BLAZIUM ENGINE                              */
/*                          https://blazium.app                           */
/**************************************************************************/
/* Copyright (c) 2024-present Blazium Engine contributors.                */
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

#include "omake.h"
#include "core/object/script_language.h"

#include "omake_find.h"
#include "omake_get_cpu_ticks_nsec.h"
#include "packed_node_ptr_array.h"
#include <cstdint>

Omake::Omake() {
	//print_line("Omake created");
}

Omake::~Omake() {
	//print_line("Omake destroyed");
}

uint64_t Omake::get_cpu_ticks_nsec() {
	return omake_get_cpu_ticks_nsec();
}

int64_t Omake::add_clampedi(int64_t a, int64_t b, int64_t min, int64_t max) {
	// Will overflow so just return max
	if (b > 0 && a > max - b) {
		return max;
	}

	// Will underflow so just return min
	if (b < 0 && a < min - b) {
		return min;
	}

	return CLAMP(a + b, min, max);
}

Ref<PackedNodePtrArray> Omake::get_children(const Node *p_node, const bool p_include_internal) {
	return OmakeFind::children(p_node, p_include_internal);
}

Ref<PackedNodePtrArray> Omake::find_all(const Node *p_node) {
	return OmakeFind::all(p_node);
}

Ref<PackedNodePtrArray> Omake::find_by_name(const Node *p_node, const String &p_node_name) {
	return OmakeFind::by_name(p_node, p_node_name);
}

Ref<PackedNodePtrArray> Omake::find_by_type(const Node *p_node, const String &p_type_name) {
	return OmakeFind::by_type(p_node, p_type_name);
}

Ref<PackedNodePtrArray> Omake::find_by_group(const Node *p_node, const String &p_group_name) {
	return OmakeFind::by_group(p_node, p_group_name);
}

Ref<PackedNodePtrArray> Omake::find_by_groups(const Node *p_node, const TypedArray<String> &p_group_names) {
	return OmakeFind::by_groups(p_node, p_group_names);
}

Ref<PackedNodePtrArray> Omake::find_by(const Node *p_node, const String &p_pattern, const String &p_type, const bool p_recursive, const bool p_owned) {
	return OmakeFind::by(p_node, p_pattern, p_type, p_recursive, p_owned);
}

void Omake::_bind_methods() {
	ClassDB::bind_static_method("Omake", D_METHOD("get_cpu_ticks_nsec"), &Omake::get_cpu_ticks_nsec);
	ClassDB::bind_static_method("Omake", D_METHOD("add_clampedi", "a", "b", "min", "max"), &Omake::add_clampedi, DEFVAL(INT64_MIN), DEFVAL(INT64_MAX));

	ClassDB::bind_static_method("Omake", D_METHOD("get_children", "node", "include_internal"), &Omake::get_children, DEFVAL(true));

	ClassDB::bind_static_method("Omake", D_METHOD("find_all", "node"), &Omake::find_all);
	ClassDB::bind_static_method("Omake", D_METHOD("find_by", "node", "pattern", "type", "recursive", "owned"), &Omake::find_by, DEFVAL(""), DEFVAL(true), DEFVAL(true));
	ClassDB::bind_static_method("Omake", D_METHOD("find_by_name", "node", "node_name"), &Omake::find_by_name);
	ClassDB::bind_static_method("Omake", D_METHOD("find_by_type", "node", "type_name"), &Omake::find_by_type);
	ClassDB::bind_static_method("Omake", D_METHOD("find_by_group", "node", "group_name"), &Omake::find_by_group);
	ClassDB::bind_static_method("Omake", D_METHOD("find_by_groups", "node", "group_names"), &Omake::find_by_groups);
}
