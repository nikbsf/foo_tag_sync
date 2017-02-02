#pragma once

#include "shared.h"

class tag_extractor : public service_base {
	static std::vector<pfc::string8> get_synchronized_tags();

public:
	bool is_empty(const file_info* info);
	tags_data_t get_tags_data(const file_info* info);
};
