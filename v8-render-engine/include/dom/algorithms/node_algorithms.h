#pragma once

#include <optional>

#include "base_v8/pins.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class NodeContextObject;
			class DocumentContextObject;
			class ElementContextObject;
			class AttrContextObject;
		}

		namespace algorithms {

#define BASIC_ARGS v8::Local<v8::Context> context, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> node, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> parent
#define CHILD_ARGS BASIC_ARGS, cpp::pins::NullPin<cpp::dom::nodes::NodeContextObject> child
#define KNOWN_CHILD_ARGS BASIC_ARGS, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> child

			bool ensurePreInsertionValidity(CHILD_ARGS);

			std::optional<pins::Pin<nodes::NodeContextObject>> preInsert(CHILD_ARGS);

			void insert(CHILD_ARGS, bool suppressObservers);

			std::optional<pins::Pin<nodes::NodeContextObject>> replace(KNOWN_CHILD_ARGS);

			void replaceAll(v8::Local<v8::Context> context, cpp::pins::NullPin<cpp::dom::nodes::NodeContextObject> node, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> parent);

			std::optional<pins::Pin<nodes::NodeContextObject>> preRemove(v8::Local<v8::Context> context, pins::Pin<nodes::NodeContextObject> child, pins::Pin<nodes::NodeContextObject> parent);

			void remove(v8::Local<v8::Context> context, pins::Pin<nodes::NodeContextObject> node, bool suppressObserversFlag, bool doErasure=true);

			void adopt(v8::Local<v8::Context> context, pins::Pin<nodes::NodeContextObject> node, pins::Pin<nodes::DocumentContextObject> document);

			pins::Pin<nodes::NodeContextObject> clone(pins::Pin<nodes::NodeContextObject> node, pins::NullPin<nodes::DocumentContextObject> document, bool cloneChildren);

			void handleAttributeChanges(pins::Pin<nodes::AttrContextObject> attribute, pins::Pin<nodes::ElementContextObject> element, v8::Local<v8::String> oldValue, v8::Local<v8::String> newValue) {
				//TODO queue mutation record
				//TODO queue custom element callback
				//TODO attribute change steps
			}
		}
	}
}