#pragma once

#include <string>
#include <deque>
#include <map>
#include <utility>
#include "v8.h"
#include "macros.h"
#include "data_structs/enum_set.h"
#include "event.h"
#include "event_target_subclass_info_flags.h"

namespace dom {
	namespace events {

		enum class internal_event_listener_option_flags {
			CAPTURE, PASSIVE, ONCE, COUNT
		};

		typedef data_structs::EnumSet<internal_event_listener_option_flags> HandlerFlags;
		typedef v8::Function EventHandler;
		typedef std::pair<HandlerFlags, v8::UniquePersistent<EventHandler>> HandlerData;
		typedef std::map<std::string, std::deque<HandlerData>> HandlerMap;

		class EventTargetContextObject {
		public:
			void addEventListener(std::string type, v8::Local<EventHandler> handler, HandlerFlags options, v8::Isolate* isolate);

			void removeEventListener(std::string type, v8::Local<EventHandler> handler, bool capture, v8::Isolate* isolate);

			Return<true>::Type<bool> dispatchEvent(EventContextObject& ev, v8::Isolate* isolate) const;

			virtual EventTargetContextObject* getTheParent(EventContextObject& ev) const {
				return nullptr;
			}

			virtual void activationBehavior(EventContextObject& ev) const;

			virtual ~EventTargetContextObject() = default;
		private:
			data_structs::EnumSet<internal_event_target_subclass_info_flags> subclass_info;
			std::unique_ptr<HandlerMap> bubbleHandlers;
			std::unique_ptr<HandlerMap> captureHandlers;
			std::size_t notPassive;
		};
	}
}