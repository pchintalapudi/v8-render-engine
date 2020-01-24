#pragma once

#include "macro_utils.h"
#include "base_v8/pins.h"

#define MIXIN_NON_ELEMENT_PARENT_NODE CO_METHOD(getElementById, pins::NullPin<nodes::ElementContextObject>, v8::Local<v8::String> id)
#define MIXIN_DOCUMENT_OR_SHADOW_ROOT
#define MIXIN_PARENT_NODE CO_READONLY_ATTRIBUTE(children, pins::Pin<dom::HTMLCollectionContextObject>); CO_READONLY_ATTRIBUTE(firstElementChild, pins::NullPin<nodes::ElementContextObject>); CO_READONLY_ATTRIBUTE(lastElementChild, pins::NullPin<nodes::ElementContextObject>); CO_READONLY_ATTRIBUTE(childElementCount, v8::Local<v8::Integer>); CO_METHOD(prepend, void, std::vector<pins::Pin<nodes::NodeContextObject>> nodes); CO_METHOD(append, void, std::vector<pins::Pin<nodes::NodeContextObject>> nodes); CO_METHOD(querySelector, pins::NullPin<nodes::ElementContextObject>, v8::Local<v8::String> selectors); CO_METHOD(querySelectorAll, pins::Pin<dom::NodeListContextObject>, v8::Local<v8::String> selectors);
#define MIXIN_NON_DOCUMENT_TYPE_CHILD_NODE CO_READONLY_ATTRIBUTE(previousElementSibling, pins::NullPin<nodes::ElementContextObject>); CO_READONLY_ATTRIBUTE(nextElementSibling, pins::NullPin<nodes::ElementContextObject>);
#define MIXIN_CHILD_NODE CO_METHOD(before, void, std::vector<pins::Pin<nodes::NodeContextObject>> nodes); CO_METHOD(after, void, std::vector<pins::Pin<nodes::NodeContextObject>> nodes); CO_METHOD(replaceWith, void, std::vector<pins::Pin<nodes::NodeContextObject>> nodes); CO_METHOD(remove, void);
#define MIXIN_SLOTABLE CO_READONLY_ATTRIBUTE(assignedSlot, pins::Pin<nodes::ElementContextObject>);
