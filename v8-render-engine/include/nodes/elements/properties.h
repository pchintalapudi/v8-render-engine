#pragma once

#include "base_v8/root.h"
#include "base_v8/macros.h"

namespace dom {

	namespace nodes {
		class AttrContextObject;
	}

	namespace utils {

		class NamedNodeMapContextObject : public js_objects::BaseContextObject {
			CO_READONLY_ATTRIBUTE(length, std::size_t);

			CO_METHOD(item, nodes::AttrContextObject*, std::size_t i);

			CO_METHOD(getNamedItem, nodes::AttrContextObject*, v8::Local<v8::String> qualifiedName);

			CO_METHOD(getNamedItemNS, nodes::AttrContextObject*, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);

			//CEReactions
			CO_METHOD(setNamedItem, nodes::AttrContextObject*, nodes::AttrContextObject& attr);

			//CEReactions
			CO_METHOD(setNamedItemNS, nodes::AttrContextObject*, nodes::AttrContextObject& attr);

			//CEReactions
			CO_METHOD(getNamedItem, nodes::AttrContextObject*, v8::Local<v8::String> qualifiedName);

			//CEReactions
			CO_METHOD(getNamedItemNS, nodes::AttrContextObject*, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
		};

		class DOMTokenListContextObject : public js_objects::BaseContextObject {
			CO_READONLY_ATTRIBUTE(length, std::size_t);

			CO_METHOD(item, v8::Local<v8::Value>, std::size_t index);

			CO_METHOD(contains, bool, v8::Local<v8::String> token);

			//CEReactions
			CO_METHOD(add, void, v8::Local<v8::Array> tokens);

			//CEReactions
			CO_METHOD(remove, void, v8::Local<v8::Array> tokens);

			//CEReactions
			CO_METHOD(toggle, bool, v8::Local<v8::String> token, bool force);

			//CEReactions
			CO_METHOD(replace, bool, v8::Local<v8::String> token, v8::Local<v8::String> newToken);

			CO_METHOD(supports, bool, v8::Local<v8::String> token);

			CO_ATTRIBUTE(value, v8::Local<v8::String>);

			CO_METHOD(toString, v8::Local<v8::String>) {
				return this->valueGET(isolate);
			}

			CO_INDEX(numIndexer, v8::Local<v8::Value>, std::size_t);
		};
	}
}