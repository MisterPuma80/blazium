/**************************************************************************/
/*  omake_find.h                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                            BLAZIUM ENGINE                              */
/*                          https://blazium.app                           */
/**************************************************************************/
/* Copyright (c) 2024-present Blazium Engine contributors.                */
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

#include "core/object/ref_counted.h"
#include "core/variant/typed_array.h"

#include "modules/omake/packed_node_ptr_array.h"

class Node;

class OmakeFind {

private:
    static Node *const *_get_children_ptr(const Node *p_node, int *p_count, bool p_include_internal = false);

public:
	static Ref<PackedNodePtrArray> children(const Node *p_node, const bool p_include_internal = true);
	static Ref<PackedNodePtrArray> all(const Node *p_node);
	static Ref<PackedNodePtrArray> by(const Node *p_node, const String &p_pattern, const String &p_type, const bool p_recursive = true, const bool p_owned = true);
	static Ref<PackedNodePtrArray> by_name(const Node *p_node, const String &p_node_name);
	static Ref<PackedNodePtrArray> by_type(const Node *p_node, const String &p_type_name);
	static Ref<PackedNodePtrArray> by_group(const Node *p_node, const String &p_group_name);
	static Ref<PackedNodePtrArray> by_groups(const Node *p_node, const TypedArray<String> &p_group_names);
};
