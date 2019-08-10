#pragma once
#include <string>
#include "v8.h"
#include "dom_exception_types.h"

namespace dom {
	namespace error {
		typedef std::string Message;
		class DOMExceptionContextObject {
		public:
			DOMExceptionContextObject(DOMErrorType name, Message message) : name(std::move(name)), message(std::move(message)) {}

			const DOMErrorType& nameGET() {
				return this->name;
			}

			const Message& messageGET() {
				return this->message;
			}

		private:
			DOMErrorType name;
			Message message;
		};

		v8::Local<v8::Value> getDOMException(DOMErrorType name, Message message, v8::Isolate* isolate);
	}
}

namespace v8_init {
	void init_DOMException(v8::Isolate* isolate);
}