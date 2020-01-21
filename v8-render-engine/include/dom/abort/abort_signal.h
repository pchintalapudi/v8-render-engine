#pragma once

#include "dom/dom_core.h"

namespace cpp {
	namespace dom {

		class AbortControllerContextObject;

		class AbortSignalContextObject : DOMContextObject {

			CO_READONLY_ATTRIBUTE(aborted) { return v8::Boolean::New(context->GetIsolate(), this->aborted); }

			void addAlgorithm(v8::Local<v8::Context> context, v8::Local<v8::Function> algorithm) {
				if (!this->aborted) {
					auto set = this->abortAlgorithms.Get(context->GetIsolate());
					set->Add(context, algorithm);
				}
			}

			void removeAlgorithm(v8::Local<v8::Context> context, v8::Local<v8::Function> algorithm) {
				auto set = this->abortAlgorithms.Get(context->GetIsolate());
				set->Delete(context, algorithm);
			}

			void signalAbort(v8::Local<v8::Context> context) {
				if (!this->aborted) {
					this->aborted = true;
					auto set = this->abortAlgorithms.Get(context->GetIsolate())->AsArray();
					for (int i = 0; i < set->Length(); i++) {
						//TODO function calls
					}
					this->abortAlgorithms.Get(context->GetIsolate())->Clear();
					//TODO fire event
				}
			}

		private:
			bool aborted;
			v8::UniquePersistent<v8::Set> abortAlgorithms;

			friend class AbortControllerContextObject;
		};
	}
}