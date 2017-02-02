#pragma once

#include "shared.h"

class json_serializer : public service_base {
public:
	std::string serialize(pfc::string8 key, const tags_data_t& item_tags_data);
};
