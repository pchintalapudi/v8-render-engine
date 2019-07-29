#pragma once

#include "include/v8.h"
namespace Globals {
	class BrowsingContext {
	public:
		BrowsingContext(v8::Isolate* isolate);
	private:
		v8::Isolate* isolate;
	};
}