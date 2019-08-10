#pragma once

#include "macros.h"
#include "v8.h"

namespace dom {

	namespace nodes {
		class NodeContextObject;
	}

	namespace collections {
		class NodeListContextObject {

			Return<false>::Type<nodes::NodeContextObject, v8::Value> itemMETHOD(std::size_t idx, v8::Isolate* isolate);

			std::size_t lengthGET(v8::Isolate* isolate) { return this->nodes.Get(isolate)->Length(); }

			//TODO: Iterable<Node>

		private:
			v8::UniquePersistent<v8::Array> nodes;
		};
	}
}