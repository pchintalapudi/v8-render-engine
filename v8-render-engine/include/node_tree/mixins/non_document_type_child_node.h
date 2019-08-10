#pragma once
namespace dom {
	namespace nodes {
#define MIXIN_NON_DOCUMENT_TYPE_CHILD_NODE Return<false>::Type<ElementContextObject, v8::Value> previousElementSiblingGET();\
Return<false>::Type<ElementContextObject, v8::Value> nextElementSiblingGET();
	}
}