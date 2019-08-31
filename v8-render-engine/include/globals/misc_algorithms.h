#pragma once
namespace dom {

	namespace events {
		class EventTargetContextObject;
	}

	namespace algorithms {
		js_objects::Pin<events::EventTargetContextObject> retarget(v8::Local<v8::Context> context, js_objects::Pin<events::EventTargetContextObject> a, js_objects::Pin<events::EventTargetContextObject> b);

		bool checkSlotable(v8::Local<v8::Context> context, js_objects::Pin<events::EventTargetContextObject> target);
	}
}