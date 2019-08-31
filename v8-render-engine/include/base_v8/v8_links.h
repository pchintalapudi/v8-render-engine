#pragma once

#include "v8.h"

namespace js_objects {
	namespace v8_links {

		v8::Isolate* createIsolate();

		void disposeIsolate(v8::Isolate* isolate);
	}
}