#include "events/event.h"
#include "events/event_target.h"
#include "nodes/node.h"

v8::Local<v8::Array> dom::events::EventContextObject::composedPathMETHOD(v8::Local<v8::Context> context) {
	auto isolate = context->GetIsolate();
	auto composedPath = v8::Array::New(isolate, int(this->path.size()));
	int composedPathIndex = 0;
	auto& path = this->path;
	if (!path.size()) return { composedPath };
	auto& curTarget = *this->currentTargetGET(context).cppRef;
	composedPath->Set(context, composedPathIndex++, curTarget.toLocal(isolate));
	std::size_t currentTargetIndex(this->currentTarget);
	std::size_t currentTargetHiddenSubtreeLevel(0);
	std::size_t index(this->path.size() - 1);
	while (index > currentTargetIndex) {
		if (path[index].rootOfClosedTree) currentTargetHiddenSubtreeLevel++;
		if (path[index].slotInClosedTree) currentTargetHiddenSubtreeLevel--;
		index--;
	}
	if (path[index].rootOfClosedTree) currentTargetHiddenSubtreeLevel++;
	std::size_t currentHiddenLevel, maxHiddenLevel = currentHiddenLevel = currentTargetHiddenSubtreeLevel;
	index = currentTargetIndex - 1;
	while (index > -1) {
		if (path[index].rootOfClosedTree) currentHiddenLevel++;
		if (currentHiddenLevel <= maxHiddenLevel) composedPath->Set(context, composedPathIndex++, path[index].invocationTarget.jsRef.Get(isolate));
		if (path[index].slotInClosedTree) {
			currentHiddenLevel--;
			if (currentHiddenLevel < maxHiddenLevel) maxHiddenLevel = currentHiddenLevel;
		}
		index--;
	}
	for (int i = 0; i < composedPathIndex / 2; i++) {
		auto localTemp = composedPath->Get(context, uint32_t(i)).ToLocalChecked();
		composedPath->Set(context, i, composedPath->Get(context, composedPathIndex - uint32_t(i)).ToLocalChecked());
		composedPath->Set(context, composedPathIndex - i, localTemp);
	}
	currentHiddenLevel = maxHiddenLevel = currentTargetHiddenSubtreeLevel;
	index = currentTargetIndex + 1;
	while (index < path.size()) {
		if (path[index].slotInClosedTree) currentHiddenLevel++;
		if (currentHiddenLevel <= maxHiddenLevel) composedPath->Set(context, composedPathIndex++, path[index].invocationTarget.jsRef.Get(isolate));
		if (path[index].rootOfClosedTree) {
			currentHiddenLevel--;
			if (currentHiddenLevel < maxHiddenLevel) maxHiddenLevel = currentHiddenLevel;
		}
		index++;
	}
	return { composedPath };
}

dom::events::EventPathData& dom::events::EventContextObject::appendToEventPath(v8::Local<v8::Context> context, js_objects::Pin<EventTargetContextObject> invocationTarget, js_objects::NullablePin<EventTargetContextObject> shadowAdjustedTarget, js_objects::Pin<EventTargetContextObject> relatedTarget, v8::Local<v8::Array> touchTargets, bool slotInClosedTree)
{
	bool invocationTargetInShadowTree = false;
	bool rootOfClosedTree = false;
	if (invocationTarget->typeof(js_objects::ObjectType::Node)) {
		auto root = invocationTarget.cast<nodes::NodeContextObject>()->getRoot(context);
		if (root->typeof(js_objects::ObjectType::ClosedShadowRoot) || root->typeof(js_objects::ObjectType::OpenShadowRoot)) {
			invocationTargetInShadowTree = true;
		}
		if (root->typeof(js_objects::ObjectType::ClosedShadowRoot)) {
			rootOfClosedTree = true;
		}
	}
	return this->path.emplace_back(context->GetIsolate(), invocationTarget, invocationTargetInShadowTree, shadowAdjustedTarget, relatedTarget, touchTargets, rootOfClosedTree, slotInClosedTree);
}

void dom::events::EventContextObject::initEventMETHOD(v8::Local<v8::Context> context, v8::Local<v8::String> type, bool bubbles, bool cancelable) {
	auto isolate = context->GetIsolate();
	this->dataAttrs.add(internal_event_flags::initialized);
	this->dataAttrs.remove(internal_event_flags::canceled, internal_event_flags::stop_propagation, internal_event_flags::stop_immediate_propagation, internal_event_flags::trusted);
	this->target.nullify();
	this->type.Reset(isolate, type);
	this->dataAttrs.toggle(bubbles, internal_event_flags::bubbles);
	this->dataAttrs.toggle(cancelable, internal_event_flags::cancelable);
}