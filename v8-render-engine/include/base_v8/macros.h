#pragma once
//Context object interface macros
#define CO_READONLY_ATTRIBUTE(attribute, type) type attribute##GET(v8::Isolate* isolate)
#define CO_TRANSPARENT_GETTER(attribute, type) CO_READONLY_ATTRIBUTE(attribute, v8::Local<type>) { this->attribute.Get(isolate); }
#define CO_ATTRIBUTE(attribute, type) CO_READONLY_ATTRIBUTE(attribute, type); void attribute##SET(v8::Isolate* isolate, type attr)
#define CO_METHOD(method, type, ...) type method##METHOD(v8::Isolate* isolate, __VA_ARGS__)
#define CO_INDEX(name, type, input_type) type name##INDEX(v8::Isolate* isolate, input_type i)