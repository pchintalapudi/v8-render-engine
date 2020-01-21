#pragma once

#include "x_path_expression.h"

#define MIXIN_XPATH_EVALUATOR_BASE CO_METHOD(createExpression, pins::Pin<xpath::XPathExpressionContextObject>,\
v8::Local<v8::String> expression, v8::Local<v8::Value> resolver);\
CO_METHOD(createNSResolver, v8::Local<v8::Function>, pins::Pin<nodes::NodeContextObject> node);\
CO_METHOD(evaluate, pins::Pin<xpath::XPathResultContextObject>, v8::Local<v8::String> expression,\
 pins::Pin<nodes::NodeContextObject> contextNode, v8::Local<v8::Value> resolver, uint32_t type,\
 pins::NullPin<xpath::XPathResultContextObject> result);