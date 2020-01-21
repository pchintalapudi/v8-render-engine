#pragma once

#include <codecvt>

#include "node.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class CharacterDataContextObject : public NodeContextObject {

				CO_INLINE_ATTRIBUTE_GETTER(data) {
					return this->data.Get(context->GetIsolate());
				}

				CO_INLINE_ATTRIBUTE_SETTER(data, v8::Local<v8::String>);

				CO_READONLY_ATTRIBUTE(length) {
					return this->data.Get(context->GetIsolate())->Length();
				}

				CO_METHOD(substringData, std::optional<v8::Local<v8::String>>, unsigned long offset, unsigned long count) {
					auto str = this->data.Get(context->GetIsolate());
					if (offset > str->Length()) {
						//TODO throw error
						return {};
					}
					auto length = str->Utf8Length(context->GetIsolate()) + 1;
					auto cbuf = new char[length];
					str->WriteUtf8(context->GetIsolate(), cbuf, length);
					cbuf[length - 1] = 0;
					std::wstring_convert<std::codecvt_utf8_utf16<char16_t>> converter;
					std::wstring utf16 = converter.from_bytes(cbuf, cbuf + length);
					delete[] cbuf;
					std::wstring substr = utf16.substr(offset, count);
					std::wstring_convert<std::codecvt_utf8_utf16<char16_t>> rcvt;
					auto bytes = rcvt.to_bytes(substr);
					return { v8::String::NewFromUtf8(context->GetIsolate(), bytes.c_str(), v8::NewStringType::kNormal, bytes.size()).ToLocalChecked() };
				}
				CO_METHOD(appendData, void, v8::Local<v8::String> data);
				CO_METHOD(insertData, void, unsigned long offset, v8::Local<v8::String> data);
				CO_METHOD(deleteData, void, unsigned long offset, unsigned long count);
				CO_METHOD(replaceData, void, unsigned long offset, unsigned long count, v8::Local<v8::String> data);

			private:
				v8::UniquePersistent<v8::String> data;
			};
		}
	}
}