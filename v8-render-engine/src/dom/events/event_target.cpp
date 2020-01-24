#include "dom/nodes/shadow_root.h"

using namespace cpp::dom;
CO_METHOD(EventTargetContextObject::addEventListener, void, v8::Local<v8::String> type, v8::Local<v8::Function> callback, bool capture, bool passive, bool once) {
	auto map = this->eventListenerMap.Get(context->GetIsolate());
	auto maybeList = map->Get(context, type);
	v8::Local<v8::Array> listeners;
	if (maybeList.IsEmpty()) {
		listeners = v8::Array::New(context->GetIsolate(), 1);
		map->Set(context, type, listeners);
	}
	else {
		listeners = maybeList.ToLocalChecked().As<v8::Array>();
	}
	auto fail = false;
	for (int i = listeners->Length(); i > -1; i--) {
		auto val = listeners->Get(context, i);
		if (!val.IsEmpty()) {
			auto listener = val.ToLocalChecked().As<v8::Array>()->Get(context, 0);
			auto cap = val.ToLocalChecked().As<v8::Array>()->Get(context, 1);
			if (cap.ToLocalChecked()->BooleanValue(context->GetIsolate()) == capture && listener.ToLocalChecked() == callback) {
				fail = true;
				break;
			}
		}
	}
	if (!fail) {
		v8::Local<v8::Array> options = v8::Array::New(context->GetIsolate(), 5);
		options->Set(context, 0, callback);
		options->Set(context, 1, v8::Boolean::New(context->GetIsolate(), capture));
		options->Set(context, 2, v8::Boolean::New(context->GetIsolate(), passive));
		options->Set(context, 3, v8::Boolean::New(context->GetIsolate(), once));
		options->Set(context, 4, v8::Boolean::New(context->GetIsolate(), false));
		listeners->Set(context, listeners->Length(), options);
	}
}

CO_METHOD(EventTargetContextObject::removeEventListener, void, v8::Local<v8::String> type, v8::Local<v8::Function> callback, bool capture) {
	auto map = this->eventListenerMap.Get(context->GetIsolate());
	auto maybeList = map->Get(context, type);
	if (!maybeList.IsEmpty()) {
		v8::Local<v8::Array> listeners = maybeList.ToLocalChecked().As<v8::Array>();
		for (int i = listeners->Length(); i > -1; i--) {
			auto val = listeners->Get(context, i);
			if (!val.IsEmpty()) {
				auto options = val.ToLocalChecked().As<v8::Array>();
				auto lst = options->Get(context, 0);
				auto cap = options->Get(context, 1);
				if (lst.ToLocalChecked() == callback && cap.ToLocalChecked()->BooleanValue(context->GetIsolate()) == capture) {
					options->Set(context, 4, v8::Boolean::New(context->GetIsolate(), true));
					auto spliceFunc = listeners->Get(context, v8::String::NewFromUtf8(context->GetIsolate(), "splice", v8::NewStringType::kNormal, 6).ToLocalChecked()).ToLocalChecked();
					v8::Local<v8::Value> argv[2] = { v8::Number::New(context->GetIsolate(), i), v8::Number::New(context->GetIsolate(), 1) };
					spliceFunc.As<v8::Function>()->Call(context, listeners, 2, argv);
					break;
				}
			}
		}
	}
}

bool innerInvoke(v8::Local<v8::Context> context, cpp::pins::Pin<EventContextObject> event, cpp::pins::Pin<EventTargetContextObject> currentTarget, bool bubbling, bool legacyOutputDidListenersThrow) {
	auto found = false;
	auto map = currentTarget->eventListenerMap.Get(context->GetIsolate());
	auto maybeListeners = map->Get(context, event->type_GET(context));
	if (!maybeListeners.IsEmpty()) {
		auto listeners = maybeListeners.ToLocalChecked().As<v8::Array>();
		std::vector<v8::Local<v8::Array>> copy;
		std::vector<v8::Local<v8::Function>> removals;
		copy.reserve(listeners->Length());
		for (uint32_t i = 0; i < listeners->Length(); i++) {
			auto val = listeners->Get(context, i);
			if (!val.IsEmpty()) {
				found = true;
				auto options = val.ToLocalChecked().As<v8::Array>();
				auto cap = options->Get(context, 1);
				if (cap.ToLocalChecked()->BooleanValue(context->GetIsolate()) != bubbling) {
					copy.push_back(options);
				}
			}
		}
		for (int i = 0; i < copy.size(); i++) {
			auto val = copy[i];
			auto listener = val->Get(context, 0).ToLocalChecked().As<v8::Function>();
			auto passive = val->Get(context, 2).ToLocalChecked();
			auto once = val->Get(context, 3).ToLocalChecked();
			auto removed = val->Get(context, 4).ToLocalChecked();
			if (!removed->BooleanValue(context->GetIsolate())) {
				if (once->BooleanValue(context->GetIsolate())) {
					removals.push_back(listener);
				}
				//TODO Globals
				if (passive->BooleanValue(context->GetIsolate())) {
					event->passive(true);
				}
				//TODO call function
				event->passive(false);
				//TODO Globals
				if (event->isImmediateStop()) {
					return found;
				}
			}
		}
	}
	return found;
}

