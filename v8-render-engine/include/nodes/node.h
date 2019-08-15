#pragma once

#include "events/event_target.h"
#include "nodes/collections.h"

namespace dom {
	namespace nodes {

		class DocumentContextObject;
		class ElementContextObject;

		class NodeContextObject : public events::EventTargetContextObject {
		public:
			CO_READONLY_ATTRIBUTE(nodeType, unsigned char);

			CO_READONLY_ATTRIBUTE(nodeName, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(baseURI, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(isConnected, bool);

			CO_READONLY_ATTRIBUTE(ownerDocument, DocumentContextObject*);

			CO_READONLY_ATTRIBUTE(parentNode, NodeContextObject*);

			CO_READONLY_ATTRIBUTE(parentElement, ElementContextObject*);

			CO_READONLY_ATTRIBUTE(hasChildNodes, bool);

			CO_READONLY_ATTRIBUTE(childNodes, collections::NodeListContextObject&);

			CO_READONLY_ATTRIBUTE(firstChild, NodeContextObject*);
			CO_READONLY_ATTRIBUTE(lastChild, NodeContextObject*);
			CO_READONLY_ATTRIBUTE(previousSibling, NodeContextObject*);
			CO_READONLY_ATTRIBUTE(nextSibling, NodeContextObject*);

			//CEReactions
			CO_ATTRIBUTE(nodeValue, v8::Local<v8::String>);
			CO_ATTRIBUTE(textContent, v8::Local<v8::String>);
			CO_METHOD(normalize, void);

			CO_METHOD(cloneNode, NodeContextObject*, bool deep);

			CO_METHOD(compareDocumentPosition, unsigned char, NodeContextObject& other);

			CO_METHOD(contains, bool, NodeContextObject* other);

			CO_METHOD(lookupPrefix, v8::Local<v8::Value>, v8::Local<v8::Value> ns);
			CO_METHOD(lookupNamespaceURI, v8::Local<v8::Value>, v8::Local<v8::Value> prefix);
			CO_METHOD(isDefaultNamespace, bool, v8::Local<v8::Value> ns);

			//CEReactions
			CO_METHOD(insertBefore, NodeContextObject&, NodeContextObject& node, NodeContextObject* child);
			CO_METHOD(appendChild, NodeContextObject&, NodeContextObject& node);
			CO_METHOD(replaceChild, NodeContextObject&, NodeContextObject& node, NodeContextObject& child);
			CO_METHOD(removeChild, NodeContextObject&, NodeContextObject& child);

			virtual events::EventTargetContextObject* getTheParent(events::EventContextObject& evt);

		private:
		};
	}
}