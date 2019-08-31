#pragma once

#include "v8.h"

namespace js_objects {

	class HashableV8String {
	public:
		HashableV8String(v8::Isolate* isolate, v8::Local<v8::String> string) : string(isolate, string) {}

		HashableV8String(v8::Isolate* isolate, const v8::UniquePersistent<v8::String>& string) : string(isolate, string.Get(isolate)) {}

		operator v8::UniquePersistent<v8::String>& () { return this->string; }

		bool operator<(const HashableV8String& other) const {
			return this->hash < other.hash;
		}

		bool operator==(const HashableV8String& other) const {
			auto isolate = v8::Isolate::GetCurrent();
			return this->string.Get(isolate)->StringEquals(other.string.Get(isolate));
		}

	private:
		v8::UniquePersistent<v8::String> string;
		int hash = 0;
	};

	template<typename Type>
	struct Nullable_JS_CPP_Property;
	template<typename Type>
	struct Pin;
	template<typename Type>
	struct NullablePin;

	template<typename Type>
	struct JS_CPP_Property {

		v8::UniquePersistent<v8::Object> jsRef;
		Type* cppRef;

		JS_CPP_Property(v8::Isolate* isolate, Type& cppRef) : jsRef(isolate, cppRef.toLocal(isolate)), cppRef(&cppRef) {}

		JS_CPP_Property(v8::UniquePersistent<v8::Object> jsRef, Type& cppRef) : jsRef(std::move(jsRef)), cppRef(&cppRef) {}

		JS_CPP_Property(v8::Isolate* isolate, Pin<Type> pin) : jsRef(isolate, pin.handle), cppRef(pin.cppRef) {}

		Nullable_JS_CPP_Property<Type> toNullable(v8::Isolate* isolate) const {
			return Nullable_JS_CPP_Property<Type>(v8::UniquePersistent<v8::Object>(isolate, jsRef.Get(isolate)), *cppRef);
		}

		Pin<Type> pin(v8::Isolate* isolate) const {
			return Pin<Type>(this->jsRef.Get(isolate), *cppRef);
		}

		Type* operator->() const {
			return cppRef;
		}

		Type& operator*() const {
			return *cppRef;
		}
	};

	template<typename Type>
	struct Nullable_JS_CPP_Property {
		v8::UniquePersistent<v8::Object> jsRef{};
		Type* cppRef = nullptr;

		Nullable_JS_CPP_Property() = default;

		Nullable_JS_CPP_Property(v8::Isolate* isolate, Type& cppRef) : jsRef(isolate, cppRef.toLocal(isolate)), cppRef(&cppRef) {}

		Nullable_JS_CPP_Property(v8::UniquePersistent<v8::Object> jsRef, Type& cppRef) : jsRef(std::move(jsRef)), cppRef(&cppRef) {}

		Nullable_JS_CPP_Property(v8::Isolate* isolate, NullablePin<Type> pinned) : jsRef(isolate, pinned.handle), cppRef(pinned.cppRef) {}

		Nullable_JS_CPP_Property(v8::Isolate* isolate, Pin<Type> pinned) : jsRef(isolate, pinned.handle), cppRef(pinned.cppRef) {}

		operator bool() const {
			return !!cppRef;
		}

		void nullify() {
			this->cppRef = nullptr;
			this->jsRef.Reset();
		}

		Nullable_JS_CPP_Property copy(v8::Isolate* isolate) const {
			if (cppRef) {
				return Nullable_JS_CPP_Property(isolate, *cppRef);
			}
			else return Nullable_JS_CPP_Property();
		}

		NullablePin<Type> pinNullable(v8::Isolate* isolate) const {
			return NullablePin<Type>(this->jsRef.Get(isolate), this->cppRef);
		}
	};

	template<typename Type>
	struct Weak_JS_CPP_Property {
		Weak_JS_CPP_Property(v8::Isolate* isolate, Type& obj) : weakRef(isolate, obj.getWeak()), obj(&obj) {
			this->weakRef.SetWeak();
		}

		std::pair<v8::MaybeLocal<v8::Object>, Type*> lock(v8::Isolate* isolate) const {
			v8::MaybeLocal<v8::Object> maybe = v8::MaybeLocal<v8::Object>(this->weakRef.Get(isolate));
			if (maybe.IsEmpty()) return std::make_pair(maybe, nullptr);
			else return std::make_pair(maybe, this->obj);
		}

	private:
		v8::Persistent<v8::Object> weakRef;
		Type* obj;
	};

	template<typename Type>
	struct Pin {
		v8::Local<v8::Object> handle;
		Type* cppRef;

		Pin(v8::Local<v8::Object> handle, Type& cppRef) : handle(handle), cppRef(&cppRef) {}

		operator v8::Local<v8::Object>() { return this->handle; }

		Type& operator*() {
			return *this->cppRef;
		}

		Type* operator->() {
			return this->cppRef;
		}

		static Pin<Type> create(v8::Isolate* isolate, Type& type) {
			return { type->getLocal(isolate), &type };
		}

		auto& escape(v8::EscapableHandleScope& ehs) const {
			ehs.Escape(this->handle);
			return *this;
		}

		template<typename T>
		Pin<T> cast() {
			return Pin<T>(handle, *static_cast<T*>(cppRef));
		}

		template<typename T>
		Pin<T> reinterpret() {
			return Pin<T>(handle, *reinterpret_cast<T*>(cppRef));
		}

		bool operator==(const Pin<Type>& other) const {
			return this->cppRef == other.cppRef;
		}

		bool operator==(const NullablePin<Type>& other) const {
			return this->cppRef == other.cppRef;
		}
	};

	template<typename Type>
	struct NullablePin {
		v8::Local<v8::Object> handle;
		Type* cppRef;

		NullablePin() = default;

		NullablePin(v8::Local<v8::Object> handle, Type* type) : handle(handle), cppRef(type) {}

		NullablePin(Pin<Type> pin) : handle(pin.handle), cppRef(pin.cppRef) {}

		static NullablePin createNullable(v8::Isolate* isolate, Type* type) {
			return type ? NullablePin{type->toLocal(isolate), type} : NullablePin{v8::Local<v8::Object>(), nullptr};
		}

		Pin<Type> pin() {
			return Pin(handle, *cppRef);
		}

		template<typename T>
		NullablePin<T> cast() {
			return NullablePin(handle, static_cast<T*>(cppRef));
		}

		template<typename T>
		auto reinterpret() {
			return NullablePin(handle, reinterpret_cast<T*>(cppRef));
		}

		bool operator==(const NullablePin<Type>& other) {
			return this->cppRef == other.cppRef;
		}

		bool operator==(const Pin<Type>& other) {
			return this->cppRef == other.cppRef;
		}

		operator bool() {
			return !!this->cppRef;
		}
	};

	enum class VoidSuccess {
		SUCCESS, __COUNT__
	};
}