#pragma once

#include <optional>
#include "dom/dom_core.h"
#include "dom/events/event.h"

namespace cpp {
	namespace dom {
		class EventTargetContextObject :public DOMContextObject {
		public:
			CO_METHOD(addEventListener, void, v8::Local<v8::String> type, v8::Local<v8::Function> callback, bool capture, bool passive, bool once);

			CO_METHOD(removeEventListener, void, v8::Local<v8::String> type, v8::Local<v8::Function> callback, bool capture);

			CO_METHOD(dispatchEvent, std::optional<bool>, pins::Pin<EventTargetContextObject> self, pins::Pin<EventContextObject> event);

			virtual pins::NullPin<EventTargetContextObject> getTheParent(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event) { return pins::NullPin<EventTargetContextObject>::null(context->GetIsolate()); }

			virtual bool hasActivationBehavior(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event) { return false; }
			virtual void activationBehavior(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event) {}

			virtual void legacyPreActivationBehavior(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event) {}
			virtual void legacyCanceledActivationBehavior(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event) {}
			v8::UniquePersistent<v8::Map> eventListenerMap;
		};
	}
}