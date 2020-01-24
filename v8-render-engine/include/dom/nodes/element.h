#pragma once

#include "shadow_root.h"
#include "dom/collections/named_node_map.h"
#include "dom/collections/dom_token_list.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class ElementContextObject : public NodeContextObject {
			public:
				CO_READONLY_ATTRIBUTE(namespaceURI, auto) {
					return this->ns.IsEmpty() ? v8::Null(context->GetIsolate()).As<v8::Value>() : this->ns.Get(context->GetIsolate()).As<v8::Value>();
				}
				CO_READONLY_ATTRIBUTE(prefix, auto) {
					return this->nsPrefix.IsEmpty() ? v8::Null(context->GetIsolate()).As<v8::Value>() : this->nsPrefix.Get(context->GetIsolate()).As<v8::Value>();
				}
				CO_TRANSPARENT_GETTER(localName);
				CO_READONLY_ATTRIBUTE(tagName, auto) {
					return this->htmlUppercasedQualifiedName.Get(context->GetIsolate());
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
				CO_READONLY_ATTRIBUTE(shadowRoot, auto) {
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

				MIXIN_PARENT_NODE;
				MIXIN_NON_DOCUMENT_TYPE_CHILD_NODE;
				MIXIN_CHILD_NODE;
				MIXIN_SLOTABLE;

				auto getShadow(v8::Local<v8::Context> context) { return this->shadowRoot.pin(context->GetIsolate()); }
			private:
				v8::UniquePersistent<v8::String> ns;
				v8::UniquePersistent<v8::String> nsPrefix;
				v8::UniquePersistent<v8::String> localName;
				v8::UniquePersistent<v8::String> customElementState;
				v8::UniquePersistent<v8::String> customElementDefinition;
				v8::UniquePersistent<v8::String> is;
				v8::UniquePersistent<v8::String> htmlUppercasedQualifiedName;
				pins::NullProperty<ShadowRootContextObject> shadowRoot;
				pins::Property<NamedNodeMapContextObject> attributes;
				pins::Property<DOMTokenListContextObject> classList;

				friend class cpp::dom::NamedNodeMapContextObject;
				friend class cpp::dom::DOMTokenListContextObject;
				friend class StackDescendantIterator;
				friend class NodeContextObject;
			};

			class StackDescendantIterator {
				pins::Pin<nodes::NodeContextObject> root;
				bool init;
				std::vector<pins::Pin<nodes::NodeContextObject>> stack;
			public:

				StackDescendantIterator(pins::Pin<nodes::NodeContextObject> root) :root(root), init(false) {}

				auto operator->() {
					return init ? this->stack.back().operator->() : root.operator->();
				}

				auto& operator*() {
					return init ? *this->stack.back() : *root;
				}

				operator bool() {
					return init && !this->stack.empty();
				}

				auto& next(v8::Local<v8::Context> context) {
					if (init) {
						auto& children = stack.back()->childNodes.pin(context->GetIsolate())->vec();
						if (!children.empty()) {
							stack.push_back(children[0].pin(context->GetIsolate()));
						}
						else {
							auto nextSibling = stack.back()->nextSibling.pin(context->GetIsolate());
							while (!nextSibling && stack.size()) {
								stack.pop_back();
								nextSibling = stack.back()->nextSibling.pin(context->GetIsolate());
							}
							if (nextSibling) {
								stack.back() = nextSibling.pin();
							}
						}
					}
					else {
						this->init = true;
						auto& children = root->childNodes.pin(context->GetIsolate())->vec();
						if (!children.empty()) {
							this->stack.push_back(children[0].pin(context->GetIsolate()));
						}
					}
					return *this;
				}

				auto& shadowNext(v8::Local<v8::Context> context) {
					if (init) {
						auto& children = stack.back()->childNodes.pin(context->GetIsolate())->vec();
						if (!children.empty()) {
							stack.push_back(children[0].pin(context->GetIsolate()));
						}
						else {
							if (stack.back()->domTypeof(DOMType::Element)) {
								auto shadow = stack.back().downcast<ElementContextObject>()->shadowRoot.pin(context->GetIsolate());
								if (shadow) {
									stack.push_back(shadow.pin().cast<NodeContextObject>());
									return *this;
								}
							}
							auto nextSibling = stack.back()->nextSibling.pin(context->GetIsolate());
							while (!nextSibling && stack.size()) {
								stack.pop_back();
								nextSibling = stack.back()->nextSibling.pin(context->GetIsolate());
							}
							if (nextSibling) {
								stack.back() = nextSibling.pin();
							}
						}
					}
					else {
						this->init = true;
						auto& children = root->childNodes.pin(context->GetIsolate())->vec();
						if (!children.empty()) {
							this->stack.push_back(children[0].pin(context->GetIsolate()));
						}
					}
					return *this;
				}
			};
		}
	}
}