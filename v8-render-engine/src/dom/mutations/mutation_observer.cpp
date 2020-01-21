#include <algorithm>
#include <set>

#include "dom/mutations/mutation_observer.h"
#include "dom/nodes/node.h"

using namespace cpp::dom;

CO_METHOD(cpp::dom::MutationObserverContextObject::observe, void, pins::Pin<MutationObserverContextObject> self, pins::Pin<nodes::NodeContextObject> target, MutationObserverInit options) {
	bool found = false;
	for (auto& registered : target->getObservers()) {
		auto& [reg_observer, reg_init, _] = registered;
		auto reg_observer_ref = reg_observer.pin(context->GetIsolate());
		auto& reg_init_ref = reg_init;
		if (reg_observer_ref.native == this) {
			for (auto& weakNode : this->nodeList) {
				auto wPin = weakNode.pin(context->GetIsolate());
				if (wPin) {
					auto pin = wPin.pin();
					auto& observers = pin->getObservers();
					observers.erase(std::remove_if(observers.begin(), observers.end(), [context, reg_observer_ref, &reg_init_ref](std::tuple<pins::Property<MutationObserverContextObject>, MutationObserverInit, std::optional<std::pair<pins::Property<MutationObserverContextObject>, MutationObserverInit>>>& tup) {
						auto& [_, __, opt] = tup;
						return opt && opt->first.pin(context->GetIsolate()) == reg_observer_ref && opt->second == reg_init_ref;
					}));
				}
			}
			reg_init = std::move(options);
			found = true;
		}
	}
	if (!found) {
		ObserverSource opt = {};
		auto tup = std::make_tuple(pins::Property(context->GetIsolate(), self), std::move(options), std::move(opt));
		target->getObservers().push_back(std::move(tup));
		this->nodeList.emplace_back(context->GetIsolate(), target.js_handle().As<v8::Object>(), target.native);
	}
}

CO_METHOD(MutationObserverContextObject::disconnect, void) {
	for (auto& weakNode : this->nodeList) {
		auto wPin = weakNode.pin(context->GetIsolate());
		if (wPin) {
			auto node = wPin.pin();
			auto& observers = node->getObservers();
			observers.erase(std::remove_if(observers.begin(), observers.end(), [context, this](RegisteredObserver& observer) {
				return std::get<0>(observer).pin(context->GetIsolate()).native == this;
			}));
		}
	}
}