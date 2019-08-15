#pragma once

#include "node.h"

namespace dom {

	namespace utils {
		class RangeContextObject;
		class NodeIteratorContextObject;
		class TreeWalkerContextObject;
		class NodeFilterContextObject;
	}

	namespace nodes {
		class TextContextObject;
		class CDATASectionContextObject;
		class CommentContextObject;
		class ProcessingInstructionContextObject;
		class AttrContextObject;
		class XMLDocumentContextObject;

		class DocumentTypeContextObject : public NodeContextObject {
		public:
			CO_READONLY_ATTRIBUTE(name, v8::Local<v8::String>);
			CO_READONLY_ATTRIBUTE(publicId, v8::Local<v8::String>);
			CO_READONLY_ATTRIBUTE(systemId, v8::Local<v8::String>);
		};

		class DOMImplementationContextObject : public js_objects::BaseContextObject {
		public:
			CO_METHOD(createDocumentType, DocumentTypeContextObject&, v8::Local<v8::String> qualifiedName, v8::Local<v8::String> publicId, v8::Local<v8::String> systemId);
			CO_METHOD(createDocument, XMLDocumentContextObject&, v8::Local<v8::String> ns, v8::Local<v8::String> qualifiedName, DocumentTypeContextObject* doctype);
			CO_METHOD(createHTMLDocument, DocumentContextObject&, v8::Local<v8::String> title);

			CO_READONLY_ATTRIBUTE(hasFeature, bool) {
				return true;
			}
		};

		class DocumentFragmentContextObject : public NodeContextObject {
		};

		class DocumentContextObject : public NodeContextObject {
		public:
			CO_READONLY_ATTRIBUTE(implementation, DOMImplementationContextObject&);

			CO_READONLY_ATTRIBUTE(URL, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(documentURI, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(origin, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(compatMode, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(characterSet, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(charset, v8::Local<v8::String>) {
				return this->characterSetGET(isolate);
			}

			CO_READONLY_ATTRIBUTE(inputEncoding, v8::Local<v8::String>) {
				return this->characterSetGET(isolate);
			}

			CO_READONLY_ATTRIBUTE(contentType, v8::Local<v8::String>);

			CO_READONLY_ATTRIBUTE(doctype, DocumentTypeContextObject*);

			CO_READONLY_ATTRIBUTE(documentElement, ElementContextObject*);

			CO_METHOD(getElementsByTagName, collections::ComputedHTMLCollectionContextObject&, v8::Local<v8::String> qualifiedName);
			CO_METHOD(getElementsByTagNameNS, collections::ComputedHTMLCollectionContextObject&, v8::Local<v8::String> ns, v8::Local<v8::String> localName);
			CO_METHOD(getElementsByClassName, collections::ComputedHTMLCollectionContextObject&, v8::Local<v8::String> classNames);

			CO_METHOD(createElement, ElementContextObject&, v8::Local<v8::String> localName, v8::Local<v8::String> is);
			CO_METHOD(createElementNS, ElementContextObject&, v8::Local<v8::String> ns, v8::Local<v8::String> qualifiedName, v8::Local<v8::String> is);
			CO_METHOD(createDocumentFragment, DocumentFragmentContextObject&);
			CO_METHOD(createTextNode, TextContextObject&, v8::Local<v8::String> data);
			CO_METHOD(createCDATASection, CDATASectionContextObject&, v8::Local<v8::String> data);
			CO_METHOD(createComment, CommentContextObject&, v8::Local<v8::String> data);
			CO_METHOD(createProcessingInstruction, ProcessingInstructionContextObject, v8::Local<v8::String> target, v8::Local<v8::String> data);

			CO_METHOD(importNode, NodeContextObject&, NodeContextObject& node, bool deep);
			CO_METHOD(adoptNode, NodeContextObject&, NodeContextObject& node);

			CO_METHOD(createAttribute, AttrContextObject&, v8::Local<v8::String> localName);
			CO_METHOD(createAttributeNS, AttrContextObject&, v8::Local<v8::String> ns, v8::Local<v8::String> qualifiedName);

			CO_METHOD(createEvent, events::EventContextObject&, v8::Local<v8::String> iface);

			CO_METHOD(createRange, utils::RangeContextObject&);

			CO_METHOD(createNodeIterator, utils::NodeIteratorContextObject&, NodeContextObject& root, std::size_t whatToShow, utils::NodeFilterContextObject* filter);
			CO_METHOD(createTreeWalker, utils::TreeWalkerContextObject&, NodeContextObject& root, std::size_t whatToShow, utils::NodeFilterContextObject* filter);

			virtual events::EventTargetContextObject* getTheParent(events::EventContextObject& evt);
		private:
		};

		class XMLDocumentContextObject : public DocumentContextObject {};
	}
}