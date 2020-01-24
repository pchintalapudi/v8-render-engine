#pragma once

#include <optional>
#include "mutation_record.h"

namespace cpp {
	namespace dom {

		namespace nodes {
			class NodeContextObject;
		}

		struct MutationObserverInit {
			bool childList;
			bool attributes;
			bool characterData;
			bool subtree;
			bool attributeOldValue;
			bool characterDataOldValue;
			v8::UniquePersistent<v8::Array> attributeFilter;

			bool operator==(const MutationObserverInit& other) const {
				return this->childList == other.childList && this->attributes == other.attributes && this->characterData == other.characterData && this->subtree == other.subtree && this->attributeOldValue == other.attributeOldValue && this->characterDataOldValue == other.characterDataOldValue && this->attributeFilter == other.attributeFilter;
			}

			bool operator!=(const MutationObserverInit& other) const {
				return !this->operator==(other);
			}

			MutationObserverInit clone(v8::Local<v8::Context> context) {
				return { childList, attributes, characterData, subtree, attributeOldValue, characterDataOldValue, v8::UniquePersistent<v8::Array>(context->GetIsolate(), attributeFilter.Get(context->GetIsolate())) };
			}
		};

		class MutationObserverContextObject : public DOMContextObject {
		public:

			CO_METHOD(observe, void, pins::Pin<MutationObserverContextObject> self, pins::Pin<nodes::NodeContextObject> target, MutationObserverInit options);

			CO_METHOD(disconnect, void);

			CO_METHOD(takeRecords, v8::Local<v8::Array>) {
				auto pinned = this->records.Get(context->GetIsolate());
				records.Reset(context->GetIsolate(), v8::Array::New(context->GetIsolate()));
				return pinned;
			}

		private:
			v8::UniquePersistent<v8::Function> callback;
			std::vector<pins::WeakProperty<nodes::NodeContextObject, false>> nodeList;
			v8::UniquePersistent<v8::Array> records;
		};

		typedef std::optional<std::pair<pins::Property<MutationObserverContextObject>, MutationObserverInit>> ObserverSource;
		typedef std::tuple<pins::Property<MutationObserverContextObject>, MutationObserverInit, ObserverSource> RegisteredObserver;
	}
}