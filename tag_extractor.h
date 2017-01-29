#pragma once

#include <vector>

class tag_extractor : public service_base {
	static std::vector<pfc::string8> get_synchronized_tags();

public:
	std::vector<std::pair<pfc::string8, pfc::string8>> get_tags_with_values(const file_info* info);
};
