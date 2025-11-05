/**************************************************************************/
/*  async_signal.cpp                                                      */
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

#include "async_signal.h"
#include "core/variant/variant_utility.h"
#include "scene/main/node.h"

AsyncSignal *AsyncSignal::singleton = nullptr;

AsyncSignal::AsyncSignal() {
}

AsyncSignal::~AsyncSignal() {
}

void AsyncSignal::initialize_module() {
	CRASH_COND_MSG(singleton != nullptr, "Singleton AsyncSignal already exists");

	ClassDB::register_class<AsyncSignal>();
	singleton = memnew(AsyncSignal);
	Engine::get_singleton()->add_singleton(Engine::Singleton("AsyncSignal", AsyncSignal::get_singleton()));

	//print_line("AsyncSignal created");
}

void AsyncSignal::uninitialize_module() {
	CRASH_COND_MSG(singleton == nullptr, "Singleton AsyncSignal does not exist");

	Engine::get_singleton()->remove_singleton("AsyncSignal");
	memdelete(singleton);
	singleton = nullptr;
	//print_line("AsyncSignal destroyed");
}

void AsyncSignal::_notification(int p_what) {
	//print_line(vformat("!!! called AsyncSignal::_notification with %d", p_what));
	if (p_what == NOTIFICATION_PREDELETE) {
		if (_subscriptions.size() > 0) {
			WARN_PRINT("Warning! Objects not unsubscribed from messages");
		}
		for (const KeyValue<Node *, Vector<Subscription>> &E : _subscriptions) {
			Node *node = E.key;
			if (node != nullptr && VariantUtilityFunctions::is_instance_valid(node)) {
				WARN_PRINT(vformat("node name:%s", node->get_name()));
			}
		}
	}
}

void AsyncSignal::unsubscribe(Node *node) {
	_subscriptions.erase(node);
}

void AsyncSignal::subscribe_to(Node *node, StringName message_type, StringName method_name) {
	if (!_subscriptions.has(node)) {
		_subscriptions[node] = Vector<Subscription>();
	}
	Vector<Subscription> &subs = _subscriptions[node];

	Subscription sub = Subscription();
	sub.message_type = message_type;
	sub.method_name = method_name;
	subs.append(sub);

	//print("'%s' subscribed to '%s'" % [node.name, message_type]);
}

void AsyncSignal::send(StringName message_type, Array args) {
	//print_line(vformat("!!! AsyncSignal::send: %s", message_type));
	call_deferred("_receive", message_type, args);
}

void AsyncSignal::_receive(StringName message_type, Array args) {
	// Find all the node methods to call
	HashMap<Node *, StringName> to_call;
	for (const KeyValue<Node *, Vector<Subscription>> &E : _subscriptions) {
		Node *node = E.key;
		for (const Subscription &sub : E.value) {
			StringName m_type = sub.message_type;
			if (m_type == message_type) {
				to_call[node] = sub.method_name;
			}
		}
	}

	// Call them
	for (const KeyValue<Node *, StringName> &E : to_call) {
		Node *node = E.key;
		if (node != nullptr && VariantUtilityFunctions::is_instance_valid(node)) {
			StringName method_name = E.value;
			node->callv(method_name, args);
		}
	}
}

void AsyncSignal::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_notification"), &AsyncSignal::_notification);
	ClassDB::bind_method(D_METHOD("unsubscribe", "node"), &AsyncSignal::unsubscribe);
	ClassDB::bind_method(D_METHOD("subscribe_to", "node", "message_type", "method_name"), &AsyncSignal::subscribe_to);
	ClassDB::bind_method(D_METHOD("send", "message_type", "args"), &AsyncSignal::send);
	ClassDB::bind_method(D_METHOD("_receive", "message_type", "args"), &AsyncSignal::_receive);

	ClassDB::bind_static_method("AsyncSignal", D_METHOD("get_singleton"), &AsyncSignal::get_singleton);
}
