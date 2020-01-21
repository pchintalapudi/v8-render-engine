#pragma once

#include "dom/nodes/node.h"

namespace cpp {
	namespace dom {
		class NodeIteratorContextObject : public DOMContextObject {
		public:
			CO_PIN_GETTER(root);
			CO_PIN_GETTER(reference);
			CO_PRIMITIVE_GETTER(pointerBeforeReference, v8::Boolean);
			CO_PRIMITIVE_GETTER(whatToShow, v8::Integer);
			CO_TRANSPARENT_GETTER(filter);

			CO_METHOD(nextNode, pins::NullPin<nodes::NodeContextObject>);
			CO_METHOD(previousNode, pins::NullPin<nodes::NodeContextObject>);

			CO_METHOD(detach, void) {}
		private:
			pins::Property<nodes::NodeContextObject> root;
			pins::Property<nodes::NodeContextObject> reference;
			bool pointerBeforeReference;
			uint32_t whatToShow;
			v8::UniquePersistent<v8::Function> filter;
		};
	}
}