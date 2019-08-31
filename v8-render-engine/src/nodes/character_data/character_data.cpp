#include "nodes/character_data/character_data.h"

dom::nodes::elements::html::HTMLSlotElementContextObject* dom::nodes::TextContextObject::assignedSlotGET(v8::Local<v8::Context> context) {
	return this->assignedSlot.cppRef;
}