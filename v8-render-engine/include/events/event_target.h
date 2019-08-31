#pragma once

#include <memory>
#include <map>
#include <set>

#include "event.h"

namespace dom {
	namespace events {

		enum class EventListenerOptions {
			capture, passive, once, __COUNT__
		};

		struct EventHandler {
			v8::UniquePersistent<v8::Function> function;
			data_structs::EnumSet<EventListenerOptions> options;

			EventHandler(v8::Local<v8::Context> context, v8::Local<v8::Function> function, data_structs::EnumSet<EventListenerOptions> options) : function(context->GetIsolate(), function), options(options) {}

			bool operator==(const EventHandler& other) const {
				return this->function == other.function;
			}

			EventHandler copy(v8::Isolate* isolate) const {
				auto handler = EventHandler(isolate, function.Get(isolate), options);
			}

		private:

			EventHandler(v8::Isolate* isolate, v8::Local<v8::Function> function, data_structs::EnumSet<EventListenerOptions> options) : function(isolate, function), options(options) {}
		};

		class EventTargetContextObject : public js_objects::BaseContextObject {
		public:

			CO_METHOD(addEventListener, void, v8::Local<v8::String> type, v8::Local<v8::Function> callback, data_structs::EnumSet<EventListenerOptions> options);

			CO_METHOD(removeEventListener, void, v8::Local<v8::String> type, v8::Local<v8::Function> callback, bool capture);

			CO_METHOD(dispatchEvent, std::optional<bool>, js_objects::Pin<EventContextObject> obj, bool legacyTargetOverride, bool& legacyOutputDidListenersThrow);

			virtual js_objects::NullablePin<EventTargetContextObject> getTheParent(v8::Local<v8::Context> context, js_objects::Pin<EventContextObject> evt) {
				return js_objects::NullablePin<EventTargetContextObject>();
			}

			virtual void activationBehavior(v8::Local<v8::Context> context, js_objects::Pin<EventContextObject> evt) {}

			virtual void legacyPreActivationBehavior(v8::Local<v8::Context> context, js_objects::Pin<EventContextObject> evt) {}

			virtual void legacyCanceledActivationBehavior(v8::Local<v8::Context> context, js_objects::Pin<EventContextObject> evt) {}

			CO_METHOD(fireEvent, template<typename EventClass> void, v8::Local<v8::String> evt, bool legacyTargetOverride);

		private:

			friend class EventPathData;

			enum class activation_behavior_properties {
				activationBehavior, legacyPreActivationBehavior, legacyCanceledActivationBehavior, __COUNT__
			};

			std::map<js_objects::HashableV8String, std::deque<EventHandler>>& accessMap(bool capturers) {
				auto& map = capturers ? this->capturers : this->bubblers;
				return map ? *map : *(map = std::make_unique<std::map<js_objects::HashableV8String, std::deque<EventHandler>>>());
			}

			std::optional<std::deque<EventHandler>*> checkMapPresence(bool capturers, js_objects::HashableV8String str) {
				const auto& map = capturers ? this->capturers : this->bubblers;
				if (map) {
					auto it = map->find(str);
					if (it != map->end()) {
						return { &const_cast<std::deque<EventHandler>&>(it->second) };
					}
				}
				return {};
			}

			std::unique_ptr<std::map<js_objects::HashableV8String, std::deque<EventHandler>>> bubblers;
			std::unique_ptr<std::map<js_objects::HashableV8String, std::deque<EventHandler>>> capturers;
			std::size_t non_passive_count;
			data_structs::EnumSet<activation_behavior_properties> activationBehaviors;
		};
	}
}