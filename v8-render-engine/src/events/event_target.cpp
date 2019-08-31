#include "events/event_target.h"
#include "base_v8/allocator.h"
#include "nodes/document.h"
#include "nodes/character_data/character_data.h"
#include "nodes/elements/element.h"
#include "globals/global_access.h"
#include "globals/misc_algorithms.h"

enum EventPhase {
	NONE = 0, CAPTURING_PHASE = 1, AT_TARGET = 2, BUBBLING_PHASE = 3
};

namespace {
	static v8::UniquePersistent<v8::Private> removed;

	auto getRemoved(v8::Isolate* isolate) {
		if (removed.IsEmpty()) {
			auto local = v8::Private::New(isolate, js_objects::allocateString(isolate, "removed"));
			removed.Reset(isolate, local);
			return local;
		}
		else {
			return removed.Get(isolate);
		}
	}
}
void dom::events::EventTargetContextObject::addEventListenerMETHOD(v8::Local<v8::Context> context, v8::Local<v8::String> type, v8::Local<v8::Function> callback, data_structs::EnumSet<EventListenerOptions> options) {
	auto isolate = context->GetIsolate();
	auto& map = this->accessMap(options.contains(EventListenerOptions::capture));
	std::deque<EventHandler>& handlers = map[js_objects::HashableV8String(isolate, type)];
	for (const EventHandler& handler : handlers) {
		if (handler.function == callback) return;
	}
	callback->SetPrivate(context, ::getRemoved(isolate), v8::Boolean::New(isolate, false));
	handlers.emplace_back(context, callback, options);
}

void dom::events::EventTargetContextObject::removeEventListenerMETHOD(v8::Local<v8::Context> context, v8::Local<v8::String> type, v8::Local<v8::Function> callback, bool capture) {
	auto isolate = context->GetIsolate();
	auto& m = this->accessMap(capture);
	auto deque = this->checkMapPresence(capture, js_objects::HashableV8String(isolate, type));
	if (deque) {
		for (auto it = (**deque).begin(); it < (**deque).end(); it++) {
			auto& handler = it->function;
			if (handler == callback) {
				it->function.Get(isolate)->SetPrivate(context, ::getRemoved(isolate), v8::Boolean::New(isolate, true));
				(**deque).erase(it);
				break;
			}
		}
	}
}

v8::Local<v8::Array> retargetTouchTargets(v8::Local<v8::Context> context, std::deque < js_objects::JS_CPP_Property < dom::events::EventTargetContextObject>>& dq, js_objects::Pin<dom::events::EventTargetContextObject> b) {
	auto isolate = context->GetIsolate();
	v8::EscapableHandleScope esh(isolate);
	auto arr = v8::Array::New(isolate, int(dq.size()));
	for (int i = 0; i < dq.size(); i++) {
		arr->Set(context, i, dom::algorithms::retarget(context, dq[i].pin(isolate), b).handle);
	}
	return esh.Escape(arr);
}

bool validateTargetClearing(v8::Local<v8::Context> context, js_objects::Pin<dom::events::EventTargetContextObject> target) {
	if (target->typeof(js_objects::ObjectType::Node)) {
		auto root = target.cast<dom::nodes::NodeContextObject>()->getRoot(context);
		return root->typeof(js_objects::ObjectType::ClosedShadowRoot, js_objects::ObjectType::OpenShadowRoot);
	}
	return false;
}

