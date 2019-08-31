#pragma once

#include <map>
#include <optional>
#include <set>

#include "bindings.h"
#include "v8.h"
#include "js_maps.h"
#include "data_structs/enum_set.h"

namespace dom {
	namespace window {
		class WindowContextObject;
	}
}

namespace js_objects {

	template<typename T>
	struct JS_CPP_Property;

	class BaseContextObject;

	struct BrowsingContextProperties {
		std::set<BaseContextObject*> natives;
		std::map<const char*, v8::UniquePersistent<v8::Value>> v8PropertyCache;
		dom::window::WindowContextObject* window;
		bool onBeforeUnload;
		bool cleaned;

		operator bool() {
			return !this->cleaned;
		}

		thread_local static std::map<v8::UniquePersistent<v8::Context>, BrowsingContextProperties> contexts;

		static v8::Local<v8::Context> initRenderingProcess(v8::Isolate* isolate);

		static bool preDestroyRenderingProcess(v8::Local<v8::Context> context);

		static void destroyRenderingProcess(v8::Local<v8::Context> context);
	};

	//Reimplementation of NodeJS's ObjectWrap
	class BaseContextObject {
	public:

		//Remains valid until enclosing handle scope is destroyed or new reference is made
		template<ObjectType type>
		BaseContextObject(v8::Local<v8::Context> context, bool& error) : types(ObjectTemplateInstantiator<type>::types()) {
			v8::Local<v8::ObjectTemplate> templ = ObjectTemplateInstantiator<type>::getTemplate(context);
			auto instanceMaybe = templ->NewInstance(context);
			if (instanceMaybe.IsEmpty()) {
				error = true;
				return;
			}
			auto instance = instanceMaybe.ToLocalChecked();
			instance->SetAlignedPointerInInternalField(0, this);
			this->ref = v8::Persistent<v8::Object>(context->GetIsolate(), instance);
			this->ref.SetWeak(this, BaseContextObject::del, v8::WeakCallbackType::kParameter);
		}

		static BaseContextObject* uncheckedAccess(v8::Local<v8::Object> obj) {
			return static_cast<BaseContextObject*>(obj->GetAlignedPointerFromInternalField(0));
		}

		template<ObjectType type>
		static std::optional<Pin<typename ObjectTemplateInstantiator<type>::type>> checkedAccess(v8::Local<v8::Object> obj) {
			if (obj->InternalFieldCount()) {
				auto ptr = BaseContextObject::uncheckedAccess(obj);
				if (ptr) {
					if (ptr->typeof(type)) {
						return std::make_optional(Pin(obj, static_cast<typename ObjectTemplateInstantiator<type>::type*>(ptr)));
					}
				}
			}
			return {};
		}

		template<typename ...Pack>
		bool typeof(Pack... type) const { return this->types.any(type...); }

		v8::Local<v8::Object> toLocal(v8::Isolate* isolate) const {
			return this->ref.Get(isolate);
		}

		template<typename Type>
		js_objects::Pin<Type> pin(v8::Isolate* isolate) {
			return js_objects::Pin<Type>(this->toLocal(isolate), *static_cast<Type*>(this));
		}

		auto& getWeak() const {
			return this->ref;
		}

		virtual ~BaseContextObject() {
			this->ref.ClearWeak();
			this->ref.Reset();
		}

	private:

		static void del(const v8::WeakCallbackInfo<BaseContextObject>& data) {

			delete data.GetParameter();
		}

		void weaken() {
			this->ref.SetWeak(this, BaseContextObject::del, v8::WeakCallbackType::kParameter);
		}

		v8::Persistent<v8::Object> ref;
		data_structs::EnumSet<ObjectType> types;
	};
}