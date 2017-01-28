#include "stdafx.h"

#include "key_provider.h"

extern advconfig_string_factory cfg_key_format;


pfc::string8 key_provider::get_key(metadb_handle_ptr item) const {
	pfc::string8 result;

	auto key_format_script = get_key_format_script();
	item->format_title(NULL, result, key_format_script, NULL);
	return result;
}

titleformat_object::ptr key_provider::get_key_format_script() const {
	pfc::string8 key_format;
	cfg_key_format.get(key_format);

	titleformat_object::ptr result;
	if (static_api_ptr_t<titleformat_compiler>()->compile(result, key_format))
		return result;
}