#pragma once

#include "dom/dom_core.h"
#include "abort_signal.h"

namespace cpp {
	namespace dom {

		class AbortControllerContextObject : public DOMContextObject {
		public:
			CO_READONLY_ATTRIBUTE(signal, pins::Pin<AbortSignalContextObject>) {
				return signal.pin(context->GetIsolate());
			}

			CO_METHOD(abort, void) {
				auto sig = this->signal.pin(context->GetIsolate());
				sig->signalAbort(context);
			}
		private:
			pins::Property<AbortSignalContextObject> signal;
		};
	}
}