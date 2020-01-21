#pragma once

#include "v8.h"

namespace cpp {
	namespace obj {
		class BaseContextObject {
		public:
			static BaseContextObject* uncheckedAccess(v8::Local<v8::Object> obj) {
				return static_cast<BaseContextObject*>(obj->GetAlignedPointerFromInternalField(0));
			}

			v8::Local<v8::Object> toLocal(v8::Isolate* isolate) const {
				return this->ref.Get(isolate);
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
		};
	}
}