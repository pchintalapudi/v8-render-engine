#pragma once

#define CO_READONLY_ATTRIBUTE(attribute, type) type attribute##_GET(v8::Local<v8::Context> context)
#define CO_WRITEONLY_ATTRIBUTE(attribute, type) void attribute##_SET(v8::Local<v8::Context> context, type attr)
#define CO_TRANSPARENT_GETTER(attribute) CO_READONLY_ATTRIBUTE(attribute, auto) { return this->attribute.Get(context->GetIsolate()); }
#define CO_PIN_GETTER(attribute) CO_READONLY_ATTRIBUTE(attribute, auto) { return this->attribute.pin(context->GetIsolate()); }
#define CO_ATTRIBUTE(attribute, type) CO_READONLY_ATTRIBUTE(attribute, type); CO_WRITEONLY_ATTRIBUTE(attribute, type);
#define CO_INLINE_ATTRIBUTE_GETTER(attribute, type) CO_READONLY_ATTRIBUTE(attribute, type)
#define CO_INLINE_ATTRIBUTE_SETTER(attribute, type) CO_WRITEONLY_ATTRIBUTE(attribute, type)
#define CO_METHOD(method, type, ...) type method##_METHOD(v8::Local<v8::Context> context, __VA_ARGS__)
#define CO_INDEX(name, type, input_type) type name##_INDEX(v8::Local<v8::Context> context, input_type i)
#define CO_STATIC_UINT32_ATTRIBUTE(attribute, number) CO_READONLY_ATTRIBUTE(attribute, v8::Local<v8::Integer>) { return v8::Uint32::New(context->GetIsolate(), number); }
#define CO_PRIMITIVE_GETTER(attribute, type) CO_READONLY_ATTRIBUTE(attribute, auto) { return type::New(context->GetIsolate(), this->attribute); }
#define CO_ALIAS_GETTER(name, alias) CO_READONLY_ATTRIBUTE(name, auto) { return this->alias##_GET(context); }