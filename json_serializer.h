#pragma once

class json_serializer : public service_base {
public:
	pfc::string8 serialize(pfc::string8 key, tags_list_t item_tags_data);
};
