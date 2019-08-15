#pragma once
//Context object macros
#define CO_GETTER(attribute, type) type attribute##GET()
#define CO_SETTER(attribute, type) void attribute##SET(type attr)
#define CO_ATTRIBUTE(attribute, type) CO_GETTER(attribute, type); CO_SETTER(attribute, type)
#define CO_READONLY_ATTRIBUTE(attribute, type) CO_GETTER(attribute, type)