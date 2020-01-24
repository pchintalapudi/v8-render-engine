#include "dom/nodes/node.h"
#include "dom/nodes/element.h"
#include "dom/nodes/attr.h"
#include "dom/nodes/text.h"

using namespace cpp::dom::nodes;

v8::Local<v8::String> NodeContextObject::nodeName_GET(v8::Local<v8::Context> context) {
	switch (this->nodeType) {
	case 1:
		return static_cast<ElementContextObject*>(this)->htmlUppercasedQualifiedName.Get(context->GetIsolate());
	case 2:
		return static_cast<AttrContextObject*>(this)->name_GET(context);
	case 3:
		return v8::String::NewFromUtf8(context->GetIsolate(), "#text").ToLocalChecked();
	case 4:
		return v8::String::NewFromUtf8(context->GetIsolate(), "#cdata-section").ToLocalChecked();
	case 7:
		return static_cast<ProcessingInstructionContextObject*>(this)->target_GET(context);
	case 8:
		return v8::String::NewFromUtf8(context->GetIsolate(), "#comment").ToLocalChecked();
	case 9:
		return v8::String::NewFromUtf8(context->GetIsolate(), "#document").ToLocalChecked();
	case 10:
		return static_cast<DocumentTypeContextObject*>(this)->name_GET(context);
	case 11:
		return v8::String::NewFromUtf8(context->GetIsolate(), "#document-fragment").ToLocalChecked();
	default:
		return v8::String::Empty(context->GetIsolate());
	}
}

CO_METHOD(NodeContextObject::getRootNode, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject>, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> self, bool composed) {
	auto parent = this->parent.pin(context->GetIsolate());
	auto valid = self;
	while (parent) {
		valid = parent.pin();
		parent = valid->parent.pin(context->GetIsolate());
	}
	if (composed && valid->domTypeof(DOMType::ShadowRoot)) {
		auto host = valid.downcast<ShadowRootContextObject>()->host_GET(context);
		return host->getRootNode_METHOD(context, valid, true);
	}
	else {
		return valid;
	}
}

CO_READONLY_ATTRIBUTE(NodeContextObject::isConnected, v8::Local<v8::Boolean>) {
	auto parent = this->parent.pin(context->GetIsolate());
	if (!parent) {
		if (this->nodeType == 9) {
			return v8::Boolean::New(context->GetIsolate(), true);
		}
		else if (this->domTypeof(DOMType::ShadowRoot)) {
			parent = static_cast<ShadowRootContextObject*>(this)->host_GET(context).cast<NodeContextObject>();
		}
		else {
			return v8::Boolean::New(context->GetIsolate(), false);
		}
	}
	while (true) {
		auto pinned = parent.pin();
		auto candidate = pinned->parent.pin(context->GetIsolate());
		if (!candidate) {
			if (pinned->nodeType == 9) {
				return v8::Boolean::New(context->GetIsolate(), true);
			}
			else if (pinned->domTypeof(DOMType::ShadowRoot)) {
				parent = pinned.downcast<ShadowRootContextObject>()->host_GET(context).cast<NodeContextObject>();
			}
			else {
				return v8::Boolean::New(context->GetIsolate(), false);
			}
		}
		else {
			parent = candidate;
		}
	}
}

CO_INLINE_ATTRIBUTE_GETTER(NodeContextObject::nodeValue, v8::Local<v8::Value>) {
	switch (this->nodeType) {
	case 2:
		return static_cast<AttrContextObject*>(this)->value_GET(context);
	case 3:
	case 7:
	case 8:
		return static_cast<CharacterDataContextObject*>(this)->data_GET(context);
	default:
		return v8::Null(context->GetIsolate());
	}
}
CO_INLINE_ATTRIBUTE_SETTER(NodeContextObject::nodeValue, v8::Local<v8::Value>) {
	switch (this->nodeType) {
	case 2:
		//TODO set attribute value
	case 3:
	case 7:
	case 8:
		//TODO replace data
		break;
	}
}

