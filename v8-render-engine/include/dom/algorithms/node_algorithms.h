#pragma once

#include <optional>

#include "base_v8/pins.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class NodeContextObject;
		}

		namespace mutation_algorithms {

#define BASIC_ARGS v8::Local<v8::Context> context, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> node, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> parent
#define CHILD_ARGS BASIC_ARGS, cpp::pins::NullPin<cpp::dom::nodes::NodeContextObject> child
#define KNOWN_CHILD_ARGS BASIC_ARGS, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> child

			bool ensurePreInsertionValidity(CHILD_ARGS);

			std::optional<pins::Pin<nodes::NodeContextObject>> preInsert(CHILD_ARGS);

			void insert(CHILD_ARGS, bool suppressObservers);

			std::optional<pins::Pin<nodes::NodeContextObject>> replace(KNOWN_CHILD_ARGS);

			void replaceAll(v8::Local<v8::Context> context, cpp::pins::NullPin<cpp::dom::nodes::NodeContextObject> node, cpp::pins::Pin<cpp::dom::nodes::NodeContextObject> parent);

			std::optional<pins::Pin<nodes::NodeContextObject>> preRemove(v8::Local<v8::Context> context, pins::Pin<nodes::NodeContextObject> child, pins::Pin<nodes::NodeContextObject> parent);

			void remove(pins::Pin<nodes::NodeContextObject> node, bool suppressObserversFlag);
		}
	}
}