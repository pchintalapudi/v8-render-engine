#pragma once

#include <deque>

#include "base_v8/root.h"
#include "base_v8/macros.h"

namespace dom {
	namespace events {

		enum class internal_event_flags {
			stop_propagation, stop_immediate_propagation, canceled, in_passive_listener, composed, initialized, dispatch, bubbles, cancelable, trusted, __COUNT__
		};

		class EventContextObject;
		class EventTargetContextObject;
		struct EventHandler;

		class EventPathData {
		public:

			EventPathData(v8::Isolate* isolate, js_objects::Pin<EventTargetContextObject> invocationTarget,
				bool invocationTargetInShadowTree,
				js_objects::NullablePin<EventTargetContextObject> shadowAdjustedTarget,
				js_objects::Pin<EventTargetContextObject> relatedTarget,
				v8::Local<v8::Array> touchTargets,
				bool rootOfClosedTree,
				bool slotInClosedTree) : invocationTarget(isolate, invocationTarget),
				invocationTargetInShadowTree(invocationTargetInShadowTree),
				shadowAdjustedTarget(isolate, shadowAdjustedTarget),
				relatedTarget(isolate, relatedTarget),
				touchTargets(isolate, touchTargets),
				rootOfClosedTree(rootOfClosedTree),
				slotInClosedTree(slotInClosedTree) {}

			js_objects::JS_CPP_Property<EventTargetContextObject> invocationTarget;
			bool invocationTargetInShadowTree;
			js_objects::Nullable_JS_CPP_Property<EventTargetContextObject> shadowAdjustedTarget;
			js_objects::JS_CPP_Property<EventTargetContextObject> relatedTarget;
			v8::UniquePersistent<v8::Array> touchTargets;
			bool rootOfClosedTree;
			bool slotInClosedTree;

			std::optional<js_objects::VoidSuccess> invoke(v8::Local<v8::Context> context, js_objects::Pin<EventContextObject> evt, bool capturing, bool& legacyOutputDidListenersThrow, js_objects::Pin<EventTargetContextObject> target);

			std::optional<bool> innerInvoke(v8::Local<v8::Context> context, js_objects::Pin<EventContextObject> evt, std::deque<EventHandler>& handlers, bool& legacyOutputDidListenersThrow, std::deque<EventHandler>& removeFrom);
		};

		class EventContextObject : public js_objects::BaseContextObject {
		public:

			CO_READONLY_ATTRIBUTE(type, v8::Local<v8::String>) {
				return this->type.Get(context->GetIsolate());
			}

			CO_READONLY_ATTRIBUTE(target, js_objects::NullablePin<EventTargetContextObject>) {
				return this->target.pinNullable(context->GetIsolate());
			}

			CO_READONLY_ATTRIBUTE(srcElement, auto) {
				return this->targetGET(context);
			}

			CO_READONLY_ATTRIBUTE(currentTarget, js_objects::NullablePin<EventTargetContextObject>) {
				return ~this->currentTarget ? this->path[this->currentTarget].invocationTarget.pin(context->GetIsolate())
					: js_objects::NullablePin<EventTargetContextObject>();
			}

			CO_METHOD(composedPath, v8::Local<v8::Array>);

			CO_READONLY_ATTRIBUTE(eventPhase, unsigned char) {
				return this->eventPhase;
			}

			CO_METHOD(stopPropagation, void) {
				this->dataAttrs.add(internal_event_flags::stop_propagation);
			}

			CO_INLINE_ATTRIBUTE_GETTER(cancelBubble, bool) {
				return this->dataAttrs.contains(internal_event_flags::stop_propagation);
			}

			CO_INLINE_ATTRIBUTE_SETTER(cancelBubble, bool) {
				if (attr) this->stopPropagationMETHOD(context);
			}

			CO_METHOD(stopImmediatePropagation, void) {
				this->dataAttrs.add(internal_event_flags::stop_propagation, internal_event_flags::stop_immediate_propagation);
			}

			CO_READONLY_ATTRIBUTE(bubbles, bool) {
				return this->dataAttrs.contains(internal_event_flags::bubbles);
			}

			CO_READONLY_ATTRIBUTE(cancelable, bool) {
				return this->dataAttrs.contains(internal_event_flags::cancelable);
			}

			CO_INLINE_ATTRIBUTE_GETTER(returnValue, bool) {
				return !this->dataAttrs.contains(internal_event_flags::canceled);
			}

			CO_INLINE_ATTRIBUTE_SETTER(returnValue, bool) {
				if (!attr) this->preventDefaultMETHOD(context);
			}

			CO_METHOD(preventDefault, void) {
				if (!this->dataAttrs.contains(internal_event_flags::in_passive_listener)) this->dataAttrs.add(internal_event_flags::canceled);
			}

			CO_READONLY_ATTRIBUTE(defaultPrevented, bool) {
				return this->dataAttrs.contains(internal_event_flags::canceled);
			}

			CO_READONLY_ATTRIBUTE(composed, bool) {
				return this->dataAttrs.contains(internal_event_flags::composed);
			}

			CO_READONLY_ATTRIBUTE(isTrusted, bool) {
				return this->dataAttrs.contains(internal_event_flags::trusted);
			}

			CO_READONLY_ATTRIBUTE(timeStamp, v8::Local<v8::Number>) {
				return v8::Number::New(context->GetIsolate(), this->timestamp);
			}

			CO_METHOD(initEvent, void, v8::Local<v8::String> type, bool bubbles, bool cancelable);

			~EventContextObject() = default;

		private:
			friend class EventTargetContextObject;
			friend class EventPathData;

			v8::UniquePersistent<v8::String> type;
			data_structs::EnumSet<internal_event_flags> dataAttrs;
			js_objects::Nullable_JS_CPP_Property<EventTargetContextObject> target;
			js_objects::Nullable_JS_CPP_Property<EventTargetContextObject> relatedTarget;
			v8::UniquePersistent<v8::Array> touchTargetList;
			std::deque<js_objects::JS_CPP_Property<EventTargetContextObject>> internalTouchTargets;
			std::deque<EventPathData> path;
			std::size_t currentTarget;
			double timestamp;
			unsigned char eventPhase;

			EventPathData& appendToEventPath(v8::Local<v8::Context> context, js_objects::Pin<EventTargetContextObject> invocationTarget,
				js_objects::NullablePin<EventTargetContextObject> shadowAdjustedTarget, js_objects::Pin<EventTargetContextObject> relatedTarget,
				v8::Local<v8::Array> touchTargets, bool slotInClosedTree);
		};
	}
}