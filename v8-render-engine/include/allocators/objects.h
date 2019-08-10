#pragma once

#include "v8.h"
#include "macros.h"

namespace js {
	namespace objects {

		template<typename O>
		void delete_native_memory(v8::Persistent<v8::Object> obj, void*) {
			ISOLATE
				obj.Get(isolate)
			obj.Reset();
			obj.ClearWeak();
		}

		template<typename O>
		v8::Local<v8::Object> allocate(v8::Local<v8::ObjectTemplate> templ, v8::Isolate* isolate) {
			CONTEXT
				auto object = templ->NewInstance(context).ToLocalChecked();
			v8::Persistent<v8::Object> weakPersistent = v8::Persistent<v8::Object>(isolate, object);
			weakPersistent.SetWeak<void>(nullptr, js::objects::delete_native_memory<O>, v8::WeakCallbackType::kParameter);
		}
	}
}