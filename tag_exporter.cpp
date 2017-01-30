#include "stdafx.h"

#include "tag_exporter.h"


void tag_exporter::export_tags(metadb_handle_list_cref items, const pfc::list_base_const_t<const file_info*>* file_infos) {
	std::map<pfc::string8, pfc::string8> tags_by_keys;

	console::printf(COMPONENT_NAME ": exporting %d items...", items.get_count());
	for (auto i = 0; i < items.get_count(); ++i) {
		auto item_metadb_handle = items[i];
		const file_info* item_file_info = file_infos != NULL ? (*file_infos)[i] : &items[i]->get_info_ref()->info();

		auto key = key_provider_service->get_key(item_metadb_handle);
		auto tags = tag_extractor_service->get_tags_with_values(item_file_info);
		tags_by_keys[key] = json_serializer_service->serialize(key, tags);
	}
	tag_storage_service->save_tags(tags_by_keys);

	console::print(COMPONENT_NAME ": export complete");
}
