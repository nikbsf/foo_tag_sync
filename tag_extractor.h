#pragma once

#include "shared.h"

class tag_extractor : public service_base {
	static std::vector<pfc::string8> get_synchronized_tags();

public:
	tags_list_t get_tags_with_values(const file_info* info);
};
