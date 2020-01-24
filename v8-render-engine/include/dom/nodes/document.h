#pragma once
#include "dom/xpath/x_path_evaluator_base.h"
#include "url/url.h"
#include "dom/mixins/mixins.h"

namespace cpp {
	namespace dom {
		namespace nodes {

			class DOMImplementationContextObject;
			class DocumentTypeContextObject : public NodeContextObject {
			public:
				CO_TRANSPARENT_GETTER(name);
				CO_TRANSPARENT_GETTER(publicId);
				CO_TRANSPARENT_GETTER(systemId);

				MIXIN_CHILD_NODE;
			private:
				v8::UniquePersistent<v8::String> name;
				v8::UniquePersistent<v8::String> publicId;
				v8::UniquePersistent<v8::String> systemId;
			};
			class DocumentFragmentContextObject : public NodeContextObject {
				MIXIN_NON_ELEMENT_PARENT_NODE;
				MIXIN_PARENT_NODE;
			};
			class TextContextObject;
			class CDATASectionContextObject;
			class CommentContextObject;
			class ProcessingInstructionContextObject;
			class AttrContextObject;
			class RangeContextObject;
			class NodeIteratorContextObject;
			class TreeWalkerContextObject;

			class DocumentContextObject : public NodeContextObject {
			public:
				CO_PIN_GETTER(implementation);
				CO_READONLY_ATTRIBUTE(url, v8::Local<v8::String>);
				CO_READONLY_ATTRIBUTE(documentURI, v8::Local<v8::String>);
				CO_READONLY_ATTRIBUTE(compatMode, v8::Local<v8::String>);
				CO_READONLY_ATTRIBUTE(characterSet, v8::Local<v8::String>) {
					return this->encoding.Get(context->GetIsolate());
				}
				CO_ALIAS_GETTER(charset, characterSet);
				CO_ALIAS_GETTER(inputEncoding, characterSet);
				CO_TRANSPARENT_GETTER(contentType);

				CO_READONLY_ATTRIBUTE(doctype, pins::NullPin<DocumentTypeContextObject>);
				CO_READONLY_ATTRIBUTE(documentElement, pins::NullPin<ElementContextObject>);

				CO_METHOD(getElementsByTagName, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::String> qualifiedName);
				CO_METHOD(getElementsByTagNameNS, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
				CO_METHOD(getElementsByClassName, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::String> className);

				CO_METHOD(createElement, pins::Pin<ElementContextObject>, v8::Local<v8::String> localName, v8::Local<v8::String> options);
				CO_METHOD(createElementNS, pins::Pin<ElementContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> qualifiedName, v8::Local<v8::String> options);
				CO_METHOD(createDocumentFragment, pins::Pin<DocumentFragmentContextObject>);
				CO_METHOD(createTextNode, pins::Pin<TextContextObject>, v8::Local<v8::String> data);
				CO_METHOD(createCDATASection, pins::Pin<CDATASectionContextObject>, v8::Local<v8::String> data);
				CO_METHOD(createComment, pins::Pin<CommentContextObject>, v8::Local<v8::String> data);
				CO_METHOD(createProcessingInstruction, pins::Pin<ProcessingInstructionContextObject>, v8::Local<v8::String> target, v8::Local<v8::String> data);

				CO_METHOD(importNode, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<NodeContextObject> node, bool deep) {
					//TODO
				}
				CO_METHOD(adoptNode, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<DocumentContextObject> self, pins::Pin<NodeContextObject> node) {
					//TODO exceptions
					mutation_algorithms::adopt(context, node, self);
					return { node };
				}

				CO_METHOD(createAttribute, pins::Pin<AttrContextObject>, v8::Local<v8::String> localName);
				CO_METHOD(createAttributeNS, pins::Pin<AttrContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> qualifiedName);

				CO_METHOD(createEvent, pins::Pin<EventContextObject>, v8::Local<v8::String> iface);
				CO_METHOD(createRange, pins::Pin<RangeContextObject>);

				CO_METHOD(createNodeInterator, pins::Pin<NodeIteratorContextObject>, pins::Pin<NodeContextObject> root, std::size_t whatToShow, v8::Local<v8::Value> nodeFilter);
				CO_METHOD(createTreeWalker, pins::Pin<TreeWalkerContextObject>, pins::Pin<NodeContextObject> root, std::size_t whatToShow, v8::Local<v8::Value> nodeFilter);

				MIXIN_XPATH_EVALUATOR_BASE;
				MIXIN_NON_ELEMENT_PARENT_NODE;
				MIXIN_DOCUMENT_OR_SHADOW_ROOT;
				MIXIN_PARENT_NODE;

				virtual pins::NullPin<EventTargetContextObject> getTheParent(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event);
			private:
				v8::UniquePersistent<v8::String> encoding;
				v8::UniquePersistent<v8::String> contentType;
				pins::Property<url::URLContextObject> url;
				//TODO origin
				v8::UniquePersistent<v8::String> type;
				v8::UniquePersistent<v8::String> mode;
				pins::Property<DOMImplementationContextObject> implementation;
			};

			class XMLDocumentContextObject : public DocumentContextObject {};

			class DOMImplementationContextObject : public DOMContextObject {
			public:
				CO_METHOD(createDocumentType, pins::Pin<DocumentTypeContextObject>, v8::Local<v8::String> qualifiedName, v8::Local<v8::String> publicId, v8::Local<v8::String> systemId);
				CO_METHOD(createDocument, pins::Pin<XMLDocumentContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> qualifiedName, pins::NullPin<DocumentTypeContextObject> doctype);
				CO_METHOD(createHTMLDocument, pins::Pin<DocumentContextObject>, v8::Local<v8::Value> title);

				CO_METHOD(hasFeature, v8::Local<v8::Boolean>) { return v8::Boolean::New(context->GetIsolate(), true); }
			};
		}
	}
}