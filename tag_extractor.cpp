#include "stdafx.h"

#include <sstream>

#include "tag_extractor.h"

extern advconfig_string_factory_MT cfg_required_tags;
extern advconfig_string_factory_MT cfg_synchronized_tags;

std::set<std::string> tag_extractor::parse_setting(advconfig_string_factory_MT& setting) {
	auto synchronized_tags = get_setting(setting);

	std::string token;
	std::istringstream stream(synchronized_tags.get_ptr());

	std::set<std::string> result;
	while (std::getline(stream, token, ','))
		result.insert(token.c_str());

	return result;
}

bool tag_extractor::is_exportable(const file_info* info) {
	auto required_tags = parse_setting(cfg_required_tags);

	for (auto it = required_tags.begin(); it != required_tags.end(); ++it)
		if (!info->meta_exists(it->c_str()) || info->meta_get(it->c_str(), 0) == NULL)
			return false;
	return true;
}

bool tag_extractor::is_empty(const file_info* info) {
	auto synchronized_tags = parse_setting(cfg_synchronized_tags);

	for (auto it = synchronized_tags.begin(); it != synchronized_tags.end(); ++it)
		if (info->meta_exists(it->c_str()) && info->meta_get(it->c_str(), 0) != NULL)
			return false;
	return true;
}

tags_data_t tag_extractor::get_tags_data(const file_info* info) {
	auto synchronized_tags = parse_setting(cfg_synchronized_tags);

	tags_data_t result;
	for (auto it = synchronized_tags.begin(); it != synchronized_tags.end(); ++it)
		if (info->meta_exists(it->c_str()))
			result.push_back(std::pair<pfc::string8, pfc::string8>(it->c_str(), info->meta_get(it->c_str(), 0)));
	return result;
}

void tag_extractor::set_tags_data(file_info* const info, const tags_data_t& tags_data) {
	auto synchronized_tags_set = parse_setting(cfg_synchronized_tags);

	for (auto it = tags_data.begin(); it != tags_data.end(); ++it) {
		if (synchronized_tags_set.find(it->first.c_str()) != synchronized_tags_set.end())
			info->meta_set(it->first, it->second);
	}
}
