#pragma once

#include "dom/dom_core.h"
#include <set>

namespace cpp {
	namespace dom {
		class DOMTokenListContextObject : public DOMContextObject {
		public:
			CO_READONLY_ATTRIBUTE(length, v8::Local<v8::Integer>) {
				return v8::Uint32::New(context->GetIsolate(), static_cast<int32_t>(this->strings.size()));
			}

			CO_METHOD(item, v8::Local<v8::Value>, uint32_t index) {
				return index < this->strings.size() ? this->strings[index].Get(context->GetIsolate()).As<v8::Value>() : v8::Null(context->GetIsolate()).As<v8::Value>();
			}

			CO_METHOD(contains, v8::Local<v8::Boolean>, v8::Local<v8::String> token) {
				return v8::Boolean::New(context->GetIsolate(), this->set.Get(context->GetIsolate())->Has(context, token).FromJust());
			}

			CO_METHOD(add, void, std::vector<v8::Local<v8::String>> tokens);
			CO_METHOD(remove, void, std::set<v8::Local<v8::String>> tokens);

			CO_METHOD(toggle, v8::Local<v8::Boolean>, v8::Local<v8::String> token, std::optional<bool> force);
			CO_METHOD(replace, v8::Local<v8::Boolean>, v8::Local<v8::String> token, v8::Local<v8::String> newToken);

			CO_METHOD(supports, v8::Local<v8::Boolean>, v8::Local<v8::String> token);

		private:
			v8::UniquePersistent<v8::Set> set;
			std::vector<v8::UniquePersistent<v8::String>> strings;
		};
	}
}