#pragma once
#include "dom/nodes/document.h"

namespace cpp {
	namespace dom {
		namespace nodes {

			class ShadowRootContextObject : public DocumentFragmentContextObject {
			public:

				MIXIN_DOCUMENT_OR_SHADOW_ROOT

				CO_READONLY_ATTRIBUTE(mode, v8::Local<v8::String>) {
					return v8::String::NewFromUtf8(context->GetIsolate(), this->closed ? "closed" : "open", v8::NewStringType::kNormal, this->closed ? 6 : 4).ToLocalChecked();
				}
				CO_READONLY_ATTRIBUTE(host, auto) {
					return this->host.pin(context->GetIsolate()).pin();
				}
				virtual pins::NullPin<EventTargetContextObject> getTheParent(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event);

				bool isClosed() {
					return this->closed;
				}

			private:
				bool closed;
				//TODO onslotchange
				bool delegatesFocus;
			};
		}
	}
}