#pragma once

#include <deque>

#include "base_v8/root.h"
#include "base_v8/macros.h"

namespace dom {
	namespace events {

		enum class internal_event_flags {
			stop_propagation, stop_immediate_propagation, canceled, in_passive_listener, composed, initialized, dispatch, bubbles, cancelable, trusted
		};

		class EventTargetContextObject;

		class EventPathData {
		public:

			EventPathData(EventTargetContextObject* invocationTarget,
				EventTargetContextObject* relatedTarget,
				v8::Local<v8::Array> touchTargets,
				bool rootOfClosedTree,
				bool slotInClosedTree);

			EventPathData(EventPathData&& move);

			EventPathData(const EventPathData& copy) = default;

			EventPathData& operator=(EventPathData&& move);

			EventPathData& operator=(const EventPathData& copy) = default;

			js_objects::CPP_JS_Obj_Ref<EventTargetContextObject> invocationTarget;
			js_objects::CPP_JS_Obj_Ref<EventTargetContextObject> relatedTarget;
			v8::Persistent<v8::Array> touchTargets;
			bool rootOfClosedTree;
			bool slotInClosedTree;

			~EventPathData();

		private:
			bool moved;
		};

		class EventContextObject : public js_objects::BaseContextObject {
		public:

			CO_READONLY_ATTRIBUTE(type, v8::Local<v8::String>) {
				return this->type.Get(isolate);
			}

			CO_READONLY_ATTRIBUTE(target, EventTargetContextObject*) {
				return target.cppCast();
			}

			CO_READONLY_ATTRIBUTE(srcElement, EventTargetContextObject*) {
				return this->targetGET(isolate);
			}

			CO_READONLY_ATTRIBUTE(currentTarget, EventTargetContextObject*);

			CO_METHOD(composedPath, js_objects::JSThrowablePrimitive<v8::Array>);

			CO_READONLY_ATTRIBUTE(eventPhase, unsigned char) {
				return this->eventPhase;
			}

			CO_METHOD(stopPropagation, void);

			CO_ATTRIBUTE(cancelBubble, bool);

			CO_METHOD(stopImmediatePropagation, void);

			CO_READONLY_ATTRIBUTE(bubbles, bool) {
				return this->dataAttrs.contains(internal_event_flags::bubbles);
			}

			CO_READONLY_ATTRIBUTE(cancelable, bool) {
				return this->dataAttrs.contains(internal_event_flags::cancelable);
			}

			CO_ATTRIBUTE(returnValue, bool);

			CO_METHOD(preventDefault, void);

			CO_READONLY_ATTRIBUTE(isTrusted, bool) {
				return this->dataAttrs.contains(internal_event_flags::trusted);
			}

			CO_READONLY_ATTRIBUTE(timeStamp, v8::Local<v8::Number>) {
				return v8::Number::New(isolate, this->timestamp);
			}

			~EventContextObject() = default;

		private:
			v8::UniquePersistent<v8::String> type;
			data_structs::EnumSet<internal_event_flags> dataAttrs;
			js_objects::CPP_JS_Obj_Ref<EventTargetContextObject> target;
			js_objects::CPP_JS_Obj_Ref<EventTargetContextObject> relatedTarget;
			v8::UniquePersistent<v8::Array> touchTargetList;
			std::deque<EventPathData> path;
			double timestamp;
			unsigned char eventPhase;
		};
	}
}