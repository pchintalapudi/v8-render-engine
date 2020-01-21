#pragma once

#include "dom/dom_core.h"
#include "macro_utils.h"

namespace cpp {
	namespace dom {

		class EventTargetContextObject;

		class EventContextObject : public DOMContextObject {
		public:
			CO_TRANSPARENT_GETTER(type);

			CO_PIN_GETTER(target);

			CO_PIN_GETTER(currentTarget);

			CO_METHOD(composedPath, v8::Local<v8::Array>);

			CO_STATIC_UINT32_ATTRIBUTE(NONE, 0);
			CO_STATIC_UINT32_ATTRIBUTE(CAPTURING_PHASE, 1);
			CO_STATIC_UINT32_ATTRIBUTE(AT_TARGET, 2);
			CO_STATIC_UINT32_ATTRIBUTE(BUBBLING_PHASE, 3);

			CO_PRIMITIVE_GETTER(eventPhase, v8::Integer);

			CO_METHOD(stopPropagation, void) {
				this->stopPropagation = true;
			}
			CO_METHOD(stopImmediatePropagation, void) {
				this->stopPropagation = true;
				this->stopImmediatePropagation = true;
			}

			CO_PRIMITIVE_GETTER(bubbles, v8::Boolean);
			CO_PRIMITIVE_GETTER(cancelable, v8::Boolean);
			CO_PRIMITIVE_GETTER(composed, v8::Boolean);
			CO_PRIMITIVE_GETTER(isTrusted, v8::Boolean);
			CO_PRIMITIVE_GETTER(timeStamp, v8::Number);

			CO_READONLY_ATTRIBUTE(defaultPrevented) {
				return v8::Boolean::New(context->GetIsolate(), this->canceled);
			}


			CO_METHOD(preventDefault, void) {
				if (this->cancelable && !this->inPassiveListener) {
					this->canceled = true;
				}
			}

			CO_INLINE_ATTRIBUTE_GETTER(cancelBubble) {
				return v8::Boolean::New(context->GetIsolate(), this->stopPropagation);
			}

			CO_INLINE_ATTRIBUTE_SETTER(cancelBubble, v8::Local<v8::Boolean>) {
				if (attr->BooleanValue(context->GetIsolate())) {
					this->stopPropagation = true;
				}
			}

			CO_INLINE_ATTRIBUTE_GETTER(returnValue) {
				return v8::Boolean::New(context->GetIsolate(), this->canceled);
			}

			CO_INLINE_ATTRIBUTE_SETTER(returnValue, v8::Local<v8::Boolean>) {
				if (!attr->BooleanValue(context->GetIsolate())) {
					this->preventDefault_METHOD(context);
				}
			}

			CO_METHOD(initEvent, void, v8::Local<v8::String> type, bool bubbles, bool cancelable);

			~EventContextObject() = default;

			void trust(bool trust) {
				this->isTrusted = trust;
			}

			bool dispatched() {
				return this->dispatch;
			}

			void setDispatch(bool d) {
				this->dispatch = d;
			}

			bool isInitialized() {
				return this->initialized;
			}

			auto related(v8::Local<v8::Context> context) {
				return this->relatedTarget.pin(context->GetIsolate());
			}

			auto& touchTargets() {
				return this->touchTargetList;
			}

			auto& getPath() {
				return this->path;
			}

			void setPhase(uint32_t phase) {
				this->eventPhase = phase;
			}

			void setCurrentTarget(v8::Local<v8::Context> context, pins::NullPin<EventTargetContextObject> next) {
				this->currentTarget.set(context->GetIsolate(), next);
			}

			void clearFlags() {
				this->dispatch = false;
				this->stopImmediatePropagation = false;
				this->stopPropagation = false;
			}

			void clearTargets(v8::Local<v8::Context> context) {
				this->target.reset(context->GetIsolate());
				this->relatedTarget.reset(context->GetIsolate());
				this->touchTargetList = nullptr;
			}

			void clearTouches() {
				this->touchTargetList = nullptr;
			}

			bool isCanceled() {
				return this->canceled;
			}

			void setTargets(v8::Local<v8::Context> context, cpp::pins::NullPin<EventTargetContextObject> target, cpp::pins::NullPin<EventTargetContextObject> relatedTarget,
				std::vector<pins::NullProperty<EventTargetContextObject>>* touchTargetList) {
				this->target.set(context->GetIsolate(), target);
				this->relatedTarget.set(context->GetIsolate(), relatedTarget);
				this->touchTargetList = touchTargetList;
			}

			bool isStopped() {
				return this->stopPropagation;
			}

			bool isImmediateStop() {
				return this->stopImmediatePropagation;
			}

			void setType(v8::Local<v8::Context> context, v8::Local<v8::String> type) {
				this->type.Reset(context->GetIsolate(), type);
			}

			void passive(bool p) {
				this->inPassiveListener = p;
			}

			struct PathStruct {
				pins::Property<EventTargetContextObject> invocationTarget;
				bool invocationTargetInShadowTree;
				pins::NullProperty<EventTargetContextObject> shadowAdjustedTarget;
				pins::NullProperty<EventTargetContextObject> relatedTarget;
				std::vector<pins::NullProperty<EventTargetContextObject>> touchTargetList;
				bool rootOfClosedTree;
				bool slotInClosedTree;
			};

		private:
			//Defined JS Properties
			v8::UniquePersistent<v8::String> type;
			pins::NullProperty<EventTargetContextObject> target;
			pins::NullProperty<EventTargetContextObject> currentTarget;

			//Internal properties
			uint32_t eventPhase;
			double timeStamp;
			pins::NullProperty<EventTargetContextObject> relatedTarget;
			std::vector<pins::NullProperty<EventTargetContextObject>>* touchTargetList;
			std::vector<PathStruct> path;

			//Event Flags
			bool bubbles;
			bool cancelable;
			bool stopPropagation;
			bool stopImmediatePropagation;
			bool canceled;
			bool inPassiveListener;
			bool composed;
			bool initialized;
			bool dispatch;
			bool isTrusted;
		};
	}
}