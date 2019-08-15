#pragma once

#include <deque>

#include "base_v8/root.h"
#include "base_v8/macros.h"
#include "data_structs/enum_set.h"

namespace dom {

	namespace nodes {
		class NodeContextObject;
	}

	namespace observer {

		//Records could leak memory if they're not taken out of a mutation observer's queue
		class MutationRecordContextObject : public js_objects::BaseContextObject {
		public:
			CO_TRANSPARENT_GETTER(type, v8::String)

				CO_TRANSPARENT_GETTER(node, v8::Object)

				CO_TRANSPARENT_GETTER(addedNodes, v8::Object)

				CO_TRANSPARENT_GETTER(removedNodes, v8::Object)

				CO_TRANSPARENT_GETTER(previousSibling, v8::Value)

				CO_TRANSPARENT_GETTER(nextSibling, v8::Value)

				CO_TRANSPARENT_GETTER(attributeName, v8::Value)

				CO_TRANSPARENT_GETTER(attributeNamespace, v8::Value)

				CO_TRANSPARENT_GETTER(oldValue, v8::Value)

		private:
			v8::UniquePersistent<v8::String> type;
			v8::UniquePersistent<v8::Object> node;
			v8::UniquePersistent<v8::Object> addedNodes;
			v8::UniquePersistent<v8::Object> removedNodes;
			v8::UniquePersistent<v8::Value> previousSibling;
			v8::UniquePersistent<v8::Value> nextSibling;
			v8::UniquePersistent<v8::Value> attributeName;
			v8::UniquePersistent<v8::Value> attributeNamespace;
			v8::UniquePersistent<v8::Value> oldValue;
		};

		enum class MutationObserverOptions {
			childList, attributes, characterData, subtree, attributeOldValue, characterDataOldValue
		};

		class MutationObserverContextObject : public js_objects::BaseContextObject {
		public:
			CO_METHOD(observe, void, nodes::NodeContextObject* nodes, data_structs::EnumSet<MutationObserverOptions> options, std::deque<std::string> attributeFilter);

			CO_METHOD(disconnect, void);

			CO_METHOD(takeRecords, v8::Local<v8::Array>);
		private:
			std::deque<MutationRecordContextObject*> records;
			v8::Persistent<v8::Function> callback;
			//Intentionally weak by standards
			v8::Persistent<v8::Object> weakNodeRef;
		};
	}
}