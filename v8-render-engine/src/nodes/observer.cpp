#include "nodes/observer.h"
#include "nodes/node.h"

v8::Local<v8::Array> dom::observer::MutationObserverContextObject::takeRecordsMETHOD(v8::Local<v8::Context> context) {
	auto isolate = context->GetIsolate();
	v8::EscapableHandleScope ehs(isolate);
	auto records = v8::Array::New(isolate);
	for (uint32_t i = 0; i < this->recordQueue.size(); i++) {
		records->Set(context, i, this->recordQueue[i].jsRef.Get(isolate));
	}
	this->recordQueue.clear();
	return ehs.Escape(records);
}

void dom::observer::MutationObserverContextObject::disconnectMETHOD(v8::Local<v8::Context> context) {
	auto isolate = context->GetIsolate();
	for (const auto& weak : this->weakNodeList) {
		if (auto maybe = weak.lock(isolate); !maybe.first.IsEmpty()) {
			auto& observers = maybe.second->getObservers();
			observers.erase(std::remove_if(observers.begin(), observers.end(),
				[this](const std::pair<js_objects::JS_CPP_Property<MutationObserverContextObject>, data_structs::EnumSet<MutationObserverOptions>>& prop)
			{return this == prop.first.cppRef; }));
		}
	}
}

void dom::observer::MutationObserverContextObject::observeMETHOD(v8::Local<v8::Context> context,
	nodes::NodeContextObject& node, data_structs::EnumSet<MutationObserverOptions> options,
	std::deque<std::string> attributeFilter) {
	auto isolate = context->GetIsolate();
	bool wasRegistered = false;
	for (auto& registered : node.getObservers()) {
		if (registered.first.cppRef == this) {
			for (const auto& weak : this->weakNodeList) {
				if (auto maybe = weak.lock(isolate); !maybe.first.IsEmpty()) {
					auto& observers = maybe.second->getObservers();
					observers.erase(std::remove_if(observers.begin(), observers.end(),
						[this](std::pair<js_objects::JS_CPP_Property<MutationObserverContextObject>,
							data_structs::EnumSet<MutationObserverOptions>>&prop) {
						return prop.first.cppRef->typeof(js_objects::ObjectType::TransientMutationObserver) && static_cast<TransientMutationObserverContextObject*>(prop.first.cppRef)->source.cppRef == this;
					}));
					registered.second = options;
				}
			}
			wasRegistered = true;
		}
	}
	if (!wasRegistered) {
		node.registerObserver(js_objects::JS_CPP_Property(isolate, *this), options);
		this->weakNodeList.emplace_back(isolate, node);
	}
}