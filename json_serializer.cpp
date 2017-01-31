#include "stdafx.h"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

#include "json_serializer.h"

using namespace rapidjson;

pfc::string8 json_serializer::serialize(pfc::string8 key, tags_list_t item_tags_data) {
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	Document document;

	document.SetObject();
	for (auto it = item_tags_data.begin(); it != item_tags_data.end(); ++it) {
		Value tag_name (kStringType), tag_value;
		tag_name.SetString(it->first.get_ptr(), it->first.get_length());
		tag_value.SetString(it->second.get_ptr(), it->second.get_length());
		document.AddMember(tag_name, tag_value, document.GetAllocator());
	}
	
	document.Accept(writer);
	return buffer.GetString();
}
