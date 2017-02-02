#pragma once

#include "shared.h"

class tag_extractor : public service_base {
	static std::vector<pfc::string8> parse_setting(advconfig_string_factory_MT &setting);

public:
	bool is_exportable(const file_info* info);
	bool is_empty(const file_info* info);
	tags_data_t get_tags_data(const file_info* info);
};
