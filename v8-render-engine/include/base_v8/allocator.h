#pragma once

#include "v8.h"
#include "macros.h"

namespace js_objects {
	template<int size>
	inline v8::Local<v8::String> allocateString(v8::Isolate* isolate, const char (&str)[size]) {
		return v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal, size - 1).ToLocalChecked();
	}
	inline v8::Local<v8::String> allocateString(v8::Isolate* isolate, const char* str, int length) {
		return v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal, length).ToLocalChecked();
	}
	template<int size>
	inline v8::Local<v8::String> allocateCommonString(v8::Isolate* isolate, const char (&str)[size]) {
		return v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kInternalized, size - 1).ToLocalChecked();
	}
	inline v8::Local<v8::String> allocateCommonString(v8::Isolate* isolate, const char* str, int length) {
		return v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kInternalized, length).ToLocalChecked();
	}
}