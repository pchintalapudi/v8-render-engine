#pragma once

#include <map>
#include <unordered_set>
#include <optional>

#include "dom/events/event_target.h"
#include "dom/collections/html_collection.h"
#include "dom/mutations/mutation_observer.h"
#include "dom/algorithms/node_algorithms.h"

namespace cpp {
	namespace dom {
		namespace nodes {

			class DocumentContextObject;
			class ElementContextObject;


			class NodeContextObject : public EventTargetContextObject {
			public:

				CO_STATIC_UINT32_ATTRIBUTE(ELEMENT_NODE, 1);
				CO_STATIC_UINT32_ATTRIBUTE(ATTRIBUTE_NODE, 2);
				CO_STATIC_UINT32_ATTRIBUTE(TEXT_NODE, 3);
				CO_STATIC_UINT32_ATTRIBUTE(CDATA_SECTION_NODE, 4);
				CO_STATIC_UINT32_ATTRIBUTE(ENTITY_REFERENCE_NODE, 5);
				CO_STATIC_UINT32_ATTRIBUTE(ENTITY_NODE, 6);
				CO_STATIC_UINT32_ATTRIBUTE(PROCESSING_INSTRUCTION_NODE, 7);
				CO_STATIC_UINT32_ATTRIBUTE(COMMENT_NODE, 8);
				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_NODE, 9);
				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_TYPE_NODE, 10);
				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_FRAGMENT_NODE, 11);
				CO_STATIC_UINT32_ATTRIBUTE(NOTATION_NODE, 12);

				CO_PRIMITIVE_GETTER(nodeType, v8::Integer);

				CO_READONLY_ATTRIBUTE(nodeName, v8::Local<v8::String>);

				//TODO
				CO_READONLY_ATTRIBUTE(baseURI, v8::Local<v8::String>);

				CO_READONLY_ATTRIBUTE(isConnected, v8::Local<v8::Boolean>);

				CO_READONLY_ATTRIBUTE(ownerDocument, auto) {
					return reinterpret_cast<NodeContextObject*>(nodeDocument.pin(context->GetIsolate()).native) == this ? pins::NullPin<DocumentContextObject>::null(context->GetIsolate()) : nodeDocument.pin(context->GetIsolate());
				}

				void setDocument(v8::Local<v8::Context> context, pins::Pin<DocumentContextObject> document) {
					this->nodeDocument.set(context->GetIsolate(), document);
				}

				CO_METHOD(getRootNode, pins::Pin<NodeContextObject>, pins::Pin<NodeContextObject> self, bool composed);

				CO_PIN_GETTER(parent);

				CO_READONLY_ATTRIBUTE(parentElement, auto) {
					auto local = this->parent.pin(context->GetIsolate());
					return (local && local.pin()->nodeType == 1) ? local.cast<ElementContextObject>() : pins::NullPin<ElementContextObject>::null(context->GetIsolate());
				}

				CO_READONLY_ATTRIBUTE(hasChildNodes, auto) {
					return v8::Boolean::New(context->GetIsolate(), !!this->childNodes.pin(context->GetIsolate())->size());
				}
				CO_PIN_GETTER(childNodes);
				CO_READONLY_ATTRIBUTE(firstChild, auto) {
					auto pin = this->childNodes_GET(context);
					return pin->size() ? pin->operator[](0).pin(context->GetIsolate()) : pins::NullPin<NodeContextObject>::null(context->GetIsolate());
				}
				CO_READONLY_ATTRIBUTE(lastChild, auto) {
					auto pin = this->childNodes_GET(context);
					return pin->size() ? pin->operator[](pin->size() - 1).pin(context->GetIsolate()) : pins::NullPin<NodeContextObject>::null(context->GetIsolate());
				}

				CO_PIN_GETTER(previousSibling);
				CO_PIN_GETTER(nextSibling);

				void setPrevSibling(v8::Local<v8::Context> context, pins::NullPin<NodeContextObject> node) {
					this->previousSibling.set(context->GetIsolate(), node);
				}
				void setNextSibling(v8::Local<v8::Context> context, pins::NullPin<NodeContextObject> node) {
					this->nextSibling.set(context->GetIsolate(), node);
				}
				void dropSiblings(v8::Local<v8::Context> context) {
					this->previousSibling.reset(context->GetIsolate());
					this->nextSibling.reset(context->GetIsolate());
				}

				CO_ATTRIBUTE(nodeValue, v8::Local<v8::Value>);
				CO_ATTRIBUTE(textContent, v8::Local<v8::Value>);
				//TODO
				CO_METHOD(normalize, void);

