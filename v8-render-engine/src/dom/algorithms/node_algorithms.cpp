#include <optional>

#include "dom/algorithms/node_algorithms.h"
#include "dom/nodes/element.h"
#include "dom/nodes/attr.h"

using namespace cpp::dom::algorithms;

bool cpp::dom::algorithms::ensurePreInsertionValidity(CHILD_ARGS) {
	return true;//TODO exceptions
}

std::optional<cpp::pins::Pin<cpp::dom::nodes::NodeContextObject>> cpp::dom::algorithms::preInsert(CHILD_ARGS)
{
	if (!ensurePreInsertionValidity(context, node, parent, child)) {
		return {};
	}
	auto referenceChild = child;
	if (referenceChild == node) {
		referenceChild = node->nextSibling_GET(context);
	}
	insert(context, node, parent, referenceChild, false);
	return { node };
}

void cpp::dom::algorithms::insert(CHILD_ARGS, bool suppressObservers) {
	std::vector<pins::Pin<nodes::NodeContextObject>> nodes;
	if (node->domTypeof(DOMType::DocumentFragment)) {
		nodes.reserve(node->childNodes_GET(context)->size());
		for (auto& prop : node->childNodes_GET(context)->vec()) {
			nodes.push_back(prop.pin(context->GetIsolate()));
		}
	}
	else {
		nodes.push_back(node);
	}
	auto& children = parent->childNodes_GET(context)->vec();
	children.reserve(children.size() + nodes.size());
	std::vector<pins::Property<nodes::NodeContextObject>> popped;
	if (child) {
		//TODO live ranges
		auto it = children.begin();
		for (; it < children.end(); ++it) {
			if (it->pin(context->GetIsolate()) == child) {
				break;
			}
		}
		popped.reserve(children.end() - it);
		std::move(it, children.end(), std::back_inserter(popped));
		children.erase(it, children.end());
	}
	auto prevSibling = child ? child.pin()->previousSibling_GET(context) : parent->lastChild_GET(context);
	auto parentShadowHost = parent->domTypeof(DOMType::Element) && parent.downcast<nodes::ElementContextObject>()->getShadow(context);
	auto signalSlotChange = parent->getRootNode_METHOD(context, parent, false)->domTypeof(DOMType::ShadowRoot) && false;//TODO parent is a slot w/ empty assigned nodes
	auto nodeDocument = parent->ownerDocument_GET(context);
	for (auto n : nodes) {
		adopt(context, n, nodeDocument.pin());
		if (!children.empty()) {
			children.back().pin(context->GetIsolate())->setNextSibling(context, n);
			n->setPrevSibling(context, children.back().pin(context->GetIsolate()));
		}
		else {
			n->setPrevSibling(context, pins::NullPin<nodes::NodeContextObject>::null(context->GetIsolate()));
		}
		children.emplace_back(context->GetIsolate(), n);
		if (parentShadowHost && false) {//TODO node is a slotable
			//TODO assign a slot
		}
		if (signalSlotChange) {
			//TODO signal slot change
		}
		//TODO assign slotables for a tree
		//TODO do insertion steps
	}
	nodes.back()->setNextSibling(context, popped.empty() ? pins::NullPin<nodes::NodeContextObject>::null(context->GetIsolate()) : popped.front().pin(context->GetIsolate()));
	if (!popped.empty()) {
		popped.front().pin(context->GetIsolate())->setPrevSibling(context, nodes.back());
	}
	std::move(popped.begin(), popped.end(), std::back_inserter(children));
	popped.clear();
	if (!suppressObservers) {
		//TODO queue a tree mutation
	}
	//TODO children changed steps
}

bool ensurePreRemoveValidity(CHILD_ARGS) {
	return true;//TODO exceptions
}

bool ensurePreReplaceValidity(CHILD_ARGS) {
	return true;//TODO exceptions
}

std::optional<cpp::pins::Pin<cpp::dom::nodes::NodeContextObject>> cpp::dom::algorithms::replace(KNOWN_CHILD_ARGS) {
	if (!ensurePreReplaceValidity(context, node, parent, child)) {
		return {};
	}
	auto referenceChild = child->nextSibling_GET(context);
	if (referenceChild == node) {
		referenceChild = node->nextSibling_GET(context);
	}
	auto previousSibling = child->previousSibling_GET(context);
	//TODO handle removed nodes
	if (child->parent_GET(context)) {
		//TODO add child to removed nodes
		remove(context, child, true);
	}
	//TODO set nodes list
	insert(context, node, parent, referenceChild, true);
	//TODO queue tree mutation record
	return { child };
}

