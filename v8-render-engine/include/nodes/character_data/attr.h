#pragma once

#include "nodes/node.h"

namespace dom {
	namespace nodes {

		class ElementContextObject;

		class AttrContextObject : public NodeContextObject {
		public:
			CO_READONLY_ATTRIBUTE(namespaceURI, v8::Local<v8::Value>);

			CO_READONLY_ATTRIBUTE(prefix, v8::Local<v8::Value>);

			CO_READONLY_ATTRIBUTE(localName, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(name, v8::Local<v8::String>);

			//CEReactions
			CO_ATTRIBUTE(value, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(ownerElement, ElementContextObject*);

			CO_READONLY_ATTRIBUTE(specified, bool) {
				return true;
			}
		};
	}
}