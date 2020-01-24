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
			public:
				MIXIN_NON_ELEMENT_PARENT_NODE;
				MIXIN_PARENT_NODE;
			protected:
				pins::NullProperty<ElementContextObject> host;
				friend class DocumentContextObject;
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
				CO_READONLY_ATTRIBUTE(documentURI, v8::Local<v8::String>) {
					return this->url.pin(context->GetIsolate())->serialize(context);
				}
				CO_ALIAS_GETTER(URL, documentURI);
				CO_READONLY_ATTRIBUTE(compatMode, v8::Local<v8::String>) {
					return v8::String::NewFromUtf8(context->GetIsolate(), this->mode == Mode::QUIRKS ? "BackCompat" : "CSS1Compat").ToLocalChecked();
				}
				CO_READONLY_ATTRIBUTE(characterSet, v8::Local<v8::String>) {
					return this->encoding.Get(context->GetIsolate());
				}
				CO_ALIAS_GETTER(charset, characterSet);
				CO_ALIAS_GETTER(inputEncoding, characterSet);
				CO_TRANSPARENT_GETTER(contentType);

				//TODO
				CO_READONLY_ATTRIBUTE(doctype, pins::NullPin<DocumentTypeContextObject>);
				//TODO
				CO_READONLY_ATTRIBUTE(documentElement, pins::NullPin<ElementContextObject>);

				//TODO
				CO_METHOD(getElementsByTagName, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::String> qualifiedName);
				//TODO
				CO_METHOD(getElementsByTagNameNS, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> localName);
				//TODO
				CO_METHOD(getElementsByClassName, pins::Pin<HTMLCollectionContextObject>, v8::Local<v8::String> className);

				//TODO
				CO_METHOD(createElement, pins::Pin<ElementContextObject>, v8::Local<v8::String> localName, v8::Local<v8::String> options);
				//TODO
				CO_METHOD(createElementNS, pins::Pin<ElementContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> qualifiedName, v8::Local<v8::String> options);
				//TODO
				CO_METHOD(createDocumentFragment, pins::Pin<DocumentFragmentContextObject>);
				//TODO
				CO_METHOD(createTextNode, pins::Pin<TextContextObject>, v8::Local<v8::String> data);
				//TODO
				CO_METHOD(createCDATASection, pins::Pin<CDATASectionContextObject>, v8::Local<v8::String> data);
				//TODO
				CO_METHOD(createComment, pins::Pin<CommentContextObject>, v8::Local<v8::String> data);
				//TODO
				CO_METHOD(createProcessingInstruction, pins::Pin<ProcessingInstructionContextObject>, v8::Local<v8::String> target, v8::Local<v8::String> data);

				CO_METHOD(importNode, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<DocumentContextObject> self, pins::Pin<NodeContextObject> node, bool deep) {
					if (node->type() == 9 || node->domTypeof(DOMType::ShadowRoot)) {
						//TODO throw error
						return {};
					}
					return algorithms::clone(node, self, deep);
				}
				CO_METHOD(adoptNode, std::optional<pins::Pin<NodeContextObject>>, pins::Pin<DocumentContextObject> self, pins::Pin<NodeContextObject> node) {
					//TODO exceptions
					if (node->type() == 9) {
						//TODO throw error
						return {};
					}
					if (node->domTypeof(DOMType::ShadowRoot)) {
						//TODO throw error
						return {};
					}
					if (node->type() == 11 && node.downcast<DocumentFragmentContextObject>()->host.pin(context->GetIsolate())) {
						return {};
					}
					algorithms::adopt(context, node, self);
					return { node };
				}

				//TODO
				CO_METHOD(createAttribute, pins::Pin<AttrContextObject>, v8::Local<v8::String> localName);
				//TODO
				CO_METHOD(createAttributeNS, pins::Pin<AttrContextObject>, v8::Local<v8::Value> ns, v8::Local<v8::String> qualifiedName);

				//TODO
				CO_METHOD(createEvent, pins::Pin<EventContextObject>, v8::Local<v8::String> iface);
				//TODO
				CO_METHOD(createRange, pins::Pin<RangeContextObject>);

				//TODO
				CO_METHOD(createNodeInterator, pins::Pin<NodeIteratorContextObject>, pins::Pin<NodeContextObject> root, std::size_t whatToShow, v8::Local<v8::Value> nodeFilter);
				//TODO
				CO_METHOD(createTreeWalker, pins::Pin<TreeWalkerContextObject>, pins::Pin<NodeContextObject> root, std::size_t whatToShow, v8::Local<v8::Value> nodeFilter);

				MIXIN_XPATH_EVALUATOR_BASE;
				MIXIN_NON_ELEMENT_PARENT_NODE;
				MIXIN_DOCUMENT_OR_SHADOW_ROOT;
				MIXIN_PARENT_NODE;

				virtual pins::NullPin<EventTargetContextObject> getTheParent(v8::Local<v8::Context> context, pins::Pin<EventContextObject> event) {

				}

				enum class Mode {
					NO_QUIRKS, LIMITED_QUIRKS, QUIRKS
				};

			private:
				v8::UniquePersistent<v8::String> encoding;
				v8::UniquePersistent<v8::String> contentType;
				pins::Property<url::URLContextObject> url;
				//TODO origin
				v8::UniquePersistent<v8::String> type;
				Mode mode;
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