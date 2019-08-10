#pragma once
namespace dom {
	namespace nodes {
#define MIXIN_CHILD_NODE Return<false>::Type<void> beforeMETHOD(std::deque<NodeContextObject*> prepend);\
Return<false>::Type<void> afterMETHOD(std::deque<NodeContextObject*> append);\
Return<false>::Type<void> replaceWithMETHOD(std::deque<NodeContextObject*> replacement);\
Return<false>::Type<void> removeMETHOD();
	}
}