#pragma once

#include <deque>
#include "base_v8/root.h"
#include "base_v8/macros.h"
#include "data_structs/bloom_filter.h"

namespace dom {

	namespace nodes {
		class NodeContextObject;
		class ElementContextObject;
	}

	namespace collections {

		class NodeListContextObject : public js_objects::BaseContextObject {
		public:

			js_objects::Pin<nodes::NodeContextObject> get(v8::Local<v8::Context> context, std::size_t i) const {
				return this->src[i].pin(context->GetIsolate());
			}

			auto back(v8::Local<v8::Context> context) const {
				return this->src.back().pin(context->GetIsolate());
			}

			std::size_t size() const {
				return this->src.size();
			}

			CO_INDEX(number, auto, std::size_t) {
				return this->get(context, i);
			}

			CO_READONLY_ATTRIBUTE(length, std::size_t) {
				return this->size();
			}

			CO_METHOD(item, auto, std::size_t i) {
				return this->get(context, i);
			}

		private:

			friend class dom::nodes::NodeContextObject;

			std::deque<js_objects::JS_CPP_Property<nodes::NodeContextObject>> src;
		};

		using HTMLCollectionFilter = bool(*)(nodes::ElementContextObject* element);

#define HTMLCollectionInterface \
CO_READONLY_ATTRIBUTE(length, std::size_t);\
CO_METHOD(item, auto, std::size_t i);\
CO_METHOD(namedItem, auto, v8::Local<v8::String> name);

		class ElementHTMLCollectionContextObject : public js_objects::BaseContextObject {
		public:
			HTMLCollectionInterface
		private:
			friend class dom::nodes::NodeContextObject;
			friend class dom::nodes::ElementContextObject;

			js_objects::JS_CPP_Property<nodes::NodeContextObject> source;
		};

		class ComputedHTMLCollectionContextObject : public js_objects::BaseContextObject {
		public:
			HTMLCollectionInterface
		private:
			friend class dom::nodes::ElementContextObject;
			HTMLCollectionFilter filter;
			js_objects::JS_CPP_Property<ElementHTMLCollectionContextObject> source;
		};
	}
}