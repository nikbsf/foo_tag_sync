#pragma once

#define COMPONENT_NAME "foo_tag_sync"
#define COMPONENT_TITLE "Tag Synchronizer"

typedef std::vector<std::pair<pfc::string8, pfc::string8>> tags_list_t;
typedef std::map<pfc::string8, pfc::string8> serialized_tags_dict_t;

inline pfc::string8 get_setting(advconfig_string_factory &setting) {
	pfc::string8 result;
	setting.get(result);
	return result;
}
