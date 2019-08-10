#pragma once

#include "v8.h"

namespace js {
	template<bool intern>
	v8::Local<v8::String> allocate(const char* string) {
		ISOLATE
			ESCAPABLE_HANDLE_SCOPE
			return ESCAPE(v8::String::NewFromUtf8(string).ToLocalChecked());
	}
}