CO_INLINE_ATTRIBUTE_GETTER(NodeContextObject::textContent, v8::Local<v8::Value>) {
	switch (this->nodeType) {
	case 1:
	case 10:
		//TODO textcontent
		return v8::String::Empty(context->GetIsolate());
	case 2:
		return static_cast<AttrContextObject*>(this)->value_GET(context);
	case 3:
	case 7:
	case 8:
		return static_cast<CharacterDataContextObject*>(this)->data_GET(context);
	default:
		return v8::Null(context->GetIsolate());
	}
}

CO_INLINE_ATTRIBUTE_SETTER(NodeContextObject::textContent, v8::Local<v8::Value>) {
	switch (this->nodeType) {
	case 1:
	case 10:
		//TODO textcontent
		break;
	case 2:
		//TODO attribute setting
		break;
	case 3:
	case 7:
	case 8:
		//TODO replace data
		break;
	}
}

bool precedes(v8::Local<v8::Context> context, cpp::pins::Pin<NodeContextObject> n1, cpp::pins::Pin<NodeContextObject> n2) {
	std::map<cpp::dom::nodes::NodeContextObject*, std::size_t> parents;
	std::vector<cpp::pins::Pin<NodeContextObject>> parentPins;
	cpp::pins::NullPin<NodeContextObject> parent = n1;
	while (parent) {
		parents.emplace(parent.native, parentPins.size());
		parentPins.push_back(parent.pin());
		parent = parent.native->parent_GET(context);
	}
	parent = n2;
	auto oldParent = n2;
	while (parent) {
		auto found = parents.find(parent.native);
		if (found != parents.end()) {
			auto common = parentPins[found->second - 1];
			auto& childNodes = common->childNodes_GET(context)->vec();
			for (auto& prop : childNodes) {
				auto pin = prop.pin(context->GetIsolate());
				if (pin == n1) {
					return true;
				}
				else if (pin == n2) {
					break;
				}
			}
			break;
		}
		oldParent = parent.pin();
		parent = parent.native->parent_GET(context);
	}
	return false;
}

CO_METHOD(NodeContextObject::compareDocumentPosition, v8::Local<v8::Integer>, cpp::pins::Pin<NodeContextObject> self, cpp::pins::Pin<NodeContextObject> other) {
	if (this == other.native) {
		return v8::Integer::New(context->GetIsolate(), 0b000000);
	}
	pins::NullPin<NodeContextObject> node1 = other;
	pins::NullPin<NodeContextObject> node2 = self;
	auto attr1 = pins::NullPin<AttrContextObject>::null(context->GetIsolate());
	auto attr2 = pins::NullPin<AttrContextObject>::null(context->GetIsolate());
	if (other->nodeType == 2) {
		auto attr = other.downcast<AttrContextObject>();
		attr1 = attr;
		node1 = attr->ownerElement_GET(context).cast<NodeContextObject>();
	}
	if (self->nodeType == 2) {
		auto attr = self.downcast<AttrContextObject>();
		attr2 = attr;
		node2 = attr->ownerElement_GET(context).cast<NodeContextObject>();
		if (attr1 && node1 && node1 == node2) {
			auto el = node2.pin().downcast<ElementContextObject>();
			for (auto& attrProp : el->attributes.pin(context->GetIsolate())->getAttributeList()) {
				auto a = attrProp.pin(context->GetIsolate());
				if (a == attr1) {
					return v8::Integer::New(context->GetIsolate(), 0b100010);
				}
				if (a == attr2) {
					return v8::Integer::New(context->GetIsolate(), 0b100100);
				}
			}
		}
		bool disconnected = !node1 || !node2;
		if (!disconnected) {
			auto n1 = node1.pin();
			auto n2 = node2.pin();
			disconnected = n1->getRootNode_METHOD(context, n1, false) != n2->getRootNode_METHOD(context, n2, false);
		}
		if (disconnected) {
			return v8::Integer::New(context->GetIsolate(), 0b100101);
		}
		auto n1 = node1.pin();
		auto n2 = node2.pin();
		auto common = n1->commonAncestor(context, n1, n2);
		if (common == n1 && !attr1 || node1 == node2 && attr2) {
			return v8::Integer::New(context->GetIsolate(), 0b001010);
		}
		if (common == n2 && !attr2 || node1 == node2 && attr1) {
			return v8::Integer::New(context->GetIsolate(), 0b010100);
		}
		return precedes(context, n1, n2) ? v8::Integer::New(context->GetIsolate(), 0b000010) : v8::Integer::New(context->GetIsolate(), 0b000100);
	}
}

