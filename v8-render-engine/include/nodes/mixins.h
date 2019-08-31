#pragma once

#include "base_v8/macros.h"

#define MIXIN_NON_ELEMENT_PARENT_NODE CO_METHOD(getElementById, ElementContextObject*, v8::Local<v8::String> id);

#define MIXIN_DOCUMENT_OR_SHADOW_ROOT

#define MIXIN_PARENT_NODE	CO_READONLY_ATTRIBUTE(children, collections::ElementHTMLCollectionContextObject&);\
							CO_READONLY_ATTRIBUTE(firstElementChild, ElementContextObject*);\
							CO_READONLY_ATTRIBUTE(lastElementChild, ElementContextObject*);\
							CO_READONLY_ATTRIBUTE(childElementCount, std::size_t);\
							CO_METHOD(prepend, void, std::deque<NodeContextObject*> nodes);\
							CO_METHOD(append, void, std::deque<NodeContextObject*> nodes);\
							CO_METHOD(querySelector, ElementContextObject*, v8::Local<v8::String> selector);\
							CO_METHOD(querySelectorAll, collections::NodeListContextObject&, v8::Local<v8::String> selector);

#define MIXIN_NON_DOCUMENT_TYPE_CHILD_NODE	CO_READONLY_ATTRIBUTE(previousElementSibling, ElementContextObject*);\
											CO_READONLY_ATTRIBUTE(nextElementSibling, ElementContextObject*);

#define MIXIN_CHILD_NODE	CO_METHOD(before, void, std::deque<NodeContextObject*> nodes);\
							CO_METHOD(after, void, std::deque<NodeContextObject*> nodes);\
							CO_METHOD(replaceWith, void, std::deque<NodeContextObject*> nodes);\
							CO_METHOD(remove, void);

#define MIXIN_SLOTABLE	CO_READONLY_ATTRIBUTE(assignedSlot, elements::html::HTMLSlotElementContextObject*);
