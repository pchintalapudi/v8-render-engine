#pragma once

#include "dom/dom_core.h"
#include <set>

namespace cpp {
	namespace dom {
		class DOMTokenListContextObject : public DOMContextObject {
		public:
			CO_READONLY_ATTRIBUTE(length, v8::Local<v8::Integer>) {
				return v8::Uint32::New(context->GetIsolate(), static_cast<int32_t>(this->strings.size()));
			}

			CO_METHOD(item, v8::Local<v8::Value>, uint32_t index) {
				return index < this->strings.size() ? this->strings[index].Get(context->GetIsolate()).As<v8::Value>() : v8::Null(context->GetIsolate()).As<v8::Value>();
			}

			CO_METHOD(contains, v8::Local<v8::Boolean>, v8::Local<v8::String> token) {
				return v8::Boolean::New(context->GetIsolate(), this->set.Get(context->GetIsolate())->Has(context, token).FromJust());
			}

			CO_METHOD(add, std::optional<void>, std::vector<v8::Local<v8::String>> tokens) {
				for (auto token : tokens) {
					//TODO exceptions
				}
				auto set = this->set.Get(context->GetIsolate());
				for (auto token : tokens) {
					if (!set->Has(context, token).ToChecked()) {
						this->strings.emplace_back(context->GetIsolate(), token);
					}
				}
				//TODO run update steps
			}
			CO_METHOD(remove, std::optional<void>, std::vector<v8::Local<v8::String>> tokens) {
				for (auto token : tokens) {
					//TODO exceptions
				}
				auto set = this->set.Get(context->GetIsolate());
				auto filtered = std::vector<v8::Local<v8::String>>();
				for (auto token : tokens) {
					if (set->Has(context, token).ToChecked()) {
						filtered.push_back(token);
						set->Delete(context, token);
					}
				}
				this->strings.erase(std::remove_if(this->strings.begin(), this->strings.end(), [context, &filtered](v8::UniquePersistent<v8::String> str) {
					auto string = str.Get(context->GetIsolate());
					for (auto token : filtered) {
						if (string->StringEquals(token)) {
							return true;
						}
					}
					return false;
				}));
				//TODO run update steps
			}

			CO_METHOD(toggle, std::optional<v8::Local<v8::Boolean>>, v8::Local<v8::String> token, std::optional<bool> force) {
				//TODO exceptions
				auto set = this->set.Get(context->GetIsolate());
				if (set->Has(context, token).ToChecked()) {
					if (!force || !*force) {
						set->Delete(context, token);
						for (auto it = this->strings.begin(); it < this->strings.end(); ++it) {
							if (it->Get(context->GetIsolate())->StringEquals(token)) {
								this->strings.erase(it);
								break;
							}
						}
						//TODO Run update steps
						return { v8::Boolean::New(context->GetIsolate(), false) };
					}
					return { v8::Boolean::New(context->GetIsolate(), true) };
				}
				else {
					if (!force || *force) {
						set->Add(context, token);
						this->strings.emplace_back(context->GetIsolate(), token);
						//TODO run update steps
					}
				}
				return { v8::Boolean::New(context->GetIsolate(), false) };
			}
			CO_METHOD(replace, std::optional<v8::Local<v8::Boolean>>, v8::Local<v8::String> token, v8::Local<v8::String> newToken) {
				//TODO exceptions
				auto set = this->set.Get(context->GetIsolate());
				if (!set->Has(context, token).ToChecked()) {
					return { v8::Boolean::New(context->GetIsolate(), false) };
				}
				for (auto it = this->strings.begin(); it < this->strings.end(); ++it) {
					if (it->Get(context->GetIsolate())->StringEquals(token)) {
						it->Reset(context->GetIsolate(), newToken);
						break;
					}
				}
				//TODO run update steps
				return { v8::Boolean::New(context->GetIsolate(), true) };
			}

			//TODO
			CO_METHOD(supports, v8::Local<v8::Boolean>, v8::Local<v8::String> token);

		private:
			v8::UniquePersistent<v8::Set> set;
			std::vector<v8::UniquePersistent<v8::String>> strings;
		};
	}
}