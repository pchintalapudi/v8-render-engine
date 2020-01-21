#pragma once

#include "node_list.h"

namespace cpp {
	namespace dom {

		namespace nodes {
			class ElementContextObject;
		}

		class HTMLCollectionContextObject : public DOMContextObject {

			CO_METHOD(item, pins::NullPin<nodes::ElementContextObject>, unsigned long index) {
				return index < this->elements.size() ? this->elements[index].pin(context->GetIsolate()) : pins::NullPin<nodes::ElementContextObject>::null(context->GetIsolate());
			}

			CO_READONLY_ATTRIBUTE(length) {
				return v8::Integer::New(context->GetIsolate(), static_cast<int32_t>(this->elements.size()));
			}

			CO_METHOD(namedItem, pins::NullPin<nodes::ElementContextObject>, v8::Local<v8::String> name);

		private:
			std::vector<pins::Property<nodes::ElementContextObject>> elements;
		};
	}
}