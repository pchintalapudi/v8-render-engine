#pragma once

#include <deque>

#include "v8.h"
#include "data_structs/data_structs.h"
#include "timing/timing.h"
#include "macros.h"
namespace dom {
	namespace events {
		enum class EventPhase {
			NONE, CAPTURING_PHASE, AT_TARGET, BUBBLING_PHASE, COUNT
		};

		enum class internal_event_flags {
			stop_propagation, stop_immediate_propagation, canceled, in_passive_listener, composed, initialized, dispatch, bubbles, cancelable, trusted, COUNT
		};

		struct EventTargetInfo;
		typedef v8::Array TouchTargetList;

		typedef v8::Value PotentialEventTarget;

		struct EventTargetInfo {
			v8::UniquePersistent<PotentialEventTarget> target;
			v8::UniquePersistent<PotentialEventTarget> relatedTarget;
			v8::UniquePersistent<TouchTargetList> touchTargetList;

			static EventTargetInfo New(v8::UniquePersistent<PotentialEventTarget> target, v8::UniquePersistent<PotentialEventTarget> relatedTarget, v8::UniquePersistent<TouchTargetList> touchTargetList) {
				return { std::move(target), std::move(relatedTarget), std::move(touchTargetList) };
			}
		};

		struct EventPathTargetData {
			EventTargetInfo targetInfo;
			bool invocationTargetInShadowTree;
			v8::UniquePersistent<PotentialEventTarget> shadowAdjustedTarget;
			bool rootOfClosedTree;
			bool slotInClosedTree;

			static EventPathTargetData New(EventTargetInfo targetInfo,
				bool invocationTargetInShadowTree,
				v8::UniquePersistent<PotentialEventTarget> shadowAdjustedTarget,
				bool rootOfClosedTree,
				bool slotInClosedTree) {
				return { std::move(targetInfo), invocationTargetInShadowTree, std::move(shadowAdjustedTarget), rootOfClosedTree, slotInClosedTree };
			}
		};
		typedef v8::String EventType;
		typedef std::deque<EventPathTargetData> EventPath;
		typedef data_structs::EnumSet<internal_event_flags> EventFlags;
		typedef v8::Array ComposedPath;
		typedef v8::Value Event;

		class EventContextObject {
		public:

			EventContextObject(v8::Isolate* isolate, v8::Local<EventType> type, EventFlags eventFlags);

			const v8::Local<EventType> typeGET(v8::Isolate* isolate) const {
				return this->type.Get(isolate);
			}

			//Also aliases srcElement
			const v8::Local<PotentialEventTarget> targetGET(v8::Isolate* isolate) const {
					return this->targetInfo.target.Get(isolate);
			}

			v8::Local<PotentialEventTarget> currentTargetGET(v8::Isolate* isolate) const {
				return this->path[this->idx].targetInfo.target.Get(isolate);
			}

			Return<false>::Type<void, ComposedPath> composedPathMETHOD(v8::Isolate *isolate) const;

			void stopPropagationMETHOD() { this->eventFlags.add(internal_event_flags::stop_propagation); }

			bool cancelBubbleGET() const { return this->eventFlags.contains(internal_event_flags::stop_propagation); }

			void cancelBubbleSET(bool cancel) { if (cancel) this->stopPropagationMETHOD(); }

			void stopImmediatePropagationMETHOD() {
				this->eventFlags.add(internal_event_flags::stop_immediate_propagation, internal_event_flags::stop_propagation);
			}

			bool bubblesGET() const {
				return this->eventFlags.contains(internal_event_flags::bubbles);
			}

			bool cancelableGET() const {
				return this->eventFlags.contains(internal_event_flags::cancelable);
			}

			bool returnValueGET() const {
				return !this->defaultPreventedGET();
			}

			void returnValueSET(bool returnValue) { if (!returnValue) this->preventDefaultMETHOD(); }

			void preventDefaultMETHOD() { this->eventFlags.add(internal_event_flags::canceled); }

			bool defaultPreventedGET() const {
				return this->eventFlags.contains(internal_event_flags::canceled);
			}

			bool composedGET() const {
				return this->eventFlags.contains(internal_event_flags::composed);
			}

			bool isTrustedGET_UNFORGEABLE() const {
				return this->eventFlags.contains(internal_event_flags::trusted);
			}

			EventPhase phaseGET() const {
				return this->phase;
			}

			v8::Local<PotentialEventTarget> getRelatedTarget(v8::Isolate* isolate) const {
				return this->targetInfo.relatedTarget.Get(isolate);
			}

			v8::Local<v8::Array> getTouchTargets(v8::Isolate* isolate) const {
				return this->targetInfo.touchTargetList.Get(isolate);
			}

			EventFlags& getFlags() {
				return this->eventFlags;
			}

		private:
			v8::UniquePersistent<EventType> type;
			EventPhase phase;
			EventFlags eventFlags;
			EventTargetInfo targetInfo;
			EventPath path;
			std::size_t idx;
		};
	}
}