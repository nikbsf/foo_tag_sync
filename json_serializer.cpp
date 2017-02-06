#include "stdafx.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include "json_serializer.h"

using namespace rapidjson;

std::string json_serializer::serialize(pfc::string8 key, const tags_data_t& tags_data) {
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	Document document;

	document.SetObject();
	for (auto it = tags_data.begin(); it != tags_data.end(); ++it) {
		Value tag_name (kStringType), tag_value;
		tag_name.SetString(it->first.get_ptr(), it->first.get_length());
		tag_value.SetString(it->second.get_ptr(), it->second.get_length());
		document.AddMember(tag_name, tag_value, document.GetAllocator());
	}
	
	document.Accept(writer);
	return buffer.GetString();
}

tags_data_t json_serializer::deserialize(std::string serialized_data) {
	Document document;
	document.Parse(serialized_data.c_str());

	tags_data_t result;
	for (auto it = document.MemberBegin(); it != document.MemberEnd(); ++it)
		result.push_back(std::pair<pfc::string8, pfc::string8>(it->name.GetString(), it->value.GetString()));

	return result;
}