				CO_METHOD(cloneNode, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<NodeContextObject> self, bool deep) {
					if (this->domTypeof(DOMType::ShadowRoot)) {
						//TODO throw exception
						return {};
					}
					return { algorithms::clone(self, pins::NullPin<DocumentContextObject>::null(context->GetIsolate()), deep) };
				}
				CO_METHOD(isEqualNode, bool, pins::NullPin<NodeContextObject> otherNode);
				CO_METHOD(isSameNode, bool, pins::NullPin<NodeContextObject> otherNode) {
					return otherNode.native == this;
				}

				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_POSITION_DISCONNECTED, 1 << 0);
				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_POSITION_PRECEDING, 1 << 1);
				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_POSITION_FOLLOWING, 1 << 2);
				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_POSITION_CONTAINS, 1 << 3);
				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_POSITION_CONTAINED_BY, 1 << 4);
				CO_STATIC_UINT32_ATTRIBUTE(DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC, 1 << 5);

				CO_METHOD(compareDocumentPosition, v8::Local<v8::Integer>, pins::Pin<NodeContextObject> self, pins::Pin<NodeContextObject> other);
				CO_METHOD(contains, v8::Local<v8::Boolean>, pins::NullPin<NodeContextObject> other);

				CO_METHOD(lookupPrefix, v8::Local<v8::Value>, v8::Local<v8::String> ns);
				CO_METHOD(lookupNamespaceURI, v8::Local<v8::Value>, v8::Local<v8::String> prefix);
				CO_METHOD(isDefaultNamespace, v8::Local<v8::Boolean>, v8::Local<v8::Value> ns);

				CO_METHOD(insertBefore, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<NodeContextObject> self, pins::Pin<NodeContextObject> node, pins::NullPin<NodeContextObject> child) {
					return algorithms::preInsert(context, node, self, child);
				}
				CO_METHOD(appendChild, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<NodeContextObject> self, pins::Pin<NodeContextObject> node) {
					return algorithms::preInsert(context, node, self, pins::NullPin<NodeContextObject>::null(context->GetIsolate()));
				}
				CO_METHOD(replaceChild, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<NodeContextObject> self, pins::Pin<NodeContextObject> node, pins::Pin<NodeContextObject> child) {
					return algorithms::replace(context, node, self, child);
				}
				CO_METHOD(removeChild, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<NodeContextObject> self, pins::Pin<NodeContextObject> node) {
					return algorithms::preRemove(context, node, self);
				}

				//TODO
				virtual pins::NullPin<EventTargetContextObject> getTheParent(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event);

				auto type() {
					return this->nodeType;
				}

				auto& getObservers() {
					return this->registeredObserverList;
				}

				//TODO
				void queueMutationRecord(v8::Local<v8::String> type, pins::Pin<NodeContextObject> target, v8::Local<v8::String> name, v8::Local<v8::String> ns, v8::Local<v8::String> oldValue, pins::Pin<NodeListContextObject> addedNodes, pins::Pin<NodeListContextObject> removedNodes, pins::NullPin<NodeContextObject> previousSibling, pins::NullPin<NodeContextObject> nextSibling);

				pins::NullPin<nodes::NodeContextObject> commonAncestor(v8::Local<v8::Context> context, pins::Pin<nodes::NodeContextObject> self, pins::Pin<NodeContextObject> other) {
					std::map<nodes::NodeContextObject*, pins::Pin<NodeContextObject>> parents;
					pins::NullPin<NodeContextObject> parent = other;
					while (parent) {
						parents.emplace(parent.native, parent.pin());
						parent = parent.native->parent_GET(context);
					}
					parent = self;
					while (parent) {
						auto found = parents.find(parent.native);
						if (found != parents.end()) {
							return found->second;
						}
						parent = parent.native->parent_GET(context);
					}
					return pins::NullPin<nodes::NodeContextObject>::null(context->GetIsolate());
				}

			private:
				pins::Property<DocumentContextObject> nodeDocument;
				int32_t nodeType;
				std::vector<RegisteredObserver> registeredObserverList;
				pins::NullProperty<NodeContextObject> parent;
				std::unordered_set<NodeContextObject*> childNodeSet;
				pins::Property<NodeListContextObject> childNodes;
				pins::NullProperty<NodeContextObject> previousSibling;
				pins::NullProperty<NodeContextObject> nextSibling;

				friend class StackDescendantIterator;
			};
		}
	}
}