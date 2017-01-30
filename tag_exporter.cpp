#include "stdafx.h"

#include "tag_exporter.h"


void tag_exporter::export_tags(metadb_handle_list_cref items, const pfc::list_base_const_t<const file_info*>& file_infos) {
	std::map<pfc::string8, pfc::string8> tags_by_keys;
	for (auto i = 0; i < items.get_count(); ++i) {
		auto key = key_provider_service->get_key(items[i]);
		auto tags = tag_extractor_service->get_tags_with_values(file_infos[i]);
		tags_by_keys[key] = json_serializer_service->serialize(key, tags);
	}
	tag_storage_service->save_tags(tags_by_keys);
}
