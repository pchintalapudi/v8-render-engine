#pragma once

#include "base_v8/root.h"
#include "base_v8/macros.h"

namespace dom {

	namespace nodes {
		class NodeContextObject;
		class DocumentFragment;
	}

	namespace ranges {
		class AbstractRangeContextObject : public js_objects::BaseContextObject {
		public:
			CO_READONLY_ATTRIBUTE(startContainer, nodes::NodeContextObject&);

			CO_READONLY_ATTRIBUTE(startOffset, std::size_t);

			CO_READONLY_ATTRIBUTE(endContainer, nodes::NodeContextObject&);

			CO_READONLY_ATTRIBUTE(endOffset, std::size_t);

			CO_READONLY_ATTRIBUTE(collapsed, bool);
		};

		class StaticRangeContextObject : public AbstractRangeContextObject {};

		class RangeContextObject : public AbstractRangeContextObject {
		public:
			CO_READONLY_ATTRIBUTE(commonAncestorContainer, nodes::NodeContextObject&);

			CO_METHOD(setStart, void, nodes::NodeContextObject& node, std::size_t offset);
			CO_METHOD(setEnd, void, nodes::NodeContextObject& node, std::size_t offset);
			CO_METHOD(setStartBefore, void, nodes::NodeContextObject& node);
			CO_METHOD(setStartAfter, void, nodes::NodeContextObject& node);
			CO_METHOD(setEndBefore, void, nodes::NodeContextObject& node);
			CO_METHOD(setEndAfter, void, nodes::NodeContextObject& node);

			CO_METHOD(collapse, void, bool toStart);

			CO_METHOD(selectNode, void, nodes::NodeContextObject& node);
			CO_METHOD(selectNodeContents, void, nodes::NodeContextObject& node);
			
			CO_METHOD(compareBoundaryPoints, signed char, unsigned char how, RangeContextObject& sourceRange);

			//CEReactions
			CO_METHOD(deleteContents, void);

			//CEReactions
			CO_METHOD(extractContents, nodes::DocumentFragment&);
			//CEReactions
			CO_METHOD(cloneContents, nodes::DocumentFragment&);

			//CEReactions
			CO_METHOD(insertNode, void, nodes::NodeContextObject& node);

			//CEReactions
			CO_METHOD(surroundContents, void, nodes::NodeContextObject& newParent);

			CO_METHOD(cloneRange, RangeContextObject&);

			CO_METHOD(detach, void);

			CO_METHOD(isPointInRange, bool, nodes::NodeContextObject& node, std::size_t offset);

			CO_METHOD(comparePoint, signed char, nodes::NodeContextObject& node, std::size_t offset);

			CO_METHOD(intersectsNode, bool, nodes::NodeContextObject& node);

			CO_METHOD(toString, v8::Local<v8::String>);
		};
	}
}