CO_METHOD(NodeContextObject::contains, v8::Local<v8::Boolean>, cpp::pins::NullPin<NodeContextObject> other) {
	while (other) {
		if (other.native == this) {
			return v8::Boolean::New(context->GetIsolate(), true);
		}
		other = other.pin()->parent.pin(context->GetIsolate());
	}
	return v8::Boolean::New(context->GetIsolate(), false);
}


CO_METHOD(NodeContextObject::isEqualNode, bool, cpp::pins::NullPin<NodeContextObject> otherNode) {
	if (!otherNode) {
		return false;
	}
	auto pin = otherNode.pin();
	if (this->nodeType != pin->nodeType) {
		return false;
	}
	switch (this->nodeType) {
	case 1: {
		auto self = static_cast<ElementContextObject*>(this);
		auto other = static_cast<ElementContextObject*>(pin.native);
		bool pass = self->htmlUppercasedQualifiedName.Get(context->GetIsolate())->StringEquals(other->htmlUppercasedQualifiedName.Get(context->GetIsolate())) && self->attributes.pin(context->GetIsolate())->getAttributeList().size() == other->attributes.pin(context->GetIsolate())->getAttributeList().size();
		if (!pass) {
			return false;
		}
		//TODO compare each attribute for equality
	}
	case 2: {
		auto self = static_cast<AttrContextObject*>(this);
		auto other = static_cast<AttrContextObject*>(pin.native);
		return self->namespaceURI_GET(context)->StrictEquals(other->namespaceURI_GET(context)) && self->localName_GET(context)->StringEquals(other->localName_GET(context)) && self->value_GET(context)->StringEquals(other->value_GET(context));
	}
	case 3:
	case 8: {
		auto self = static_cast<CharacterDataContextObject*>(this);
		auto other = static_cast<CharacterDataContextObject*>(pin.native);
		return self->data_GET(context)->StringEquals(other->data_GET(context));
	}
	case 7: {
		auto self = static_cast<ProcessingInstructionContextObject*>(this);
		auto other = static_cast<ProcessingInstructionContextObject*>(pin.native);
		return self->data_GET(context)->StringEquals(other->data_GET(context)) && self->target_GET(context)->StringEquals(other->target_GET(context));
	}
	case 10: {
		auto self = static_cast<DocumentTypeContextObject*>(this);
		auto other = static_cast<DocumentTypeContextObject*>(pin.native);
		return self->name_GET(context)->StringEquals(other->name_GET(context)) && self->publicId_GET(context)->StringEquals(other->publicId_GET(context)) && self->systemId_GET(context)->StringEquals(other->systemId_GET(context));
	}
	}
	auto& selfNodes = this->childNodes.pin(context->GetIsolate())->vec();
	auto& otherNodes = pin->childNodes.pin(context->GetIsolate())->vec();
	if (selfNodes.size() != otherNodes.size()) {
		return false;
	}
	for (std::size_t i = 0; i < selfNodes.size(); i++) {
		if (!selfNodes[i].pin(context->GetIsolate())->isEqualNode_METHOD(context, otherNodes[i].pin(context->GetIsolate()))) {
			return false;
		}
	}
	return true;
}

v8::Local<v8::Value> locateNSPrefix(v8::Local<v8::Context> context, ElementContextObject* element, v8::Local<v8::String> ns) {
	if (element->namespaceURI_GET(context)->StrictEquals(ns)) {
		auto prefix = element->prefix_GET(context);
		if (!prefix->IsNull()) {
			return prefix;
		}
	}
	for (auto& attrProp : element->attributes_GET(context)->getAttributeList()) {
		auto attr = attrProp.pin(context->GetIsolate());
		if (attr->namespacePrefix_GET(context)->StrictEquals(v8::String::NewFromUtf8(context->GetIsolate(), "xmlns").ToLocalChecked()) && attr->value_GET(context)->StringEquals(ns)) {
			return attr->localName_GET(context);
		}
	}
	auto parent = element->parentElement_GET(context);
	return parent ? parent.pin()->lookupPrefix_METHOD(context, ns) : v8::Null(context->GetIsolate()).As<v8::Value>();
}

