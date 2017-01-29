#pragma once

#define COMPONENT_NAME "foo_tag_sync"
#define COMPONENT_TITLE "Tag Synchronizer"

inline pfc::string8 get_setting(advconfig_string_factory &setting) {
	pfc::string8 result;
	setting.get(result);
	return result;
}
