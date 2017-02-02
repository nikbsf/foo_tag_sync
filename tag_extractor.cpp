#include "stdafx.h"

#include <sstream>

#include "tag_extractor.h"

extern advconfig_string_factory_MT cfg_synchronized_tags;

std::vector<pfc::string8> tag_extractor::get_synchronized_tags() {
	auto synchronized_tags = get_setting(cfg_synchronized_tags);

	std::string token;
	std::istringstream stream(synchronized_tags.get_ptr());

	std::vector<pfc::string8> result;
	while (std::getline(stream, token, ','))
		result.push_back(pfc::string8(token.c_str()));

	return result;
}

bool tag_extractor::is_empty(const file_info* info) {
	auto synchronized_tags = get_synchronized_tags();

	for (auto it = synchronized_tags.begin(); it != synchronized_tags.end(); ++it)
		if (info->meta_exists(*it) && info->meta_get(*it, 0) != NULL)
			return false;
	return true;
}

tags_data_t tag_extractor::get_tags_data(const file_info* info) {
	auto synchronized_tags = get_synchronized_tags();

	tags_data_t result;
	for (auto it = synchronized_tags.begin(); it != synchronized_tags.end(); ++it)
		if (info->meta_exists(*it))
			result.push_back(std::pair<pfc::string8, pfc::string8>(*it, info->meta_get(*it, 0)));
	return result;
}
