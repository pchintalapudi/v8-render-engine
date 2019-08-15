#pragma once

#include "document.h"

namespace dom {
	namespace nodes {
		class OpenShadowRoot : public DocumentFragmentContextObject {
		public:
			CO_READONLY_ATTRIBUTE(mode, v8::Local<v8::String>) {
				if (this->open.IsEmpty()) {
					auto o = v8::String::NewFromUtf8(isolate, "open");
					if (!o.IsEmpty()) {
						auto l = o.ToLocalChecked();
						this->open = v8::Persistent<v8::String>(isolate, l);
						return l;
					}
					else {
						return v8::Local<v8::String>();
					}
				}
				return this->open.Get(isolate);
			}

			CO_READONLY_ATTRIBUTE(host, ElementContextObject&);
		private:
			static v8::Persistent<v8::String> open;
		};

		class ClosedShadowRoot : public DocumentFragmentContextObject {
		public:
			CO_READONLY_ATTRIBUTE(mode, v8::Local<v8::String>) {
				if (this->closed.IsEmpty()) {
					auto o = v8::String::NewFromUtf8(isolate, "closed");
					if (!o.IsEmpty()) {
						auto l = o.ToLocalChecked();
						this->closed = v8::Persistent<v8::String>(isolate, l);
						return l;
					}
					else {
						return v8::Local<v8::String>();
					}
				}
				return this->closed.Get(isolate);
			}

			CO_READONLY_ATTRIBUTE(host, ElementContextObject&);
		private:
			static v8::Persistent<v8::String> closed;
		};
	}
}