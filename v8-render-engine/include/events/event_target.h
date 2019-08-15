#pragma once

#include <memory>
#include <map>

#include "event.h"

namespace dom {
	namespace events {

		enum class EventListenerOptions {
			capture, passive, once
		};

		struct EventHandler {
			v8::UniquePersistent<v8::Function> function;
			bool passive;
		};

		class EventTargetContextObject : public js_objects::BaseContextObject {
		public:

			CO_METHOD(addEventListener, void, v8::Local<v8::String> type, v8::Local<v8::Function> callback, data_structs::EnumSet<EventListenerOptions> options);

			CO_METHOD(removeEventListener, void, v8::Local<v8::String> type, v8::Local<v8::Function> callback, bool capture);

			CO_METHOD(dispatchEvent, js_objects::CPPThrowablePrimitive<bool>, EventContextObject* obj);

			virtual EventTargetContextObject* getTheParent(EventContextObject& evt) {
				return nullptr;
			}

			virtual void activationBehavior(EventContextObject& evt) {}

		private:
			std::unique_ptr<std::map<std::string, EventHandler>> bubblers;
			std::unique_ptr<std::map<std::string, EventHandler>> capturers;
			std::size_t passive_count;
			bool hasActivationBehavior;
		};
	}
}