void cpp::dom::algorithms::replaceAll(v8::Local<v8::Context> context, cpp::pins::NullPin<cpp::dom::nodes::NodeContextObject> node, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> parent) {
	auto r_Nodes = parent->childNodes_GET(context);
	//TODO set addedNodes
	std::vector<pins::Pin<nodes::NodeContextObject>> removedNodes = {};
	removedNodes.reserve(r_Nodes->size());
	for (std::size_t i = 0; i < r_Nodes->size(); i++) {
		removedNodes.push_back(r_Nodes->operator[](i).pin(context->GetIsolate()));
	}
	for (std::size_t i = 0; i < removedNodes.size(); i++) {
		remove(context, removedNodes[i], true, false);
	}
	r_Nodes->vec().clear();
	if (node) {
		insert(context, node.pin(), parent, pins::NullPin<nodes::NodeContextObject>::null(context->GetIsolate()), true);
	}
	//TODO queue tree mutation record
}

std::optional<cpp::pins::Pin<cpp::dom::nodes::NodeContextObject>> cpp::dom::algorithms::preRemove(v8::Local<v8::Context> context, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> child, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> parent) {
	if (child->parent_GET(context) != parent) {
		//TODO exception
		return {};
	}
	remove(context, child, false);
	return { child };
}

void cpp::dom::algorithms::remove(v8::Local<v8::Context> context, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> node, bool suppressObserversFlag, bool doErasure) {
	auto parent = node->parent_GET(context);
	if (parent) {
		auto& children = parent.pin()->childNodes_GET(context)->vec();
		auto idx = 0;
		for (; idx < children.size(); idx++) {
			if (children[idx].pin(context->GetIsolate()) == node) {
				break;
			}
		}
		//TODO ranges
		//TODO nodeiterator
		if (doErasure) {
			children.erase(children.begin() + idx);
		}
		auto prev = node->previousSibling_GET(context);
		auto next = node->nextSibling_GET(context);
		if (prev) {
			prev.pin()->setNextSibling(context, next);
		}
		if (next) {
			next.pin()->setPrevSibling(context, prev);
		}
		node->dropSiblings(context);
		//TODO assigned
		//TODO slot changed
		//TODO descendant slot
		//TODO removing steps
		//TODO custom element callback
		//TODO custom descendants
		auto inclusiveParent = parent;
		auto& nodeObservers = node->getObservers();
		do {
			auto pin = inclusiveParent.pin();
			auto& observers = pin->getObservers();
			for (auto& registered : observers) {
				if (std::get<1>(registered).subtree) {
					auto pinned = std::get<0>(registered).pin(context->GetIsolate());
					auto pair = std::make_pair(
						pins::Property<MutationObserverContextObject>(context->GetIsolate(), pinned), std::get<1>(registered).clone(context));
					nodeObservers.push_back(std::make_tuple(pins::Property<MutationObserverContextObject>(context->GetIsolate(), pinned),
						std::get<1>(registered).clone(context), std::make_optional(std::move(pair))));
				}
			}
			inclusiveParent = pin->parent_GET(context);
		} while (inclusiveParent);
		if (!suppressObserversFlag) {
			//TODO queue tree mutation record
		}
		//TODO children changed
	}
}

void cpp::dom::algorithms::adopt(v8::Local<v8::Context> context, cpp::pins::Pin<nodes::NodeContextObject> node, pins::Pin<nodes::DocumentContextObject> document) {
	auto oldDocument = node->ownerDocument_GET(context);
	auto parent = node->parent_GET(context);
	if (parent) {
		remove(context, node, false);
	}
	if (document != oldDocument) {
		nodes::StackDescendantIterator it(node);
		do {
			it->setDocument(context, document);
			if (it->domTypeof(DOMType::Element)) {
				auto& attrs = static_cast<nodes::ElementContextObject&>(*it).attributes_GET(context)->getAttributeList();
				for (auto& attr : attrs) {
					attr.pin(context->GetIsolate())->setDocument(context, document);
				}
			}
			//TODO custom element reactions
			//TODO adopting steps
		} while (it.shadowNext(context));
	}
}