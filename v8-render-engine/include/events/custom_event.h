#pragma once

#include "event.h"

namespace dom {
	namespace events {
		typedef v8::Object Detail;
		class CustomEventContextObject : public EventContextObject {
		public:
			CustomEventContextObject(v8::Isolate* isolate, v8::Local<EventType> type, EventFlags eventFlags, v8::Local<Detail> detail):EventContextObject(isolate, type, eventFlags), detail(isolate, detail) {}
		private:
			v8::UniquePersistent<Detail> detail;
		};
	}
}