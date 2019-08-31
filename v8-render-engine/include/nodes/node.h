#pragma once

#include "observer.h"
#include "events/event_target.h"
#include "nodes/collections.h"

namespace dom {
	namespace nodes {

		class DocumentContextObject;
		class ElementContextObject;

		enum NodeType {
			OFFSET, ELEMENT_NODE, ATTRIBUTE_NODE, TEXT_NODE, CDATA_SECTION_NODE, ENTITY_REFERENCE_NODE, ENTITY_NODE, PROCESSING_INSTRUCTION_NODE, COMMENT_NODE, DOCUMENT_NODE, DOCUMENT_TYPE_NODE, NOTATION_NODE
		};

		class NodeContextObject : public events::EventTargetContextObject {
		public:
			CO_READONLY_ATTRIBUTE(nodeType, unsigned char) {
				return this->nodeType;
			}

			CO_TRANSPARENT_GETTER(nodeName, v8::Local<v8::String>)

				CO_READONLY_ATTRIBUTE(baseURI, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(isConnected, bool) {
				return !!this->document;
			}

			CO_READONLY_ATTRIBUTE(ownerDocument, js_objects::NullablePin<DocumentContextObject>) {
				return reinterpret_cast<NodeContextObject*>(this->document.cppRef) == this ? js_objects::NullablePin<DocumentContextObject>() : this->document.pinNullable(context->GetIsolate());
			}

			CO_METHOD(getRootNode, js_objects::Pin<NodeContextObject>, bool composed);

			CO_READONLY_ATTRIBUTE(parentNode, auto) {
				return this->parent.pinNullable(context->GetIsolate());
			}

			CO_READONLY_ATTRIBUTE(parentElement, js_objects::NullablePin<ElementContextObject>) {
				if (this->parent) {
					auto pinned = this->parent.pinNullable(context->GetIsolate()).pin();
					if (pinned->nodeType == NodeType::ELEMENT_NODE) return pinned.reinterpret<ElementContextObject>();
				}
				return js_objects::NullablePin<ElementContextObject>();
			}

			CO_READONLY_ATTRIBUTE(hasChildNodes, bool) {
				return this->childNodes->size();
			}

			CO_READONLY_ATTRIBUTE(childNodes, js_objects::Pin<collections::NodeListContextObject>) {
				return this->childNodes.pin(context->GetIsolate());
			}

			CO_READONLY_ATTRIBUTE(firstChild, js_objects::NullablePin<NodeContextObject>) {
				return this->childNodes->size() ? this->childNodes->get(context, 0) : js_objects::NullablePin<NodeContextObject>();
			}

			CO_READONLY_ATTRIBUTE(lastChild, js_objects::NullablePin<NodeContextObject>) {
				return this->childNodes->size() ? this->childNodes->back(context) : js_objects::NullablePin<NodeContextObject>();
			}

			CO_READONLY_ATTRIBUTE(previousSibling, js_objects::NullablePin<NodeContextObject>) {
				return ~this->idx || !this->idx ? js_objects::NullablePin<NodeContextObject>() : this->parent.cppRef->childNodes->get(context, this->idx - 1);
			}

			CO_READONLY_ATTRIBUTE(nextSibling, js_objects::NullablePin<NodeContextObject>) {
				if (~this->idx) return js_objects::NullablePin<NodeContextObject>();
				auto& family = this->parent.cppRef->childNodes;
				return this->idx + 1 < family->size() ? family->get(context, this->idx) : js_objects::NullablePin<NodeContextObject>();
			}

			//CEReactions
			CO_ATTRIBUTE(nodeValue, v8::Local<v8::String>);
			CO_ATTRIBUTE(textContent, v8::Local<v8::String>);
			CO_METHOD(normalize, void);

			CO_METHOD(cloneNode, js_objects::Pin<NodeContextObject>, bool deep);

			CO_METHOD(compareDocumentPosition, unsigned char, js_objects::Pin<NodeContextObject> other);

			CO_METHOD(contains, bool, js_objects::NullablePin<NodeContextObject> other);

			CO_METHOD(lookupPrefix, v8::Local<v8::Value>, v8::Local<v8::Value> ns);
			CO_METHOD(lookupNamespaceURI, v8::Local<v8::Value>, v8::Local<v8::Value> prefix);
			CO_METHOD(isDefaultNamespace, bool, v8::Local<v8::Value> ns);

			//CEReactions
			CO_METHOD(insertBefore, js_objects::Pin<NodeContextObject>, js_objects::Pin<NodeContextObject> node, js_objects::NullablePin<NodeContextObject> child);
			CO_METHOD(appendChild, js_objects::Pin<NodeContextObject>, js_objects::Pin<NodeContextObject> node);
			CO_METHOD(replaceChild, js_objects::Pin<NodeContextObject>, js_objects::Pin<NodeContextObject> node, js_objects::Pin<NodeContextObject> child);
			CO_METHOD(removeChild, js_objects::Pin<NodeContextObject>, js_objects::Pin<NodeContextObject> child);

			virtual js_objects::NullablePin<events::EventTargetContextObject> getTheParent(v8::Local<v8::Context> context,
				js_objects::Pin<events::EventContextObject> evt) override {
				//TODO: Implement for slots
				return this->parentNodeGET(context).cast<events::EventTargetContextObject>();
			}

			bool privilegedContains(v8::Local<v8::Context> context, js_objects::Pin<NodeContextObject> node) {
				//TODO: actually check contains
				return false;
			}

			js_objects::Pin<NodeContextObject> getRoot(v8::Local<v8::Context> context) {
				//TODO: actually return the root
				return this->pin<NodeContextObject>(context->GetIsolate());
			}

			auto& getObservers() { return this->observers; }

			void registerObserver(js_objects::JS_CPP_Property<observer::MutationObserverContextObject> observer, data_structs::EnumSet<observer::MutationObserverOptions> options) {
				//TODO: Track optimal observations
				observers.emplace_back(std::move(observer), options);
			}

		private:
			unsigned char nodeType;
			v8::UniquePersistent<v8::String> nodeName;
			js_objects::Nullable_JS_CPP_Property<NodeContextObject> parent;
			js_objects::Nullable_JS_CPP_Property<DocumentContextObject> document;
			js_objects::JS_CPP_Property<collections::NodeListContextObject> childNodes;
			js_objects::JS_CPP_Property<collections::ElementHTMLCollectionContextObject> children;
			std::size_t idx;

			std::deque<std::pair<js_objects::JS_CPP_Property<observer::MutationObserverContextObject>,
				data_structs::EnumSet<observer::MutationObserverOptions>>> observers;

			void textContentInternal(v8::Local<v8::Context> context, std::string& contentBuffer);
		};
	}
}