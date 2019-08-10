#pragma once

#include "v8.h"

namespace js {
	namespace string {
		template<bool intern>
		v8::Local<v8::String> allocate(const char* string, v8::Isolate* isolate) {
			return v8::String::NewFromUtf8(isolate, string).ToLocalChecked();
		}
	}
}