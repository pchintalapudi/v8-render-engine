#pragma once

#include "base_v8/pins.h"
#include "base_v8/native.h"
#include "data_structs/enum_set.h"

namespace cpp {
	namespace dom {
		enum class DOMType {
			Event,
			EventTarget,
			AbortController,
			AbortSignal,
			NodeList,
			HTMLCollection,
			MutationObserver,
			MutationRecord,
			Node,
			Document,
			XMLDocument,
			DOMImplementation,
			DocumentType,
			DocumentFragment,
			ShadowRoot,
			Element,
			NamedNodeMap,
			Attr,
			CharacterData,
			Text,
			CDATASection,
			ProcessingInstruction,
			Comment,
			AbstractRange,
			StaticRange,
			Range,
			NodeIterator,
			TreeWalker,
			NodeFilter,
			DOMTokenList,
			XPathResult,
			XPathExpression,
			XPathNSResolver,
			XPathEvaluatorBase,
			XPathEvaluator,
			__COUNT__
		};

		class DOMContextObject : public obj::BaseContextObject {
		public:
			bool domTypeof(DOMType type) {
				return this->types.contains(type);
			}
		private:
			data_structs::EnumSet<DOMType> types;
		};
	}
}