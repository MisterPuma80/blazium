/**************************************************************************/
/*  omake.h                                                               */
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

#pragma once

#include "core/object/ref_counted.h"
#include "packed_node_ptr_array.h"

class Omake : public RefCounted {
	GDCLASS(Omake, RefCounted);

protected:
	static void _bind_methods();

public:
	Omake();
	~Omake();

	static uint64_t get_cpu_ticks_nsec();
	static int64_t add_clampedi(int64_t a, int64_t b, int64_t min = INT64_MIN, int64_t max = INT64_MAX);

	static Ref<PackedNodePtrArray> get_children(const Node *p_node, const bool p_include_internal = true);

	static Ref<PackedNodePtrArray> find_all(const Node *p_node);
	static Ref<PackedNodePtrArray> find_by(const Node *p_node, const String &p_pattern, const String &p_type, const bool p_recursive = true, const bool p_owned = true);
	static Ref<PackedNodePtrArray> find_by_name(const Node *p_node, const String &p_node_name);
	static Ref<PackedNodePtrArray> find_by_type(const Node *p_node, const String &p_type_name);
	static Ref<PackedNodePtrArray> find_by_group(const Node *p_node, const String &p_group_name);
	static Ref<PackedNodePtrArray> find_by_groups(const Node *p_node, const TypedArray<String> &p_group_names);
	static PackedStringArray get_groups(const Node *p_node);
};
