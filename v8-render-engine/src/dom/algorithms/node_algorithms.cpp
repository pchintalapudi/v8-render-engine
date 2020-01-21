#include <optional>

#include "dom/algorithms/node_algorithms.h"
#include "dom/nodes/node.h"

using namespace cpp::dom::mutation_algorithms;

bool cpp::dom::mutation_algorithms::ensurePreInsertionValidity(CHILD_ARGS) {
	return true;//TODO exceptions
}

std::optional<cpp::pins::Pin<cpp::dom::nodes::NodeContextObject>> cpp::dom::mutation_algorithms::preInsert(CHILD_ARGS)
{
	if (!ensurePreInsertionValidity(context, node, parent, child)) {
		return {};
	}
	auto referenceChild = child;
	if (referenceChild == node) {
		referenceChild = node->nextSibling_GET(context);
	}
	insert(context, node, parent, referenceChild, false);
	return node;
}

void cpp::dom::mutation_algorithms::insert(CHILD_ARGS, bool suppressObservers) {}//TODO insert

bool ensurePreRemoveValidity(CHILD_ARGS) {
	return true;//TODO exceptions
}

std::optional<cpp::pins::Pin<cpp::dom::nodes::NodeContextObject>> cpp::dom::mutation_algorithms::replace(KNOWN_CHILD_ARGS) {
	if (!ensurePreRemoveValidity(context, node, parent, child)) {
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
		remove(child, true);
	}
	//TODO set nodes list
	insert(context, node, parent, referenceChild, true);
	//TODO queue tree mutation record
	return { child };
}

void cpp::dom::mutation_algorithms::replaceAll(v8::Local<v8::Context> context, cpp::pins::NullPin<cpp::dom::nodes::NodeContextObject> node, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> parent) {
	auto r_Nodes = parent->childNodes_GET(context);
	//TODO addedNodes = []
	//TODO set addedNodes
	std::vector<pins::Pin<nodes::NodeContextObject>> removedNodes = {};
	removedNodes.reserve(r_Nodes->size());
	for (std::size_t i = 0; i < r_Nodes->size(); i++) {
		removedNodes.push_back(r_Nodes->operator[](i).pin(context->GetIsolate()));
	}
	for (std::size_t i = 0; i < removedNodes.size(); i++) {
		remove(removedNodes[i], true);
	}
	if (node) {
		insert(context, node.pin(), parent, pins::NullPin<nodes::NodeContextObject>::null(context->GetIsolate()), true);
	}
	//TODO queue tree mutation record
}

std::optional<cpp::pins::Pin<cpp::dom::nodes::NodeContextObject>> cpp::dom::mutation_algorithms::preRemove(v8::Local<v8::Context> context, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> child, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> parent) {
	if (child->parent_GET(context) != parent) {
		//TODO exception
		return {};
	}
	remove(child, false);
	return { child };
}

void cpp::dom::mutation_algorithms::remove(cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> node, bool suppressObserversFlag) {}//TODO remove