void invoke(v8::Local<v8::Context> context, EventContextObject::PathStruct& pathStruct, EventContextObject::PathStruct& lastNonNull, cpp::pins::Pin<EventContextObject> event, bool bubbling, bool legacyOutputDidListenersThrow) {
	event->setTargets(context, lastNonNull.shadowAdjustedTarget.pin(context->GetIsolate()), pathStruct.relatedTarget.pin(context->GetIsolate()), &pathStruct.touchTargetList);
	if (event->isStopped()) {
		return;
	}
	auto currentTarget = pathStruct.invocationTarget.pin(context->GetIsolate());
	event->setCurrentTarget(context, currentTarget);
	auto found = innerInvoke(context, event, currentTarget, bubbling, legacyOutputDidListenersThrow);
	if (!found && event->isTrusted_GET(context)->BooleanValue(context->GetIsolate())) {
		auto originalEventType = event->type_GET(context);
		v8::Local<v8::String> newType;
		if (originalEventType == v8::String::NewFromUtf8(context->GetIsolate(), "animationend").ToLocalChecked()) {
			newType = v8::String::NewFromUtf8(context->GetIsolate(), "webkitAnimationEnd").ToLocalChecked();
		}
		else if (originalEventType == v8::String::NewFromUtf8(context->GetIsolate(), "animationiteration").ToLocalChecked()) {
			newType = v8::String::NewFromUtf8(context->GetIsolate(), "webkitAnimationIteration").ToLocalChecked();
		}
		else if (originalEventType == v8::String::NewFromUtf8(context->GetIsolate(), "animationstart").ToLocalChecked()) {
			newType = v8::String::NewFromUtf8(context->GetIsolate(), "webkitAnimationStart").ToLocalChecked();
		}
		else if (originalEventType == v8::String::NewFromUtf8(context->GetIsolate(), "transitionend").ToLocalChecked()) {
			newType = v8::String::NewFromUtf8(context->GetIsolate(), "webkitTransitionEnd").ToLocalChecked();
		}
		if (!newType.IsEmpty()) {
			event->setType(context, newType);
			innerInvoke(context, event, currentTarget, bubbling, legacyOutputDidListenersThrow);
			event->setType(context, originalEventType);
		}
	}
}

EventContextObject::PathStruct& appendToEventPath(v8::Local<v8::Context> context, cpp::pins::Pin<EventContextObject> event, cpp::pins::Pin<EventTargetContextObject> invocationTarget, cpp::pins::NullPin<EventTargetContextObject> shadowAdjustedTarget, cpp::pins::NullPin<EventTargetContextObject> relatedTarget, std::vector<cpp::pins::NullProperty<EventTargetContextObject>> touchTargets, bool slotInClosedTree) {
	auto invocationTargetInShadowTree = false;
	if (invocationTarget->domTypeof(DOMType::Node) && invocationTarget.downcast<nodes::NodeContextObject>()->getRootNode_METHOD(context, invocationTarget.downcast<nodes::NodeContextObject>(), false)->domTypeof(DOMType::ShadowRoot)) {
		invocationTargetInShadowTree = true;
	}
	auto rootOfClosedTree = false;
	if (invocationTarget->domTypeof(DOMType::ShadowRoot) && invocationTarget.downcast<nodes::ShadowRootContextObject>()->isClosed()) {
		rootOfClosedTree = true;
	}
	EventContextObject::PathStruct p = { cpp::pins::Property<EventTargetContextObject>(context->GetIsolate(), invocationTarget), invocationTargetInShadowTree, cpp::pins::NullProperty<EventTargetContextObject>(context->GetIsolate(), shadowAdjustedTarget), cpp::pins::NullProperty<EventTargetContextObject>(context->GetIsolate(), relatedTarget), std::move(touchTargets), rootOfClosedTree, slotInClosedTree };
	event->getPath().push_back(std::move(p));
	return event->getPath().back();
}

