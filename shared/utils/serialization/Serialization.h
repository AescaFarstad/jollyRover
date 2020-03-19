#pragma once
#include <string>

#if DEBUG_MODE
	#define WRITE_FIELD(object, serializer, fieldName) serializer.write(object.fieldName, #fieldName);
	#define READ__FIELD(object, serializer, fieldName) serializer.read(object.fieldName, #fieldName);
	#define FIELD_NAME(name) #name
#else
	#define WRITE_FIELD(object, serializer, fieldName) serializer.write(object.fieldName);
	#define READ__FIELD(object, serializer, fieldName) serializer.read(object.fieldName);
	#define FIELD_NAME(name) anonymous
#endif

const std::string anonymous;