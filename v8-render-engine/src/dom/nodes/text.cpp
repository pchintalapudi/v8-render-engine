#include "dom/nodes/text.h"

using namespace cpp::dom::nodes;

CO_READONLY_ATTRIBUTE(TextContextObject::wholeText, v8::Local<v8::String>) {
	std::vector<pins::Pin<TextContextObject>> rPrev;
	std::vector<pins::Pin<TextContextObject>> next;
	auto sib = this->previousSibling_GET(context);
	while (sib) {
		auto pin = sib.pin();
		if (pin->domTypeof(DOMType::Text)) {
			rPrev.push_back(pin.downcast<nodes::TextContextObject>());
			sib = sib.pin()->previousSibling_GET(context);
		}
		else {
			break;
		}
	}
	sib = this->nextSibling_GET(context);
	while (sib) {
		auto pin = sib.pin();
		if (pin->domTypeof(DOMType::Text)) {
			rPrev.push_back(pin.downcast<nodes::TextContextObject>());
			sib = sib.pin()->nextSibling_GET(context);
		}
		else {
			break;
		}
	}
	std::size_t length = 8;
	char* cbuf = static_cast<char*>(std::malloc(length + static_cast<std::size_t>(1)));
	if (!cbuf) {
		throw std::bad_alloc();
	}
	std::string buffer;
	for (auto i = rPrev.size(); i-- > 0;) {
		auto s = rPrev[i]->data_GET(context);
		auto len = s->Utf8Length(context->GetIsolate());
		if (len > length) {
			do {} while (length <<= 1 < len);
			auto buf = static_cast<char*>(std::realloc(cbuf, length + static_cast<std::size_t>(1)));
			if (!buf) {
				throw std::bad_alloc();
			}
			else {
				cbuf = buf;
			}
		}
		s->WriteUtf8(context->GetIsolate(), cbuf);
#pragma warning(push)
#pragma warning(disable:6385)
		buffer.append(cbuf, len);
#pragma warning(pop)
	}
	std::free(cbuf);
	return v8::String::NewFromUtf8(context->GetIsolate(), buffer.c_str()).ToLocalChecked();
}