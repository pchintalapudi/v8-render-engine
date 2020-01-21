#pragma once

#include "shadow_root.h"
#include "dom/collections/named_node_map.h"
#include "dom/collections/dom_token_list.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class ElementContextObject : public NodeContextObject {

				CO_READONLY_ATTRIBUTE(namespaceURI) {
					return this->ns.IsEmpty() ? v8::Null(context->GetIsolate()).As<v8::Value>() : this->ns.Get(context->GetIsolate()).As<v8::Value>();
				}
				CO_READONLY_ATTRIBUTE(prefix) {
					return this->nsPrefix.IsEmpty() ? v8::Null(context->GetIsolate()).As<v8::Value>() : this->nsPrefix.Get(context->GetIsolate()).As<v8::Value>();
				}
				CO_TRANSPARENT_GETTER(localName);
				CO_READONLY_ATTRIBUTE(tagName) {
					auto qualifiedName = this->nsPrefix.IsEmpty() ? this->localName.Get(context->GetIsolate()) : v8::String::Concat(context->GetIsolate(), this->nsPrefix.Get(context->GetIsolate()), v8::String::Concat(context->GetIsolate(), v8::String::NewFromUtf8(context->GetIsolate(), ":", v8::NewStringType::kNormal, 1).ToLocalChecked(), this->localName.Get(context->GetIsolate())));
					//TODO if node in html namespace && document == html document uppercase name
					return qualifiedName;
				}

				CO_ATTRIBUTE(id, v8::Local<v8::String>);
				CO_ATTRIBUTE(className, v8::Local<v8::String>);
				CO_PIN_GETTER(classList);

				CO_ATTRIBUTE(slot, v8::Local<v8::String>);

				CO_METHOD(hasAttributes, v8::Local<v8::Boolean>);
				CO_PIN_GETTER(attributes);
				CO_METHOD(getAttributeNames, v8::Local<v8::Array>);
				CO_METHOD(getAttribute, v8::Local<v8::Value>, v8::Local<v8::String> qualifiedName);
				CO_METHOD(getAttributeNS, v8::Local<v8::Value>, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
				CO_METHOD(setAttribute, void, v8::Local<v8::String> qualifiedName, v8::Local<v8::String> value);
				CO_METHOD(setAttributeNS, void, v8::Local<v8::Value> ns, v8::Local<v8::String> localName, v8::Local<v8::String> value);
				CO_METHOD(removeAttribute, void, v8::Local<v8::String> qualifiedName);
				CO_METHOD(removeAttributeNS, void, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
				CO_METHOD(toggleAttribute, v8::Local<v8::Boolean>, v8::Local<v8::String> qualifiedName, bool force);
				CO_METHOD(hasAttribute, bool, v8::Local<v8::String> qualifiedName);
				CO_METHOD(hasAttributeNS, bool, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);

				CO_METHOD(getAttributeNode, pins::NullPin<AttrContextObject>, v8::Local<v8::String> qualifiedName);
				CO_METHOD(getAttributeNodeNS, pins::NullPin<AttrContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
				CO_METHOD(setAttributeNode, pins::NullPin<AttrContextObject>, pins::Pin<AttrContextObject>);
				CO_METHOD(setAttributeNodeNS, pins::NullPin<AttrContextObject>, pins::Pin<AttrContextObject>);
				CO_METHOD(removeAttributeNodeNS, pins::Pin<AttrContextObject>, pins::Pin<AttrContextObject>);

				CO_METHOD(attachShadow, pins::Pin<ShadowRootContextObject>, v8::Local<v8::String> mode, bool delegatesFocus);
				CO_READONLY_ATTRIBUTE(shadowRoot) {
					auto pin = this->shadowRoot.pin(context->GetIsolate());
					return pin && !pin.pin()->isClosed() ? pin : pins::NullPin<ShadowRootContextObject>::null(context->GetIsolate());
				}

				CO_METHOD(closest, pins::NullPin<ElementContextObject>, v8::Local<v8::String> selectors);
				CO_METHOD(matches, v8::Local<v8::Boolean>, v8::Local<v8::String> selectors);
				CO_METHOD(webkitMatchesSelector, v8::Local<v8::Boolean>, v8::Local<v8::String> selectors) {
					return this->matches_METHOD(context, selectors);
				}

				CO_METHOD(getElementsByTagName, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::String> qualifiedName);
				CO_METHOD(getElementsByTagNameNS, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
				CO_METHOD(getElementsByClassName, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::String> className);

				CO_METHOD(insertAdjacentElement, std::optional<pins::NullPin<ElementContextObject>>, v8::Local<v8::String> whre, pins::Pin<ElementContextObject> element);
				CO_METHOD(insertAdjacentText, std::optional<pins::NullPin<TextContextObject>>, v8::Local<v8::String> whre, v8::Local<v8::String> data);
			private:
				v8::UniquePersistent<v8::String> ns;
				v8::UniquePersistent<v8::String> nsPrefix;
				v8::UniquePersistent<v8::String> localName;
				v8::UniquePersistent<v8::String> customElementState;
				v8::UniquePersistent<v8::String> customElementDefinition;
				v8::UniquePersistent<v8::String> is;
				pins::NullProperty<ShadowRootContextObject> shadowRoot;
				pins::Property<NamedNodeMapContextObject> attributes;
				pins::Property<DOMTokenListContextObject> classList;

				friend class cpp::dom::NamedNodeMapContextObject;
				friend class cpp::dom::DOMTokenListContextObject;
			};
		}
	}
}