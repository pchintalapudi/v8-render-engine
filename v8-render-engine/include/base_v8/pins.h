#pragma once

#include <type_traits>
#include "v8.h"

#include "macro_utils.h"

namespace cpp {
	namespace pins {

		template<typename Type>
		class _native_local {
		public:
			v8::Local<v8::Value> handle;
			Type* native;
			_native_local(v8::Local<v8::Value> handle, Type* native) :handle(handle), native(native) {}

			template<typename Other>
			std::enable_if_t<std::is_base_of_v<Other, Type> || std::is_base_of_v<Type, Other>, bool> operator==(const _native_local<Other>& obj) {
				return obj.native == this->native;
			}
			template<typename Other>
			std::enable_if_t<std::is_base_of_v<Other, Type> || std::is_base_of_v<Type, Other>, bool> operator!=(const _native_local<Other>& obj) {
				return obj.native != this->native;
			}
			bool operator==(v8::Local<v8::Value> other) {
				return this->handle == other;
			}
			bool operator!=(v8::Local<v8::Value> other) {
				return !(*this == other);
			}
			v8::Local<v8::Value> js_handle() {
				return this->handle;
			}
		};

		template<typename Type>
		class Pin : public _native_local<Type> {
		public:
			Pin(const Pin<Type>& other) = default;
			Pin(v8::Local<v8::Object> handle, Type* native) :_native_local<Type>(handle, native) {}

			Type& operator*() {
				return *this->native;
			}

			Type* operator->() {
				return this->native;
			}

			template<typename Other>
			std::enable_if_t<std::is_base_of_v<Other, Type>, Pin<Other>&> cast() {
				return reinterpret_cast<Pin<Other>&>(*this);
			}

			template<typename Other>
			std::enable_if_t<std::is_base_of_v<Type, Other> or std::is_base_of_v<Other, Type>, Pin<Other>&> downcast() {
				return reinterpret_cast<Pin<Other>&>(*this);
			}
		};

		template<typename Type>
		class NullPin :public _native_local<Type> {
		public:
			NullPin(Pin<Type> other) :_native_local<Type>(other.handle, other.native) {}

			NullPin(v8::Local<v8::Value> handle, Type* native) :_native_local<Type>(handle, native) {}


			static NullPin<Type> null(v8::Isolate* isolate) {
				return NullPin<Type>(v8::Null(isolate), nullptr);
			}

			static NullPin<Type> undefined(v8::Isolate* isolate) {
				return NullPin<Type>(v8::Undefined(isolate), nullptr);
			}

			Pin<Type>& pin() {
				return reinterpret_cast<Pin<Type>&>(*this);
			}

			template<typename Other>
			NullPin<Other>& cast() {
				return reinterpret_cast<NullPin<Other>&>(*this);
			}

			operator bool() {
				return !!this->native;
			}

			bool empty() {
				return this->js_handle().IsEmpty();
			}

			void reset(v8::Isolate* isolate) {
				this->handle.Clear();
				this->native = nullptr;
			}
		};

		template<typename Type, bool nullable>
		class WeakPin {
		private:
			v8::Local<std::conditional_t<nullable, v8::Value, v8::Object>> handle;
			Type* native;
		public:
			WeakPin(v8::Local<v8::Object> handle, Type* native) : handle(handle), native(native) {}

			operator bool() {
				return !this->handle.IsEmpty();
			}

			auto pin() {
				return std::conditional_t<nullable, NullPin<Type>, Pin<Type>>(this->handle, this->native);
			}
		};

		template<typename Type>
		class Property {
		private:
			v8::UniquePersistent<v8::Object> handle;
			Type* native;
		public:

			Property(v8::Isolate* isolate, Pin<Type> local) : handle(isolate, local.js_handle().As<v8::Object>()), native(local.native) {}

			Pin<Type> pin(v8::Isolate* isolate) {
				return Pin<Type>(this->handle.Get(isolate), this->native);
			}

			void set(v8::Isolate* isolate, Pin<Type> local) {
				this->handle.Reset(isolate, local.handle.As<v8::Object>());
				this->native = local.native;
			}
		};

		template<typename Type>
		class NullProperty {
		private:
			v8::UniquePersistent<v8::Value> handle;
			Type* native;
		public:
			NullProperty(v8::Isolate* isolate, Pin<Type> local) : handle(isolate, local.js_handle()), native(local.native) {}
			NullProperty(v8::Isolate* isolate, NullPin<Type> local) : handle(isolate, local.js_handle()), native(local.native) {}

			NullPin<Type> pin(v8::Isolate* isolate) {
				return NullPin<Type>{this->handle.Get(isolate).As<v8::Object>(), this->native};
			}

			void set(v8::Isolate* isolate, _native_local<Type> local) {
				this->handle.Reset(isolate, local.js_handle());
				this->native = local.native;
			}

			void reset(v8::Isolate* isolate) {
				this->handle.Reset(isolate, v8::Null(isolate));
				this->native = nullptr;
			}
		};

		template<typename Type, bool nullable>
		class WeakProperty {
		private:
			typedef std::conditional_t<nullable, v8::Value, v8::Object> HandleType;
			v8::UniquePersistent<HandleType> handle;
			Type* native;
		public:

			WeakProperty(v8::Isolate* isolate, v8::Local<HandleType> handle, Type* native) : handle(isolate, handle), native(native) {
				this->handle.SetWeak();
			}

			WeakPin<Type, nullable> pin(v8::Isolate* isolate) {
				return WeakPin<Type, nullable>(this->handle.Get(isolate), this->native);
			}
		};
	}
}