#pragma once
#include "dom/nodes/document.h"

namespace cpp {
	namespace dom {
		namespace nodes {

			class ShadowRootContextObject : public DocumentFragmentContextObject {
			public:

				MIXIN_DOCUMENT_OR_SHADOW_ROOT

				CO_READONLY_ATTRIBUTE(mode) {
					return v8::String::NewFromUtf8(context->GetIsolate(), this->closed ? "closed" : "open", v8::NewStringType::kNormal, this->closed ? 6 : 4);
				}
				CO_PIN_GETTER(host);
				virtual pins::NullPin<EventTargetContextObject> getTheParent(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event);

				bool isClosed() {
					return this->closed;
				}

			private:
				bool closed;
				pins::Property<ElementContextObject> host;
				//TODO onslotchange
				bool delegatesFocus;
			};
		}
	}
}