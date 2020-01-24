#pragma once

#include "node.h"
#include "dom/mixins/mixins.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class CharacterDataContextObject : public NodeContextObject {
			public:
				CO_INLINE_ATTRIBUTE_GETTER(data, v8::Local<v8::String>) {
					return this->data.Get(context->GetIsolate());
				}

				CO_INLINE_ATTRIBUTE_SETTER(data, v8::Local<v8::String>);

				CO_READONLY_ATTRIBUTE(length, v8::Local<v8::Integer>) {
					return v8::Integer::New(context->GetIsolate(), this->data.Get(context->GetIsolate())->Length());
				}

				//TODO
				CO_METHOD(substringData, std::optional<v8::Local<v8::String>>, unsigned long offset, unsigned long count);
				//TODO
				CO_METHOD(appendData, void, v8::Local<v8::String> data);
				//TODO
				CO_METHOD(insertData, void, unsigned long offset, v8::Local<v8::String> data);
				//TODO
				CO_METHOD(deleteData, void, unsigned long offset, unsigned long count);
				//TODO
				CO_METHOD(replaceData, void, unsigned long offset, unsigned long count, v8::Local<v8::String> data);

				MIXIN_NON_DOCUMENT_TYPE_CHILD_NODE;
				MIXIN_CHILD_NODE;

			private:
				v8::UniquePersistent<v8::String> data;
			};
		}
	}
}