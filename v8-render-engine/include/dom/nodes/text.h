#pragma once

#include "character_data.h"

namespace cpp {
	namespace dom {
		namespace nodes {
			class TextContextObject : public CharacterDataContextObject {
			public:
				CO_METHOD(splitText, pins::Pin<TextContextObject>, unsigned long offset);

				CO_READONLY_ATTRIBUTE(wholeText, v8::Local<v8::String>);

				MIXIN_SLOTABLE;
			};

			class CDATASectionContextObject : public TextContextObject {};

			class ProcessingInstructionContextObject : public CharacterDataContextObject {
			public:
				CO_TRANSPARENT_GETTER(target);
			private:
				v8::UniquePersistent<v8::String> target;
			};

			class CommentContextObject : public CharacterDataContextObject {};
		}
	}
}