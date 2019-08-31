#include "nodes/elements/element.h"

dom::nodes::elements::html::HTMLSlotElementContextObject* dom::nodes::ElementContextObject::assignedSlotGET(v8::Local<v8::Context> context) {
	return this->assignedSlot.cppRef;
}