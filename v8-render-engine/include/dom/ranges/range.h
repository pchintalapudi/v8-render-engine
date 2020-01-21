#pragma once

#include "abstract_range.h"

namespace cpp {
	namespace dom {

		namespace nodes {
			class DocumentFragmentContextObject;
		}

		class RangeContextObject : public AbstractRangeContextObject {
			CO_READONLY_ATTRIBUTE(commonAncestorContainer) {
				auto start = this->startContainer.pin(context->GetIsolate());
				return start->commonAncestor(context, start, this->endContainer.pin(context->GetIsolate()));
			}

			CO_METHOD(setStart, void, pins::Pin<nodes::NodeContextObject> node, uint32_t offset);
			CO_METHOD(setEnd, void, pins::Pin<nodes::NodeContextObject> node, uint32_t offset);
			CO_METHOD(setStartBefore, void, pins::Pin<nodes::NodeContextObject> node);
			CO_METHOD(setStartAfter, void, pins::Pin<nodes::NodeContextObject> node);
			CO_METHOD(setEndBefore, void, pins::Pin<nodes::NodeContextObject> node);
			CO_METHOD(setEndAfter, void, pins::Pin<nodes::NodeContextObject> node);

			CO_METHOD(collapse, void, bool toStart);
			CO_METHOD(selectNode, void, pins::Pin<nodes::NodeContextObject> node);
			CO_METHOD(selectNodeContents, void, pins::Pin<nodes::NodeContextObject> node);

			CO_STATIC_UINT32_ATTRIBUTE(START_TO_START, 0);
			CO_STATIC_UINT32_ATTRIBUTE(START_TO_END, 1);
			CO_STATIC_UINT32_ATTRIBUTE(END_TO_END, 2);
			CO_STATIC_UINT32_ATTRIBUTE(END_TO_START, 3);

			uint32_t compareBoundaryPoints(uint32_t how, pins::Pin<RangeContextObject> sourceRange);

			CO_METHOD(deleteContents, void);
			CO_METHOD(extractContents, pins::Pin<nodes::DocumentFragmentContextObject>);
			CO_METHOD(cloneContents, pins::Pin<nodes::DocumentFragmentContextObject>);
			CO_METHOD(insertNode, void, pins::Pin<nodes::NodeContextObject> node);
			CO_METHOD(surroundContents, void, pins::Pin<nodes::NodeContextObject> newParent);

			CO_METHOD(cloneRange, pins::Pin<RangeContextObject>);
			CO_METHOD(detach, void);

			CO_METHOD(isPointInRange, v8::Local<v8::Boolean>, pins::Pin<nodes::NodeContextObject> node, uint32_t offset);
			CO_METHOD(comparePoint, uint32_t, pins::Pin<nodes::NodeContextObject> node, uint32_t offset);

			CO_METHOD(intersectsNode, bool, pins::Pin<nodes::NodeContextObject> node);
		};
	}
}