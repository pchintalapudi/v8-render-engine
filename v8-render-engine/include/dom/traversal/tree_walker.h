#pragma once

#include "dom/nodes/node.h"

namespace cpp {
	namespace dom {
		class TreeWalkerContextObject : public DOMContextObject {
		public:
			CO_PIN_GETTER(root);
			CO_PIN_GETTER(currentNode);
			CO_PRIMITIVE_GETTER(whatToShow, v8::Integer);
			CO_TRANSPARENT_GETTER(filter);

			CO_METHOD(parentNode, pins::NullPin<nodes::NodeContextObject>);
			CO_METHOD(firstChild, pins::NullPin<nodes::NodeContextObject>);
			CO_METHOD(lastChild, pins::NullPin<nodes::NodeContextObject>);
			CO_METHOD(previousSibling, pins::NullPin<nodes::NodeContextObject>);
			CO_METHOD(nextSibling, pins::NullPin<nodes::NodeContextObject>);
			CO_METHOD(previousNode, pins::NullPin<nodes::NodeContextObject>);
			CO_METHOD(nextNode, pins::NullPin<nodes::NodeContextObject>);
		private:
			pins::Property<nodes::NodeContextObject> root;
			pins::Property<nodes::NodeContextObject> currentNode;
			uint32_t whatToShow;
			v8::UniquePersistent<v8::Function> filter;
		};
	}
}