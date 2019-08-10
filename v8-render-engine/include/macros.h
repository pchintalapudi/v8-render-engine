#pragma once
#include "v8.h"
#define ISOLATE v8::Isolate* isolate = v8::Isolate::GetCurrent();
#define CONTEXT v8::Local<v8::Context> context = isolate->GetCurrentContext();
#define HANDLE_SCOPE v8::HandleScope handleScope(isolate);
#define ESCAPABLE_HANDLE_SCOPE v8::EscapableHandleScope escapableHandleScope(isolate);
//Note the lack of semicolons here
#define ESCAPE(LOCAL) escapableHandleScope.Escape(LOCAL)
#define ESCAPE_GLOBAL(GLOBAL) escapableHandleScope.Escape(GLOBAL.Get(isolate))
//V8 Properties
#define GET_PROPERTY(NAME, TYPE) void Get##NAME##(v8::Local<##TYPE##> prop, const v8::PropertyCallbackInfo<v8::Value>& info)
#define SET_PROPERTY(NAME, TYPE) void Set##NAME##(v8::Local<##TYPE##> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
#define RETURN_PROPERTY(RETURN_VALUE) info.GetReturnValue().Set(RETURN_VALUE)

template<typename Type>
Type& castInternalField(v8::Local<v8::Object> obj, v8::Isolate* isolate) {
	return *static_cast<Type*>(v8::Local<v8::External>::Cast(obj->GetInternalField(0))->Value());
}

template<typename ContextObject, typename v8Map>
struct ReturnTypeImpl {
	ContextObject* contextObject;
	v8::Local<v8Map> v8Return;
	bool exceptionThrown;
};

template<typename ContextObject>
struct RawReturnImpl {
	ContextObject obj;
	bool exceptionThrown;
};

template<bool raw>
struct Return {
	template<typename ContextObject, typename v8Map>
	using Type = ReturnTypeImpl<ContextObject, v8Map>;
};

template<>
struct Return<true> {
	template<typename ContextObject>
	using Type = std::conditional<std::is_void<ContextObject>, bool, RawReturnImpl<ContextObject>>;
};