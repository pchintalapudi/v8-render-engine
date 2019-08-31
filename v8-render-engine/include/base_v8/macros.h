#pragma once
//Context object interface macros
#define CO_READONLY_ATTRIBUTE(attribute, type) type attribute##GET(v8::Local<v8::Context> context)
#define CO_TRANSPARENT_GETTER(attribute, type) CO_READONLY_ATTRIBUTE(attribute, v8::Local<type>) { this->attribute.Get(context->GetIsolate()); }
#define CO_ATTRIBUTE(attribute, type) CO_READONLY_ATTRIBUTE(attribute, type); void attribute##SET(v8::Local<v8::Context> context, type attr)
#define CO_INLINE_ATTRIBUTE_GETTER(attribute, type) CO_READONLY_ATTRIBUTE(attribute, type)
#define CO_INLINE_ATTRIBUTE_SETTER(attribute, type) void attribute##SET(v8::Local<v8::Context> context, type attr)
#define CO_METHOD(method, type, ...) type method##METHOD(v8::Local<v8::Context> context, __VA_ARGS__)
#define CO_INDEX(name, type, input_type) type name##INDEX(v8::Local<v8::Context> context, input_type i)