#pragma once

#include <cstdint>
#include <string>
#include <map>
#include "v8.h"

namespace strings {

	static std::map<std::string, v8::UniquePersistent<v8::String>> reservedStringMap;

	v8::MaybeLocal<v8::String> toV8(v8::Isolate* isolate, std::string string) {
		auto it = reservedStringMap.find(string);
		if (it != reservedStringMap.end()) {
			return v8::MaybeLocal<v8::String>(it->second.Get(isolate));
		}
		else {
			v8::MaybeLocal<v8::String> str = v8::String::NewFromUtf8(isolate, string.c_str(), v8::NewStringType::kNormal, string.length());
			if (!str.IsEmpty()) {
				reservedStringMap[string] = v8::UniquePersistent<v8::String>(isolate, str.ToLocalChecked());
			}
			return str;
		}
	}
}