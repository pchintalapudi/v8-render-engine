#pragma once

#include "node.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class AttrContextObject : public NodeContextObject {
			public:
				CO_READONLY_ATTRIBUTE(namespaceURI, v8::Local<v8::Value>) {
					return this->ns.IsEmpty() ? v8::Null(context->GetIsolate()).As<v8::Value>() : this->ns.Get(context->GetIsolate()).As<v8::Value>();
				}

				CO_READONLY_ATTRIBUTE(namespacePrefix, v8::Local<v8::Value>) {
					return this->nsPrefix.IsEmpty() ? v8::Null(context->GetIsolate()).As<v8::Value>() : this->nsPrefix.Get(context->GetIsolate()).As<v8::Value>();
				}

				CO_TRANSPARENT_GETTER(localName);

				CO_READONLY_ATTRIBUTE(name, v8::Local<v8::String>) {
					return this->qualifiedName.Get(context->GetIsolate());
				}

				CO_TRANSPARENT_GETTER(value);
				//TODO
				CO_WRITEONLY_ATTRIBUTE(value, v8::Local<v8::String>);

				CO_PIN_GETTER(ownerElement);

				CO_READONLY_ATTRIBUTE(specified, v8::Local<v8::Boolean>) {
					return v8::Boolean::New(context->GetIsolate(), true);
				}

			private:
				v8::UniquePersistent<v8::String> ns;
				v8::UniquePersistent<v8::String> nsPrefix;
				v8::UniquePersistent<v8::String> localName;
				v8::UniquePersistent<v8::String> value;
				v8::UniquePersistent<v8::String> qualifiedName;
				pins::NullProperty<nodes::ElementContextObject> ownerElement;
				friend class ElementContextObject;
				friend class NodeContextObject;
			};
		}
	}
}