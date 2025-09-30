/**************************************************************************/
/*  packed_node_ptr_array.h                                               */
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
#include "core/variant/typed_array.h"

#include "scene/main/node.h"

class PackedNodePtrArray : public RefCounted {
	GDCLASS(PackedNodePtrArray, RefCounted);

private:
	mutable LocalVector<Node *> nodes;
	uint32_t current_index;

protected:
	static void _bind_methods();

public:
	PackedNodePtrArray();
	~PackedNodePtrArray();

	LocalVector<Node *> *get_node_ptr();
	void add_node(Node *p_node);
	Node *get_node(int p_index) const;
	void set(int p_index, Node *p_node);
	int size() const;
	void resize(int p_new_size);
	void clear();

	Node *front() const;
	Node *back() const;
	Node *pick_random() const;
	TypedArray<Node> to_array() const;
	bool is_empty() const;

	bool _iter_init(const Variant &p_args);
	bool _iter_next(const Variant &p_args);
	Node *_iter_get(const Variant &p_args);
};
