/**************************************************************************/
/*  omake_find.cpp                                                        */
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

#include "omake_find.h"
#include "core/object/script_language.h"
#include "scene/main/node.h"

Node *const *OmakeFind::_get_children_ptr(const Node *p_node, int *p_count, bool p_include_internal) {
	*p_count = p_node->get_child_count();
	int offset = p_include_internal ? 0 : p_node->data.internal_children_front_count_cache;
	Node *const *cptr = p_node->data.children_cache.ptr();
	return cptr + offset;
}

Ref<PackedNodePtrArray> OmakeFind::children(const Node *p_node, const bool p_include_internal) {
	//ERR_THREAD_GUARD_V(nullptr); // FIXME

	int cc;
	Node *const *cptr = _get_children_ptr(p_node, &cc, p_include_internal);

	Ref<PackedNodePtrArray> nodes = memnew(PackedNodePtrArray);
	LocalVector<Node *> *ptr = nodes->get_node_ptr();
	ptr->resize(cc);
	//for (int i = 0; i < cc; i++) {
	//	(*ptr)[i] = cptr[i];
	//}
	memcpy(ptr->ptr(), cptr, cc * sizeof(Node *));

	return nodes;
}

Ref<PackedNodePtrArray> OmakeFind::all(const Node *p_node) {
	return OmakeFind::by(p_node, "*", "", true, false);
}

Ref<PackedNodePtrArray> OmakeFind::by_name(const Node *p_node, const String &p_node_name) {
	return OmakeFind::by(p_node, p_node_name, "", true, false);
}

Ref<PackedNodePtrArray> OmakeFind::by_type(const Node *p_node, const String &p_type_name) {
	return OmakeFind::by(p_node, "*", p_type_name, true, false);
}

Ref<PackedNodePtrArray> OmakeFind::by_group(const Node *p_node, const String &p_group_name) {
	//ERR_THREAD_GUARD_V(TypedArray<Node>()); // FIXME

	LocalVector<Node *> to_search;
	Ref<PackedNodePtrArray> matches = memnew(PackedNodePtrArray);
	to_search.push_back((Node *)p_node);
	while (!to_search.is_empty()) {
		Node *entry = to_search[0];
		to_search.remove_at_unordered(0);

		entry->_update_children_cache();
		Node *const *cptr = entry->data.children_cache.ptr();
		const int ccount = entry->data.children_cache.size();
		for (int i = 0; i < ccount; i++) {
			to_search.push_back(cptr[i]);
		}

		if (entry->is_in_group(p_group_name)) {
			matches->add_node(entry);
		}
	}

	return matches;
}

Ref<PackedNodePtrArray> OmakeFind::by_groups(const Node *p_node, const TypedArray<String> &p_group_names) {
	//ERR_THREAD_GUARD_V(TypedArray<Node>()); // FIXME

	Ref<PackedNodePtrArray> matches = memnew(PackedNodePtrArray);
	LocalVector<Node *> to_search;
	to_search.push_back((Node *)p_node);
	while (!to_search.is_empty()) {
		Node *entry = to_search[0];
		to_search.remove_at_unordered(0);

		entry->_update_children_cache();
		Node *const *cptr = entry->data.children_cache.ptr();
		const int ccount = entry->data.children_cache.size();
		for (int i = 0; i < ccount; i++) {
			to_search.push_back(cptr[i]);
		}

		const int ncount = p_group_names.size();
		for (int i = 0; i < ncount; i++) {
			if (entry->is_in_group(p_group_names[i])) {
				matches->add_node(entry);
			}
		}
	}

	return matches;
}

Ref<PackedNodePtrArray> OmakeFind::by(const Node *p_node, const String &p_pattern, const String &p_type, const bool p_recursive, const bool p_owned) {
	//ERR_THREAD_GUARD; // FIXME
	//ERR_FAIL_COND(p_pattern.is_empty() && p_type.is_empty());

	// Save basic pattern and type info for faster lookup
	bool is_pattern_empty = p_pattern.is_empty();
	bool is_type_empty = p_type.is_empty();
	bool is_type_global_class = !is_type_empty && ScriptServer::is_global_class(p_type);
	String type_global_path = is_type_global_class ? ScriptServer::get_global_class_path(p_type) : "";

	LocalVector<Node *> to_search;
	Ref<PackedNodePtrArray> matches = memnew(PackedNodePtrArray);
	to_search.push_back((Node *)p_node);
	bool is_adding_children = true;
	while (!to_search.is_empty()) {
		// Pop the next entry off the search stack
		Node *entry = to_search[0];
		to_search.remove_at_unordered(0);

		// Add all the children to the list to search
		entry->_update_children_cache();
		if (is_adding_children) {
			Node *const *cptr = entry->data.children_cache.ptr();
			int ccount = entry->data.children_cache.size();
			for (int i = 0; i < ccount; i++) {
				if (p_owned && !cptr[i]->data.owner) {
					continue;
				}

				to_search.push_back(cptr[i]);
			}

			// Stop further child adding if we don't want recursive
			if (!p_recursive) {
				is_adding_children = false;
			}
		}

		// Check if the entry matches
		bool is_pattern_match = is_pattern_empty || entry->data.name.operator String().match(p_pattern);
		bool is_type_match = is_type_empty || entry->is_class(p_type);
		bool is_script_type_match = false;
		if (!is_type_match) {
			if (ScriptInstance *script_inst = entry->get_script_instance()) {
				Ref<Script> scr = script_inst->get_script();
				while (scr.is_valid()) {
					if ((is_type_global_class && type_global_path == scr->get_path()) || p_type == scr->get_path()) {
						is_script_type_match = true;
						break;
					}

					scr = scr->get_base_script();
				}
			}
		}

		// Save it if it matches the pattern and at least one type
		if (is_pattern_match && (is_type_match || is_script_type_match)) {
			matches->add_node(entry);
		}
	}

	return matches;
}
