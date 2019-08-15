#pragma once
#include "v8.h"
#include "js_maps.h"
#include "data_structs/enum_set.h"

namespace js_objects {
	//Reimplementation of NodeJS's ObjectWrap
	class BaseContextObject {
	public:
		BaseContextObject(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> templ, data_structs::EnumSet<ObjectType> types) : types(types) {
			v8::MaybeLocal<v8::Object> obj = templ->NewInstance(isolate->GetCurrentContext());
			if (!obj.IsEmpty()) {
				auto local = obj.ToLocalChecked();
				ref = v8::Persistent<v8::Object>(isolate, local);
				local->SetAlignedPointerInInternalField(0, this);
				this->weaken();
			}
		}

		static BaseContextObject* access(v8::Local<v8::Object> obj) {
			return static_cast<BaseContextObject*>(obj->GetAlignedPointerFromInternalField(0));
		}

		template<bool checked = true>
		v8::MaybeLocal<v8::Object> getRefFromThis(v8::Isolate* isolate) {
			return checked ? v8::MaybeLocal<v8::Object>(this->getRefFromThis<false>(isolate)) : this->ref.Get(isolate);
		}

		bool typeof(ObjectType type) { return this->types.contains(type); }

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
		std::size_t cppRefs;
	};

	template<typename CO>
	struct JSThrowableReturn {
		CO* returnValue;
		bool thrown;
	};

	template<typename CO>
	static JSThrowableReturn<CO> errorThrown() {
		return { nullptr, true };
	}

	template<typename V8>
	struct JSThrowablePrimitive {
		v8::Local<V8> primitive;
		bool thrown;
	};

	template<typename CPP_Primitive>
	struct CPPThrowablePrimitive {
		CPP_Primitive primitive;
		bool thrown;
	};

	template<typename ContextObject>
	class CPP_JS_Obj_Ref {
	public:

		CPP_JS_Obj_Ref() = default;

		CPP_JS_Obj_Ref(ContextObject* ref, v8::Isolate* isolate) : co(ref), obj(ref->getRefFromThisUnchecked(isolate)) {}

		ContextObject* cppCast() {
			return this->co;
		}

		v8::Local<v8::Object> handle(v8::Isolate* isolate) {
			return this->obj.Get(isolate);
		}

	private:
		ContextObject* co;
		v8::UniquePersistent<v8::Object> obj;
	};

	template<typename ContextObject>
	class Weak_CPP_JS_Obj_Ref {
	public:

		CPP_JS_Obj_Ref() = default;

		CPP_JS_Obj_Ref(ContextObject* ref, v8::Isolate* isolate) : co(ref), obj(ref->getRefFromThisUnchecked(isolate)) {}

		ContextObject* cppCastUnchecked() {
			return this->co;
		}

		v8::MaybeLocal<v8::Object> handle(v8::Isolate* isolate) {
			return v8::MaybeLocal<v8::Object>(this->obj.Get(isolate));
		}

	private:
		ContextObject* co;
		v8::Persistent<v8::Object> weakRef;
	};
}