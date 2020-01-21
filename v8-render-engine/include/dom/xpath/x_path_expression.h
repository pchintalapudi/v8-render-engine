#pragma once

#include "x_path_result.h"

namespace cpp {
	namespace dom {
		namespace xpath {
			class XPathExpressionContextObject : public DOMContextObject {
			public:
				CO_METHOD(evaluate, pins::Pin<XPathResultContextObject>, pins::Pin<nodes::NodeContextObject> node, uint32_t type, pins::NullPin<XPathResultContextObject> result);
			};
		}
	}
}