bool dispatchEvent(v8::Local<v8::Context> context, cpp::pins::Pin<EventContextObject> event, cpp::pins::Pin<EventTargetContextObject> target, bool legacyTargetOverride, bool legacyOutputDidListenersThrow) {
	event->setDispatch(false);
	auto targetOverride = target;//TODO legacyTargetOverride
	auto activationTarget = cpp::pins::NullPin<EventTargetContextObject>::null(context->GetIsolate());
	auto relatedTarget = retarget(context, event->related(context), target);
	if (target != relatedTarget || event->related(context) == target) {
		std::vector<cpp::pins::NullProperty<EventTargetContextObject>> touchTargets;
		if (event->touchTargets()) {
			for (auto& prop : *event->touchTargets()) {
				auto touchTarget = prop.pin(context->GetIsolate());
				touchTargets.emplace_back(context->GetIsolate(), retarget(context, touchTarget, target));
			}
		}
		auto cap_shadows = std::vector<EventContextObject::PathStruct*>();
		auto bub_shadows = std::vector<EventContextObject::PathStruct*>();
		cap_shadows.push_back(&appendToEventPath(context, event, target, targetOverride, relatedTarget, std::move(touchTargets), false));
		//TODO activationEvent
		bool isActivationEvent = false;
		if (isActivationEvent && target->hasActivationBehavior(context, event)) {
			activationTarget = target;
		}
		//TODO slotable
		cpp::pins::NullPin<EventTargetContextObject> slotable = cpp::pins::NullPin<EventTargetContextObject>::null(context->GetIsolate());
		auto slotInClosedTree = false;
		auto parent = target->getTheParent(context, event);
		while (parent) {
			if (slotable) {
				//TODO assert parent is a slot
				slotable.reset(context->GetIsolate());
				auto pinned = parent.pin();
				if (pinned->domTypeof(DOMType::Node)) {
					auto root = pinned.downcast<nodes::NodeContextObject>()->getRootNode_METHOD(context, pinned.downcast<nodes::NodeContextObject>(), false);
					if (root->domTypeof(DOMType::ShadowRoot) && root.downcast<nodes::ShadowRootContextObject>()->isClosed()) {
						slotInClosedTree = true;
					}
				}
			}
			//TODO slots
			slotable = slotable;
			relatedTarget = retarget(context, event->related(context), parent.pin());
			touchTargets = std::vector<cpp::pins::NullProperty<EventTargetContextObject>>();
			if (event->touchTargets()) {
				for (auto& prop : *event->touchTargets()) {
					auto touchTarget = prop.pin(context->GetIsolate());
					touchTargets.emplace_back(context->GetIsolate(), retarget(context, touchTarget, parent.pin()));
				}
			}
			//TODO activation
			if (false) {
				if (isActivationEvent && event->bubbles_GET(context)->BooleanValue(context->GetIsolate()) && !activationTarget && parent.pin()->hasActivationBehavior(context, event)) {
					activationTarget = parent;
				}
				appendToEventPath(context, event, parent.pin(), cpp::pins::NullPin<EventTargetContextObject>::null(context->GetIsolate()), relatedTarget, std::move(touchTargets), slotInClosedTree);
			}
			else if (parent == relatedTarget) {
				parent.reset(context->GetIsolate());
			}
			else {
				target = parent.pin();
				if (isActivationEvent && !activationTarget && target->hasActivationBehavior(context, event)) {
					activationTarget = target;
				}
				cap_shadows.push_back(&appendToEventPath(context, event, parent.pin(), target, relatedTarget, std::move(touchTargets), slotInClosedTree));
			}
			if (parent) {
				parent = parent.pin()->getTheParent(context, event);
			}
			slotInClosedTree = false;
		}
		auto& clearTargetsStruct = *cap_shadows.back();
		//TODO clearTargets
		auto clearTargets = false;
		if (activationTarget) {
			activationTarget.pin()->legacyPreActivationBehavior(context, event);
		}
		bub_shadows.reserve(cap_shadows.size());
		for (auto i = cap_shadows.size(); i > 0; i--) {
			bub_shadows.push_back(cap_shadows[i]);
		}
		delete event->touchTargets();
		event->clearTouches();
		for (auto i = event->getPath().size(); i > 0; i--) {
			auto& val = event->getPath()[i - 1];
			if (val.shadowAdjustedTarget.pin(context->GetIsolate())) {
				event->setPhase(2);
			}
			else {
				event->setPhase(1);
			}
			invoke(context, val, *cap_shadows.back(), event, false, legacyOutputDidListenersThrow);
			if (&val == cap_shadows.back()) {
				cap_shadows.pop_back();
			}
		}
		for (std::size_t i = 0; i < event->getPath().size(); i++) {
			auto& val = event->getPath()[i];
			if (val.shadowAdjustedTarget.pin(context->GetIsolate())) {
				event->setPhase(2);
			}
			else {
				if (!event->bubbles_GET(context)->BooleanValue(context->GetIsolate())) {
					continue;
				}
				event->setPhase(3);
			}
			invoke(context, val, *bub_shadows.back(), event, true, legacyOutputDidListenersThrow);
			if (&val == bub_shadows.back()) {
				bub_shadows.pop_back();
			}
		}
		event->setPhase(0);
		event->setCurrentTarget(context, cpp::pins::NullPin<EventTargetContextObject>::null(context->GetIsolate()));
		event->getPath().clear();
		event->clearFlags();
		if (clearTargets) {
			event->clearTargets(context);
		}
		event->clearTouches();
		if (activationTarget) {
			if (event->isCanceled()) {
				activationTarget.pin()->legacyCanceledActivationBehavior(context, event);
			}
			else {
				activationTarget.pin()->activationBehavior(context, event);
			}
		}
		return event->isCanceled();
	}
}

CO_METHOD(EventTargetContextObject::dispatchEvent, std::optional<bool>, cpp::pins::Pin<EventTargetContextObject> self, cpp::pins::Pin<EventContextObject> event) {
	if (event->dispatched() || !event->isInitialized()) {
		//TODO throw error
		return {};
	}
	event->trust(false);
	return { dispatchEvent(context, event, self, false, false) };
}