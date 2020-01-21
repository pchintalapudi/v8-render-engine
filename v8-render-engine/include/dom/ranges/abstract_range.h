#pragma once

#include "dom/nodes/node.h"

namespace cpp {
	namespace dom {
		class AbstractRangeContextObject : public DOMContextObject {

			CO_PIN_GETTER(startContainer);
			CO_PIN_GETTER(endContainer);
			CO_PRIMITIVE_GETTER(startOffset, v8::Integer);
			CO_PRIMITIVE_GETTER(endOffset, v8::Integer);
			CO_PRIMITIVE_GETTER(collapsed, v8::Boolean);

		protected:
			pins::Property<nodes::NodeContextObject> startContainer;
			pins::Property<nodes::NodeContextObject> endContainer;
			unsigned long startOffset, endOffset;
			bool collapsed;
		};

		class StaticRangeContextObject : public AbstractRangeContextObject {
		public:
		};
	}
}