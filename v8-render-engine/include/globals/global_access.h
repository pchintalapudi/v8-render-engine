#pragma once

#include <optional>
#include "v8.h"

namespace dom {

	namespace nodes {
		class DocumentContextObject;
	}

	namespace window {
		class WindowContextObject;
	}

	namespace globals {
		js_objects::NullablePin<nodes::DocumentContextObject> getAssociatedDocument(v8::Local<v8::Context> context) {
			return js_objects::NullablePin<nodes::DocumentContextObject>();
		}
		js_objects::NullablePin<window::WindowContextObject> getAssociatedWindow(v8::Local<v8::Context> context);
	}
}