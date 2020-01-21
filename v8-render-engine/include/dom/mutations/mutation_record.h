#pragma once

#include "dom/dom_core.h"
#include "dom/collections/node_list.h"
#include "macro_utils.h"

namespace cpp {
	namespace dom {

		namespace nodes {
			class NodeContextObject;
		}

		class MutationRecordContextObject {

			CO_TRANSPARENT_GETTER(type);

			CO_PIN_GETTER(addedNodes);
			CO_PIN_GETTER(removedNodes);

		private:
			v8::UniquePersistent<v8::String> type;
			pins::Pin<nodes::NodeContextObject> target;
			pins::Property<dom::NodeListContextObject> addedNodes;
			pins::Property<dom::NodeListContextObject> removedNodes;
			pins::NullProperty<nodes::NodeContextObject> previousSibling;
			pins::NullProperty<nodes::NodeContextObject> nextSibling;
			v8::UniquePersistent<v8::Value> attributeName;
			v8::UniquePersistent<v8::Value> attributeNamespace;
			v8::UniquePersistent<v8::Value> attributeOldValue;
		};
	}
}