CO_METHOD(NodeContextObject::lookupPrefix, v8::Local<v8::Value>, v8::Local<v8::String> ns) {
	if (ns->Utf8Length(context->GetIsolate()) == 0) {
		return v8::Null(context->GetIsolate());
	}
	switch (this->nodeType) {
	case 1: {
		return locateNSPrefix(context, static_cast<ElementContextObject*>(this), ns);
	}
	case 9: {
		auto self = static_cast<DocumentContextObject*>(this);
		auto element = self->documentElement_GET(context);
		return element ? element.pin()->lookupPrefix_METHOD(context, ns) : v8::Null(context->GetIsolate()).As<v8::Value>();
	}
	case 2: {
		auto self = static_cast<AttrContextObject*>(this);
		auto element = self->ownerElement_GET(context);
		return element ? element.pin()->lookupPrefix_METHOD(context, ns) : v8::Null(context->GetIsolate()).As<v8::Value>();
	}
	case 10:
	case 11:
		return v8::Null(context->GetIsolate());
	default: {
		auto element = this->parentElement_GET(context);
		return element ? element.pin()->lookupPrefix_METHOD(context, ns) : v8::Null(context->GetIsolate()).As<v8::Value>();
	}
	}
}

v8::Local<v8::Value> locateNS(v8::Local<v8::Context> context, NodeContextObject* node, v8::Local<v8::String> prefix) {
	auto element = cpp::pins::NullPin<ElementContextObject>::null(context->GetIsolate());
	switch (node->type()) {
	case 2: {
		element = static_cast<AttrContextObject*>(node)->ownerElement_GET(context);
		break;
	}
	case 9: {
		element = static_cast<DocumentContextObject*>(node)->documentElement_GET(context);
		break;
	}
	case 10:
	case 11:
		return v8::Null(context->GetIsolate());
	case 1: {
		auto el = static_cast<ElementContextObject*>(node);
		auto ns = el->namespaceURI_GET(context);
		if (!ns->IsNull() && el->prefix_GET(context)->StrictEquals(prefix)) {
			return ns;
		}
		bool null = prefix->Utf8Length(context->GetIsolate());
		for (auto& attrProp : el->attributes_GET(context)->getAttributeList()) {
			auto attr = attrProp.pin(context->GetIsolate());
			auto ns = attr->namespaceURI_GET(context);
			auto pfx = attr->namespacePrefix_GET(context);
			auto localName = attr->localName_GET(context);
			if (ns->StrictEquals(v8::String::NewFromUtf8(context->GetIsolate(), "http://www.w3.org/2000/xmlns/").ToLocalChecked()) && (pfx->StrictEquals(v8::String::NewFromUtf8(context->GetIsolate(), "xmlns").ToLocalChecked()) && localName->StringEquals(prefix) || null && pfx->IsNull() && localName->StringEquals(v8::String::NewFromUtf8(context->GetIsolate(), "xmlns").ToLocalChecked()))) {
				auto value = attr->value_GET(context);
				return value->Utf8Length(context->GetIsolate()) ? value.As<v8::Value>() : v8::Null(context->GetIsolate()).As<v8::Value>();
			}
		}
	}
	default:
		element = node->parentElement_GET(context);
		break;
	}
	return element ? locateNS(context, element.native, prefix) : v8::Null(context->GetIsolate()).As<v8::Value>();
}


CO_METHOD(NodeContextObject::lookupNamespaceURI, v8::Local<v8::Value>, v8::Local<v8::String> prefix) {
	return locateNS(context, this, prefix);
}

CO_METHOD(NodeContextObject::isDefaultNamespace, v8::Local<v8::Boolean>, v8::Local<v8::Value> ns) {
	if (ns->IsString()) {
		if (!ns.As<v8::String>()->Utf8Length(context->GetIsolate())) {
			ns = v8::Null(context->GetIsolate());
		}
	}
	auto defaultNamespace = locateNS(context, this, v8::String::Empty(context->GetIsolate()));
	return v8::Boolean::New(context->GetIsolate(), defaultNamespace->StrictEquals(ns));
}