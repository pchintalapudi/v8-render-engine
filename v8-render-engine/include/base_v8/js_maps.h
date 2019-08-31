#pragma once

#include "v8.h"

namespace js_objects {
	enum class ObjectType {
		Event, EventTarget, Node, ClosedShadowRoot, OpenShadowRoot, MouseEvent, Window, MutationRecord,
		MutationObserver, TransientMutationObserver, __COUNT__
	};

	class BaseContextObject;

	template<ObjectType objectType>
	struct ObjectTemplateInstantiator {
	};

#define _specialization(objectType) ObjectTemplateInstantiator<ObjectType::objectType>
#define _declare(objectType) template<> struct _specialization(objectType)
	//All forward type declarations go here
	_declare(Event);
	_declare(EventTarget);
	_declare(Node);
	_declare(ClosedShadowRoot);
	_declare(OpenShadowRoot);
	_declare(MouseEvent);
	_declare(Window);

#define define_template(objectType, realType) _declare(objectType) {static v8::UniquePersistent<v8::ObjectTemplate> templ; static v8::Local<v8::ObjectTemplate> getTemplate(v8::Local<v8::Context> context) {if (templ.IsEmpty()) {auto local = instantiate(context); templ = v8::UniquePersistent<v8::ObjectTemplate>(context->GetIsolate(), local); return local;} else return templ.Get(context->GetIsolate());} static v8::Local<v8::ObjectTemplate> instantiate(v8::Local<v8::Context> context); typedef realType type; static data_structs::EnumSet<js_objects::ObjectType> types();}
#define define_instantiator(objectType, realType) v8::Local<v8::ObjectTemplate> js_objects::_specialization(objectType)::instantiate(v8::Local<v8::Context> context)


	template<ObjectType objectType>
	v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) {
		return ObjectTemplateInstantiator<objectType>::instantiate(isolate);
	}
}