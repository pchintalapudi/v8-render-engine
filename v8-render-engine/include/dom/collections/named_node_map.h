#pragma once

#include "dom/dom_core.h"
#include "dom/nodes/document.h"

namespace cpp {
	namespace dom {
		class NamedNodeMapContextObject : public DOMContextObject {
		public:
			CO_READONLY_ATTRIBUTE(length) {
				return v8::Integer::New(context->GetIsolate(), this->attributeList.size());
			}
			CO_METHOD(item, pins::NullPin<nodes::AttrContextObject>, unsigned long index) {
				return index < this->attributeList.size() ? this->attributeList[index].pin(context->GetIsolate()) : pins::NullPin<nodes::AttrContextObject>::null(context->GetIsolate());
			}
			CO_METHOD(getNamedItem, pins::NullPin<nodes::AttrContextObject>, v8::Local<v8::String> qualifiedName);
			CO_METHOD(getNamedItemNS, pins::NullPin<nodes::AttrContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
			CO_METHOD(setNamedItem, pins::NullPin<nodes::AttrContextObject>, pins::Pin<nodes::AttrContextObject> attr);
			CO_METHOD(setNamedItemNS, pins::NullPin<nodes::AttrContextObject>, pins::Pin<nodes::AttrContextObject> attr);
			CO_METHOD(removeNamedItem, pins::Pin<nodes::AttrContextObject>, v8::Local<v8::String> qualifiedName);
			CO_METHOD(removeNamedItemNS, pins::Pin<nodes::AttrContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
		private:
			pins::Property<nodes::ElementContextObject> element;
			std::vector<pins::Property<nodes::AttrContextObject>> attributeList;
			v8::UniquePersistent<v8::Map> attributeMap;
		};
	}
}