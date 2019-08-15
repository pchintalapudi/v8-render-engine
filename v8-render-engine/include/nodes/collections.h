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

		class ObservableListContextObject : public js_objects::BaseContextObject {
		protected:
			template<typename Collection, typename ReturnType>
			ReturnType internal_get(std::size_t i) {
				return static_cast<Collection*>(this)->get(i);
			}

			template<typename Collection>
			std::size_t length() {
				return static_cast<Collection*>(this)->size();
			}
		};

		class NodeListContextObject : ObservableListContextObject {

			nodes::NodeContextObject& get(std::size_t i) {
				return *this->src[i];
			}

			std::size_t size() {
				return this->src.size();
			}

			CO_INDEX(number, nodes::NodeContextObject&, std::size_t) {
				return this->get(i);
			}

			CO_READONLY_ATTRIBUTE(length, std::size_t) {
				return this->size();
			}

			CO_METHOD(item, nodes::NodeContextObject&, std::size_t i) {
				return this->get(i);
			}

		private:
			std::deque<nodes::NodeContextObject*> src;
		};

		using HTMLCollectionFilter = bool(*)(nodes::ElementContextObject* element);

#define HTMLCollectionInterface \
CO_READONLY_ATTRIBUTE(length, std::size_t);\
CO_METHOD(item, nodes::ElementContextObject*, std::size_t i);\
CO_METHOD(namedItem, nodes::ElementContextObject*, v8::Local<v8::String> name);

		class ComputedHTMLCollectionContextObject : ObservableListContextObject {
		public:
			HTMLCollectionInterface
		private:
			HTMLCollectionFilter filter;
			nodes::NodeContextObject* src;
			data_structs::BloomFilter<nodes::ElementContextObject*, 115, 7> bloomFilter;
			bool nodeList;
		};

		class HTMLCollectionContextObject : ObservableListContextObject {
		public:
			HTMLCollectionInterface
		private:

		};
	}
}