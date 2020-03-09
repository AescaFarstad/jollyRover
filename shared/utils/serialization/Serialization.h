#pragma once
#include <string>

#ifdef DEBUG_MODE
	#define WRITE_FIELD(object, serializer, fieldName) serializer.write(object.fieldName, #fieldName);
	#define READ__FIELD(object, serializer, fieldName) serializer.read(object.fieldName, #fieldName);
#else
	#define WRITE_FIELD(object, serializer, fieldName) serializer.write(object.fieldName);
	#define READ__FIELD(object, serializer, fieldName) serializer.read(object.fieldName);
#endif

const std::string anonymous;