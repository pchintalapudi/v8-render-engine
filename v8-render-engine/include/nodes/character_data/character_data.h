#pragma once

#include "nodes/node.h"

namespace dom {
	namespace nodes {
		class CharacterDataContextObject : public NodeContextObject {
		public:
			CO_ATTRIBUTE(data, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(length, std::size_t);

			CO_METHOD(substringData, v8::Local<v8::String>, std::size_t offset, std::size_t count);

			CO_METHOD(appendData, void, v8::Local<v8::String> data);

			CO_METHOD(insertData, void, std::size_t offset, v8::Local<v8::String> data);

			CO_METHOD(deleteData, void, std::size_t offset, std::size_t count);

			CO_METHOD(replaceData, void, std::size_t offset, std::size_t count, v8::Local<v8::String> data);
		};

		class TextContextObject : public CharacterDataContextObject {
		public:
			CO_METHOD(splitText, TextContextObject&, std::size_t offset);

			CO_READONLY_ATTRIBUTE(wholeText, v8::Local<v8::String>);
		};

		class CDATASectionContextObject : public TextContextObject {
		};

		class ProcessingInstructionContextObject : public CharacterDataContextObject {
		public:
			CO_READONLY_ATTRIBUTE(target, v8::Local<v8::String>);
		};

		class CommentContextObject : public CharacterDataContextObject {};
	}
}