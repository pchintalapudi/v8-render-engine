#pragma once

namespace dom {
	namespace nodes {
#define MIXIN_NON_ELEMENT_PARENT_NODE Return<false>::Type<ElementContextObject, v8::Value> getElementByIDMETHOD(std::string id);
	}
}