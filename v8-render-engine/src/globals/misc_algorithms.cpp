#include "nodes/elements/element.h"
#include "nodes/character_data/character_data.h"
#include "globals/misc_algorithms.h"

bool dom::algorithms::checkSlotable(v8::Local<v8::Context> context, js_objects::Pin<dom::events::EventTargetContextObject> target) {
	if (target->typeof(js_objects::ObjectType::Node)) {
		auto type = target.cast<dom::nodes::NodeContextObject>()->nodeTypeGET(context);
		return type == dom::nodes::NodeType::ELEMENT_NODE && target.cast<dom::nodes::ElementContextObject>()->assignedSlotGET(context)
			|| type == dom::nodes::NodeType::TEXT_NODE && target.cast<dom::nodes::TextContextObject>()->assignedSlotGET(context);
	}
	return false;
}

js_objects::Pin<dom::events::EventTargetContextObject> dom::algorithms::retarget(v8::Local<v8::Context> context, js_objects::Pin<dom::events::EventTargetContextObject> a, js_objects::Pin<dom::events::EventTargetContextObject> b)
{
	while (true) {
		if (a->typeof(js_objects::ObjectType::Node)) {
			auto node = a.cast<dom::nodes::NodeContextObject>();
			auto root = node->getRoot(context);
			if (root->typeof(js_objects::ObjectType::ClosedShadowRoot) || root->typeof(js_objects::ObjectType::OpenShadowRoot)) {
				if (!b->typeof(js_objects::ObjectType::Node) || !root->privilegedContains(context, b.cast<dom::nodes::NodeContextObject>())) {
					a = node->getRoot(context).cast<events::EventTargetContextObject>();
					continue;
				}
			}
		}
		return a;
	}
}