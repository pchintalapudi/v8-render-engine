#pragma once

#include "x_path_evaluator_base.h"

namespace cpp {
	namespace dom {
		namespace xpath {
			class XPathEvaluatorContextObject : public DOMContextObject {
			public:
				MIXIN_XPATH_EVALUATOR_BASE
			};
		}
	}
}