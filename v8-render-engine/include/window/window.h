#pragma once

#include "base_v8/pins.h"

namespace cpp {

	namespace dom {
		class EventContextObject;
	}

	namespace window {
		class WindowContextObject {
			CO_INLINE_ATTRIBUTE_GETTER(event, pins::NullPin<dom::EventContextObject>) {
				return this->event.pin(context->GetIsolate());
			}
		private:
			pins::NullProperty<dom::EventContextObject> event;
		};
	}
}