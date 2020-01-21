#pragma once

#include "node.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class AttrContextObject : public NodeContextObject {

				CO_READONLY_ATTRIBUTE(namespaceURI) {
					return this->ns.IsEmpty() ? v8::Null(context->GetIsolate()).As<v8::Value>() : this->ns.Get(context->GetIsolate()).As<v8::Value>();
				}

				CO_READONLY_ATTRIBUTE(namespacePrefix) {
					return this->nsPrefix.IsEmpty() ? v8::Null(context->GetIsolate()).As<v8::Value>() : this->nsPrefix.Get(context->GetIsolate()).As<v8::Value>();
				}

				CO_TRANSPARENT_GETTER(localName);

				CO_READONLY_ATTRIBUTE(name) {
					return this->nsPrefix.IsEmpty() ? this->localName.Get(context->GetIsolate()) : v8::String::Concat(context->GetIsolate(), this->nsPrefix.Get(context->GetIsolate()), v8::String::Concat(context->GetIsolate(), v8::String::NewFromUtf8(context->GetIsolate(), ":", v8::NewStringType::kNormal, 1).ToLocalChecked(), this->localName.Get(context->GetIsolate())));
				}

				CO_ATTRIBUTE(value, v8::Local<v8::String>);

				CO_PIN_GETTER(ownerElement);

				CO_READONLY_ATTRIBUTE(specified) {
					return v8::Boolean::New(context->GetIsolate(), true);
				}

			private:
				v8::UniquePersistent<v8::String> ns;
				v8::UniquePersistent<v8::String> nsPrefix;
				v8::UniquePersistent<v8::String> localName;
				v8::UniquePersistent<v8::String> value;
				pins::NullProperty<nodes::ElementContextObject> ownerElement;
			};
		}
	}
}