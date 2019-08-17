#pragma once
#include "base_v8/root.h"
#include "base_v8/macros.h"

namespace dom {

	namespace nodes {
		class NodeContextObject;
	}

	namespace utils {

		class NodeFilterContextObject : public js_objects::BaseContextObject {
			CO_METHOD(acceptNode, unsigned short, nodes::NodeContextObject& node);
		};

		class NodeIteratorContextObject : public js_objects::BaseContextObject {
		public:
			CO_READONLY_ATTRIBUTE(root, nodes::NodeContextObject&);

			CO_READONLY_ATTRIBUTE(referenceNode, nodes::NodeContextObject&);

			CO_READONLY_ATTRIBUTE(pointerBeforeReferenceNode, bool);

			CO_READONLY_ATTRIBUTE(whatToShow, std::size_t);

			CO_READONLY_ATTRIBUTE(filter, NodeFilterContextObject*);

			CO_METHOD(nextNode, nodes::NodeContextObject*);
			CO_METHOD(previousNode, nodes::NodeContextObject*);

			CO_METHOD(detach, void);
		};

		class TreeWalkerContextObject : public js_objects::BaseContextObject {
		public:
			CO_READONLY_ATTRIBUTE(root, nodes::NodeContextObject&);

			CO_READONLY_ATTRIBUTE(whatToShow, std::size_t);

			CO_READONLY_ATTRIBUTE(nodeFilter, NodeFilterContextObject*);

			CO_ATTRIBUTE(currentNode, nodes::NodeContextObject&);

			CO_METHOD(parentNode, nodes::NodeContextObject*);
			CO_METHOD(firstChild, nodes::NodeContextObject*);
			CO_METHOD(lastChild, nodes::NodeContextObject*);
			CO_METHOD(previousSibling, nodes::NodeContextObject*);
			CO_METHOD(nextSibling, nodes::NodeContextObject*);
			CO_METHOD(previousNode, nodes::NodeContextObject*);
			CO_METHOD(nextNode, nodes::NodeContextObject*);
		};
	}
}