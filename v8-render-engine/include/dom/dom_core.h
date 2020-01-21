#pragma once

#include "base_v8/pins.h"
#include "base_v8/native.h"
#include "data_structs/enum_set.h"

namespace cpp {
	namespace dom {
		enum class DOMType { __COUNT__ };

		class DOMContextObject : public obj::BaseContextObject {
		public:
			bool typeof(DOMType type) {
				return this->types.contains(type);
			}
		private:
			data_structs::EnumSet<DOMType> types;
		};
	}
}