#pragma once

#include "dom/nodes/node.h"

namespace cpp {
	namespace dom {
		class TreeWalkerContextObject : public DOMContextObject {
		public:
			CO_PIN_GETTER(root);
			CO_PIN_GETTER(currentNode);
			CO_PRIMITIVE_GETTER(whatToShow, v8::Integer);
			CO_TRANSPARENT_GETTER(filter);

			CO_METHOD(parentNode, pins::NullPin<nodes::NodeContextObject>) {
				pins::NullPin<nodes::NodeContextObject> node = this->currentNode.pin(context->GetIsolate());
				auto root = this->root.pin(context->GetIsolate());
				while (node && node != root) {
					node = node.pin()->parent_GET(context);
					if (node && true) {//TODO filter node
						this->currentNode.set(context->GetIsolate(), node.pin());
						return node;
					}
				}
				return pins::NullPin<nodes::NodeContextObject>::null(context->GetIsolate());
			}

			//TODO
			pins::NullPin<nodes::NodeContextObject> traverseChildren(v8::Local<v8::Context> context, bool first);

			CO_METHOD(firstChild, pins::NullPin<nodes::NodeContextObject>) {
				return this->traverseChildren(context, true);
			}
			CO_METHOD(lastChild, pins::NullPin<nodes::NodeContextObject>) {
				return this->traverseChildren(context, false);	
			}

			//TODO
			pins::NullPin<nodes::NodeContextObject> traverseSiblings(v8::Local<v8::Context> context, bool previous);

			CO_METHOD(previousSibling, pins::NullPin<nodes::NodeContextObject>) {
				return this->traverseSiblings(context, true);
			}
			CO_METHOD(nextSibling, pins::NullPin<nodes::NodeContextObject>) {
				return this->traverseSiblings(context, false);
			}
			//TODO
			CO_METHOD(previousNode, pins::NullPin<nodes::NodeContextObject>);
			//TODO
			CO_METHOD(nextNode, pins::NullPin<nodes::NodeContextObject>);
		private:
			pins::Property<nodes::NodeContextObject> root;
			pins::Property<nodes::NodeContextObject> currentNode;
			uint32_t whatToShow;
			v8::UniquePersistent<v8::Function> filter;
		};
	}
}