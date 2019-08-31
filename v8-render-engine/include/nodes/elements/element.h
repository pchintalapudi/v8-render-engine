#pragma once

#include "nodes/node.h"
#include "nodes/mixins.h"

namespace dom {

	namespace utils {
		class DOMTokenListContextObject;
		class NamedNodeMapContextObject;
	}

	namespace nodes {

		namespace elements {
			namespace html {
				class HTMLSlotElementContextObject;
			}
		}

		class AttrContextObject;
		class OpenShadowRoot;
		class ClosedShadowRoot;

		class ElementContextObject : public NodeContextObject {
		public:

			MIXIN_PARENT_NODE

				MIXIN_NON_DOCUMENT_TYPE_CHILD_NODE

				MIXIN_CHILD_NODE

				MIXIN_SLOTABLE

				CO_READONLY_ATTRIBUTE(namespaceURI, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(prefix, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(localName, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(tagName, v8::Local<v8::String>);

			//CEReactions
			CO_ATTRIBUTE(id, v8::Local<v8::String>);

			//CEReactions
			CO_ATTRIBUTE(className, v8::Local<v8::String>);

			//PutForwards = value
			CO_ATTRIBUTE(classList, utils::DOMTokenListContextObject&);

			//CEReactions
			CO_ATTRIBUTE(slot, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(hasAttributes, bool);

			CO_METHOD(getAttributeNames, v8::Local<v8::Array>);

			CO_METHOD(getAttribute, v8::Local<v8::Value>, v8::Local<v8::String> qualifiedName);

			CO_METHOD(getAttributeNS, v8::Local<v8::Value>, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);

			//CEReactions
			CO_METHOD(setAttribute, void, v8::Local<v8::String> qualifiedName, v8::Local<v8::String> value);

			//CEReactions
			CO_METHOD(setAttributeNS, void, v8::Local<v8::Value> ns, v8::Local<v8::String> localName, v8::Local<v8::String> value);

			//CEReactions
			CO_METHOD(removeAttribute, void, v8::Local<v8::String> qualifiedName);

			//CEReactions
			CO_METHOD(removeAttributeNS, void, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);

			//CEReactions
			CO_METHOD(toggleAttribute, void, v8::Local<v8::String> qualifiedName, bool force);

			CO_METHOD(hasAttribute, bool, v8::Local<v8::String> qualifiedName);

			CO_METHOD(hasAttributeNS, bool, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);

			CO_METHOD(getAttributeNode, AttrContextObject*, v8::Local<v8::String> qualifiedName);

			CO_METHOD(getAttributeNodeNS, AttrContextObject*, v8::Local<v8::Value> ns, v8::Local<v8::String> qualifiedName);

			//CEReactions
			CO_METHOD(setAttributeNode, AttrContextObject*, AttrContextObject& attr);

			//CEReactions
			CO_METHOD(setAttributeNodeNS, AttrContextObject*, AttrContextObject& attr);

			//CEReactions
			CO_METHOD(removeAttributeNode, AttrContextObject&, AttrContextObject& attr);

#define COMMA ,
			CO_METHOD(attachShadow, template<bool open> std::conditional<open COMMA OpenShadowRoot& COMMA ClosedShadowRoot&>);
#undef COMMA

			CO_READONLY_ATTRIBUTE(shadowRoot, v8::Local<v8::Value>);

			CO_METHOD(closest, ElementContextObject*, v8::Local<v8::String> selectors);

			CO_METHOD(matches, bool, v8::Local<v8::String> selectors);

			CO_METHOD(webkitMatchesSelector, bool, v8::Local<v8::String> selectors) {
				return this->matchesMETHOD(context, selectors);
			}

			CO_METHOD(getElementsByTagName, collections::ComputedHTMLCollectionContextObject&, v8::Local<v8::String> qualifiedName);

			CO_METHOD(getElementsByTagNameNS, collections::ComputedHTMLCollectionContextObject&, v8::Local<v8::String> ns, v8::Local<v8::String> localName);

			CO_METHOD(getElementsByClassName, collections::ComputedHTMLCollectionContextObject&, v8::Local<v8::String> classNames);

			//CEReactions
			CO_METHOD(insertAdjacentElement, ElementContextObject*, v8::Local<v8::String> whre, ElementContextObject& element);

			//CEReactions
			CO_METHOD(insertAdjacentText, void, v8::Local<v8::String> whre, v8::Local<v8::String> data);
		private:
			js_objects::JS_CPP_Property<elements::html::HTMLSlotElementContextObject> assignedSlot;
		};
	}
}