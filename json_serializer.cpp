#include "stdafx.h"

#include "shared.h"
#include "json_serializer.h"

pfc::string8 json_serializer::serialize(pfc::string8 key, tags_list_t item_tags_data) {
	pfc::string8 result;
	for (auto it = item_tags_data.begin(); it != item_tags_data.end(); ++it) {
		// dummy, todo
		result.add_string(it->first);
		result.add_string(it->second);
	}
	return result;
}
