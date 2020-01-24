#pragma once

#include "dom/dom_core.h"

namespace cpp {
	namespace dom {

		namespace nodes {
			class NodeContextObject;
		}

		class NodeListContextObject : public DOMContextObject {
		public:

			CO_METHOD(item, pins::NullPin<nodes::NodeContextObject>, unsigned long index) {
				return index < nodes.size() ? nodes[index].pin(context->GetIsolate()) : pins::NullPin<nodes::NodeContextObject>::null(context->GetIsolate());
			}

			CO_READONLY_ATTRIBUTE(length, v8::Local<v8::Integer>) {
				return v8::Integer::New(context->GetIsolate(), static_cast<int32_t>(nodes.size()));
			}

			auto& operator[](std::size_t idx) {
				return this->nodes[idx];
			}

			auto size() {
				return this->nodes.size();
			}

			auto& vec() { return this->nodes; }

		private:
			std::vector<pins::Property<nodes::NodeContextObject>> nodes;
		};
	}
}