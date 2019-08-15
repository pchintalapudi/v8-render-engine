#pragma once

#include "v8.h"

namespace timing {
	double now(v8::Isolate* isolate);
}