std::optional<bool> dom::events::EventTargetContextObject::dispatchEventMETHOD(v8::Local<v8::Context> context, js_objects::Pin<dom::events::EventContextObject> evt, bool legacyTargetOverride, bool& legacyOutputDidListenersThrow) {
	auto isolate = context->GetIsolate();
	v8::HandleScope hs(isolate);
	evt->dataAttrs.add(internal_event_flags::dispatch);
	auto target = this->pin<EventTargetContextObject>(isolate);
	auto targetOverride = legacyTargetOverride ? globals::getAssociatedDocument(context).pin().cast<dom::events::EventTargetContextObject>() : target;
	js_objects::NullablePin<EventTargetContextObject> activationTarget;
	auto relatedTarget = algorithms::retarget(context, evt->relatedTarget.pinNullable(isolate).pin(), target);
	auto& evtTouchTargets = evt->internalTouchTargets;
	auto clearTargets = false;
	if (&target != &relatedTarget || target == evt->relatedTarget.pinNullable(isolate)) {
		std::deque<EventPathData*> nonNullStructs;
		auto touchTargets = retargetTouchTargets(context, evtTouchTargets, target);
		nonNullStructs.push_back(&evt->appendToEventPath(context, target, targetOverride, relatedTarget, touchTargets, false));
		auto isActivationEvent = evt->typeof(js_objects::ObjectType::MouseEvent) && evt->type.Get(isolate)->StringEquals(js_objects::allocateString(isolate, "click"));
		if (isActivationEvent && target->activationBehaviors.contains(activation_behavior_properties::activationBehavior)) {
			activationTarget = target;
		}
		auto slotable = algorithms::checkSlotable(context, target);
		auto slotInClosedTree = false;
		auto parent = target->getTheParent(context, evt);
		auto currentTarget = target;
		while (parent) {
			auto pinnedParent = parent.pin();
			if (slotable) {
				if (pinnedParent.cast<nodes::NodeContextObject>()->typeof(js_objects::ObjectType::ClosedShadowRoot)) slotInClosedTree = true;
			}
			slotable = algorithms::checkSlotable(context, pinnedParent);
			auto relatedTarget = algorithms::retarget(context, evt->relatedTarget.pinNullable(isolate).pin(), pinnedParent);
			auto touchTargets = retargetTouchTargets(context, evtTouchTargets, pinnedParent);
			if (pinnedParent->typeof(js_objects::ObjectType::Window) || pinnedParent->typeof(js_objects::ObjectType::Node) && pinnedParent.cast<nodes::NodeContextObject>()->getRoot(context)->privilegedContains(context, pinnedParent.cast<nodes::NodeContextObject>())) {
				if (isActivationEvent && evt->bubblesGET(context) && !activationTarget && pinnedParent->activationBehaviors.contains(activation_behavior_properties::activationBehavior)) {
					activationTarget = parent;
				}
				evt->appendToEventPath(context, pinnedParent, js_objects::NullablePin<EventTargetContextObject>(), relatedTarget, touchTargets, slotInClosedTree);
				nonNullStructs.push_back(nonNullStructs.back());
			}
			else if (parent == relatedTarget) {
				parent = js_objects::NullablePin<EventTargetContextObject>();
				slotInClosedTree = false;
				break;
			}
			else {
				currentTarget = pinnedParent;
				if (isActivationEvent && !activationTarget && currentTarget->activationBehaviors.contains(activation_behavior_properties::activationBehavior)) {
					activationTarget = currentTarget;
				}
				nonNullStructs.push_back(&evt->appendToEventPath(context, pinnedParent, pinnedParent, relatedTarget, touchTargets, slotInClosedTree));
			}
			parent = pinnedParent->getTheParent(context, evt);
			slotInClosedTree = false;
		}
		auto nonNullIt = nonNullStructs.end() - 1;
		auto& clearTargetsStruct = **nonNullIt;
		clearTargets = validateTargetClearing(context, clearTargetsStruct.shadowAdjustedTarget.pinNullable(isolate).pin()) || validateTargetClearing(context, clearTargetsStruct.relatedTarget.pin(isolate));
		if (!clearTargets) {
			auto touchTargets = clearTargetsStruct.touchTargets.Get(isolate);
			for (uint32_t i = 0; i < evtTouchTargets.size(); i++) {
				auto ttarget = v8::Local<v8::Object>::Cast(touchTargets->Get(context, i).ToLocalChecked());
				if (validateTargetClearing(context, static_cast<js_objects::BaseContextObject*>(ttarget->GetAlignedPointerFromInternalField(0))->pin<EventTargetContextObject>(isolate))) {
					clearTargets = true;
					break;
				}
			}
		}
		if (activationTarget && activationTarget.pin()->activationBehaviors.contains(activation_behavior_properties::legacyPreActivationBehavior)) activationTarget.pin()->legacyPreActivationBehavior(context, evt);
		v8::HandleScope hs(isolate);
		for (auto it = evt->path.end(); it-- > evt->path.begin();) {
			evt->eventPhase = it->shadowAdjustedTarget ? EventPhase::AT_TARGET : EventPhase::CAPTURING_PHASE;
			auto ret = it->invoke(context, evt, true, legacyOutputDidListenersThrow, (**nonNullIt--).shadowAdjustedTarget.pinNullable(isolate).pin());
			if (!ret) return {};
		}
		for (auto it = evt->path.begin(); it < evt->path.end(); it++) {
			if (it->shadowAdjustedTarget) {
				evt->eventPhase = EventPhase::AT_TARGET;
			}
			else {
				if (!evt->bubblesGET(context)) continue;
				evt->eventPhase = EventPhase::BUBBLING_PHASE;
			}
			auto ret = it->invoke(context, evt, false, legacyOutputDidListenersThrow, (**++nonNullIt).shadowAdjustedTarget.pinNullable(isolate).pin());
			if (!ret) return {};
		}
	}
	evt->eventPhase = EventPhase::NONE;
	evt->currentTarget = -1;
	evt->path.clear();
	evt->dataAttrs.remove(internal_event_flags::dispatch, internal_event_flags::stop_propagation, internal_event_flags::stop_immediate_propagation);
	if (clearTargets) {
		evt->target.nullify();
		evt->relatedTarget.nullify();
		evt->touchTargetList.Get(isolate)->Set(context, js_objects::allocateString(isolate, "length"), v8::Number::New(isolate, 0));
	}
	if (activationTarget) {
		if (!evt->dataAttrs.contains(internal_event_flags::canceled)) {
			activationTarget.pin()->activationBehavior(context, evt);
		}
		else if (activationTarget.pin()->activationBehaviors.contains(activation_behavior_properties::legacyCanceledActivationBehavior)) {
			activationTarget.pin()->legacyCanceledActivationBehavior(context, evt);
		}
	}
	return { !evt->dataAttrs.contains(internal_event_flags::canceled) };
}

