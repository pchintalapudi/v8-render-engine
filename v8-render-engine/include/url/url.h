#pragma once

#include <optional>

#include "dom/dom_core.h"

namespace cpp {
	namespace url {
		class URLContextObject : public dom::DOMContextObject {
		public:
			v8::Local<v8::String> serialize(v8::Local<v8::Context> context) {
				return this->serialized.Get(context->GetIsolate());
			}
		private:
			v8::UniquePersistent<v8::String> scheme;
			enum class SchemeType {
				NON_FILE_SPECIAL, FILE, NON_SPECIAL
			};
			SchemeType schemeType;
			v8::UniquePersistent<v8::String> host;
			v8::UniquePersistent<v8::String> username;
			v8::UniquePersistent<v8::String> password;
			enum class HostType {
				HOST_DOMAIN, IPV4, IPV6, OPAQUE_HOST, EMPTY_HOST, HOST_NULL
			};
			HostType hostType;
			std::optional<uint16_t> port;
			v8::UniquePersistent<v8::Array> path;
			v8::UniquePersistent<v8::String> query;
			v8::UniquePersistent<v8::String> fragment;
			bool cannotBeABaseFlag;
			//TODO blobURLEntry
			v8::UniquePersistent<v8::String> serialized;
		};
	}
}