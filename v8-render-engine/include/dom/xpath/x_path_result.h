#pragma once

#include "dom/nodes/node.h"

namespace cpp {
	namespace dom {
		namespace xpath {
			class XPathResultContextObject : public DOMContextObject {
			public:
				CO_STATIC_UINT32_ATTRIBUTE(ANY_TYPE, 0);
				CO_STATIC_UINT32_ATTRIBUTE(NUMBER_TYPE, 1);
				CO_STATIC_UINT32_ATTRIBUTE(STRING_TYPE, 2);
				CO_STATIC_UINT32_ATTRIBUTE(BOOLEAN_TYPE, 3);
				CO_STATIC_UINT32_ATTRIBUTE(UNORDERED_NODE_ITERATOR_TYPE, 4);
				CO_STATIC_UINT32_ATTRIBUTE(ORDERED_NODE_ITERATOR_TYPE, 5);
				CO_STATIC_UINT32_ATTRIBUTE(UNORDERED_NODE_SNAPSHOT_TYPE, 6);
				CO_STATIC_UINT32_ATTRIBUTE(ORDERED_NODE_SNAPSHOT_TYPE, 7);
				CO_STATIC_UINT32_ATTRIBUTE(ANY_UNORDERED_NODE_TYPE, 8);
				CO_STATIC_UINT32_ATTRIBUTE(FIRST_ORDERED_NODE_TYPE, 9);

				CO_PRIMITIVE_GETTER(resultType, v8::Uint32);
				CO_PRIMITIVE_GETTER(numberValue, v8::Number);
				CO_TRANSPARENT_GETTER(stringValue);
				CO_PRIMITIVE_GETTER(booleanValue, v8::Boolean);
				CO_PIN_GETTER(singleNodeValue);
				CO_PRIMITIVE_GETTER(invalidIteratorState, v8::Boolean);
				CO_PRIMITIVE_GETTER(snapshotLength, v8::Uint32);

			private:
				uint32_t resultType;
				double numberValue;
				v8::UniquePersistent<v8::String> stringValue;
				bool booleanValue;
				pins::NullProperty<nodes::NodeContextObject> singleNodeValue;
				bool invalidIteratorState;
				uint32_t snapshotLength;
			};
		}
	}
}