bool checkWindow(v8::Isolate* isolate, v8::Local<v8::Object> obj) {
	return !obj.IsEmpty() && obj->InternalFieldCount() && static_cast<js_objects::BaseContextObject*>(obj->GetAlignedPointerFromInternalField(0))->typeof(js_objects::ObjectType::Window);
}

std::optional<js_objects::VoidSuccess> dom::events::EventPathData::invoke(v8::Local<v8::Context> context, js_objects::Pin<EventContextObject> evt, bool capturing, bool& legacyOutputDidListenersThrow, js_objects::Pin<dom::events::EventTargetContextObject> target) {
	auto isolate = context->GetIsolate();
	auto listeners = this->invocationTarget->checkMapPresence(capturing, js_objects::HashableV8String(isolate, evt->typeGET(context)));
	if (listeners) {
		if (evt->dataAttrs.contains(internal_event_flags::stop_propagation)) return { js_objects::VoidSuccess::SUCCESS };
		evt->target = js_objects::Nullable_JS_CPP_Property(isolate, target);
		evt->relatedTarget = this->relatedTarget.toNullable(isolate);
		evt->touchTargetList.Reset(isolate, this->touchTargets);
		std::remove_reference<decltype(**listeners)>::type listenerCopy;
		for (const auto& listener : **listeners) {
			listenerCopy.push_back(listener.copy(isolate));
		}
		auto ret = this->innerInvoke(context, evt, listenerCopy, legacyOutputDidListenersThrow, **listeners);
		if (ret) {
			if (!*ret && evt->isTrustedGET(context)) {
				v8::UniquePersistent<v8::String> originalEventType = std::move(evt->type);
				auto localType = originalEventType.Get(isolate);
				auto exception = false;
				if (localType->StringEquals(js_objects::allocateString(isolate, "animationend"))) {
					evt->type = v8::UniquePersistent<v8::String>(isolate, js_objects::allocateString(isolate, "webkitAnimationEnd"));
					exception = !this->innerInvoke(context, evt, listenerCopy, legacyOutputDidListenersThrow, **listeners);
				}
				else if (localType->StringEquals(js_objects::allocateString(isolate, "animationiteration"))) {
					evt->type = v8::UniquePersistent<v8::String>(isolate, js_objects::allocateString(isolate, "webkitAnimationIteration"));
					exception = !this->innerInvoke(context, evt, listenerCopy, legacyOutputDidListenersThrow, **listeners);
				}
				else if (localType->StringEquals(js_objects::allocateString(isolate, "animationstart"))) {
					evt->type = v8::UniquePersistent<v8::String>(isolate, js_objects::allocateString(isolate, "webkitAnimationStart"));
					exception = !this->innerInvoke(context, evt, listenerCopy, legacyOutputDidListenersThrow, **listeners);
				}
				else if (localType->StringEquals(js_objects::allocateString(isolate, "transitionend"))) {
					evt->type = v8::UniquePersistent<v8::String>(isolate, js_objects::allocateString(isolate, "webkitTransitionEnd"));
					exception = !this->innerInvoke(context, evt, listenerCopy, legacyOutputDidListenersThrow, **listeners);
				}
				evt->type = std::move(originalEventType);
				if (exception) return {};
				else return { js_objects::VoidSuccess::SUCCESS };
			}
			else return { js_objects::VoidSuccess::SUCCESS };
		}
	}
	else {
		return { js_objects::VoidSuccess::SUCCESS };
	}
}

