#pragma once
namespace dom {
	namespace nodes {
#define MIXIN_PARENT_NODE Return<false>::Type<HTMLCollectionContextObject, v8::Object> childrenGET();\
Return<false>::Type<ElementContextObject, v8::Value> firstElementChildGET();\
Return<false>::Type<ElementContextObject, v8::Value> lastElementChildGET();\
std::size_t childElementCountGET();\
Return<true>::Type<void> prependMETHOD(std::deque<NodeContextObject> nodes);\
Return<true>::Type<void> appendMETHOD(std::deque<NodeContextObject*> nodes);\
Return<false>::Type<ElementContextObject, v8::Value> querySelectorMETHOD(const std::string &selector);\
Return<false>::Type<NodeListContextObject, v8::Object> querySelectorAllMETHOD(const std::string &selector);
	}
}