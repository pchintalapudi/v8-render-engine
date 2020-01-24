#include "dom/nodes/shadow_root.h"
#include "dom/nodes/element.h"
using namespace cpp::dom;

v8::Local<v8::Array> EventContextObject::composedPath_METHOD(v8::Local<v8::Context> context) {
	v8::Local<v8::Array> composedPath = v8::Array::New(context->GetIsolate(), static_cast<int>(this->path.size()));
	if (path.empty()) {
		return composedPath;
	}
	auto idx = 0;
	auto currentTarget = this->currentTarget.pin(context->GetIsolate());
	//See later for current target append
	auto currentTargetIndex = 0;
	auto currentTargetHiddenSubtreeLevel = 0;
	int index = static_cast<int>(this->path.size()) - 1;
	while (index >= 0) {
		auto& val = this->path[idx];
		if (val.rootOfClosedTree) {
			currentTargetHiddenSubtreeLevel++;
		}
		if (val.invocationTarget.pin(context->GetIsolate()) == currentTarget) {
			currentTargetIndex = index;
			break;
		}
		if (val.slotInClosedTree) {
			currentTargetHiddenSubtreeLevel--;
		}
		index--;
	}
	auto currentHiddenLevel = currentTargetHiddenSubtreeLevel;
	auto maxHiddenLevel = currentTargetHiddenSubtreeLevel;
	index = currentTargetIndex - 1;
	std::vector<pins::Pin<EventTargetContextObject>> prepends;
	prepends.reserve(index);
	while (index >= 0) {
		auto& val = this->path[idx];
		if (val.rootOfClosedTree) {
			currentHiddenLevel++;
		}
		if (currentHiddenLevel <= maxHiddenLevel) {
			prepends.push_back(val.invocationTarget.pin(context->GetIsolate()));
		}
		if (val.slotInClosedTree) {
			currentHiddenLevel--;
			if (currentHiddenLevel < maxHiddenLevel) {
				maxHiddenLevel = currentHiddenLevel;
			}
		}
		index--;
	}
	for (auto i = prepends.size(); i > 0; i--) {
		composedPath->Set(context, idx++, prepends.operator[](i).js_handle());
	}
	composedPath->Set(context, idx++, currentTarget.js_handle());
	currentHiddenLevel = maxHiddenLevel = currentTargetHiddenSubtreeLevel;
	index = currentTargetIndex + 1;
	while (index < path.size()) {
		auto& val = this->path[idx];
		if (val.slotInClosedTree) {
			currentHiddenLevel++;
		}
		if (currentHiddenLevel <= maxHiddenLevel) {
			composedPath->Set(context, idx++, val.invocationTarget.pin(context->GetIsolate()).js_handle());
		}
		if (val.rootOfClosedTree) {
			currentHiddenLevel--;
			if (currentHiddenLevel < maxHiddenLevel) {
				maxHiddenLevel = currentHiddenLevel;
			}
		}
		index++;
	}
	return composedPath;
}
CO_METHOD(EventContextObject::initEvent, void, v8::Local<v8::String> type, bool bubbles, bool cancelable) {
	if (!this->dispatch) {
		this->initialized = true;
		this->stopPropagation = false;
		this->stopImmediatePropagation = false;
		this->canceled = false;
		this->isTrusted = false;
		this->target.reset(context->GetIsolate());
		this->type.Reset(context->GetIsolate(), type);
		this->bubbles = bubbles;
		this->cancelable = cancelable;
	}
}

cpp::pins::NullPin<EventTargetContextObject> cpp::dom::retarget(v8::Local<v8::Context> context, pins::NullPin<EventTargetContextObject> a, pins::Pin<EventTargetContextObject> b)
{
	auto bnode = b->domTypeof(DOMType::Node);
	while (a && a.pin()->domTypeof(DOMType::Node)) {
		auto pinned = a.pin().downcast<nodes::NodeContextObject>();
		auto root = pinned->getRootNode_METHOD(context, pinned, false);
		if (!root->domTypeof(DOMType::ShadowRoot)) {
			return a;
		}
		if (bnode && b.downcast<nodes::NodeContextObject>()->commonAncestor(context, b.downcast<nodes::NodeContextObject>(), pinned) == a) {
			return a;
		}
		a = root.downcast<nodes::ShadowRootContextObject>()->host_GET(context).cast<EventTargetContextObject>();
	}
	return a;
}