std::optional<bool> dom::events::EventPathData::innerInvoke(v8::Local<v8::Context> context, js_objects::Pin<EventContextObject> evt, std::deque<EventHandler>& handlers, bool& legacyOutputDidListenersThrow, std::deque<EventHandler>& removeFrom)
{
	auto isolate = context->GetIsolate();
	auto found = false;
	for (const auto& listener : handlers) {
		auto func = listener.function.Get(isolate);
		if (auto maybe = func->GetPrivate(context, ::getRemoved(isolate)); maybe.IsEmpty() || !maybe.ToLocalChecked()->BooleanValue(isolate)) {
			found = true;
			if (listener.options.contains(EventListenerOptions::once)) {
				for (auto it = removeFrom.begin(); it < removeFrom.end(); it++) {
					if (*it == listener) {
						func->SetPrivate(context, ::getRemoved(isolate), v8::Boolean::New(isolate, true));
						removeFrom.erase(it);
						break;
					}
				}
			}
			//TODO: window/globals
			if (listener.options.contains(EventListenerOptions::passive)) {
				evt->dataAttrs.add(internal_event_flags::in_passive_listener);
			}
			auto bound = func->GetBoundFunction();
			if (bound->IsUndefined()) {
				//TODO: call func with window global
			}
			else {
				auto result = v8::Local<v8::Function>::Cast(bound)->Call(context, v8::Undefined(isolate), 1, &evt.handle.As<v8::Value>());
				legacyOutputDidListenersThrow = true;
				if (result.IsEmpty()) return {};
			}
			evt->dataAttrs.remove(internal_event_flags::in_passive_listener);
			//TODO: window/globals
			if (evt->dataAttrs.contains(internal_event_flags::stop_immediate_propagation)) return { found };
		}
	}
	return { found };
}