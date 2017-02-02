#include "stdafx.h"

#include "tag_exporter.h"


bool tag_exporter::is_empty(const tags_list_t& tags) {
	for (auto it = tags.begin(); it != tags.end(); ++it)
		if (!it->second.is_empty())
			return false;
	return true;
}

void tag_exporter::export_tags(metadb_handle_list_cref items, const pfc::list_base_const_t<const file_info*>* file_infos) {
	serialized_tags_dict_t serialized_tags;

	auto exported_items_count = 0;
	for (auto i = 0; i < items.get_count(); ++i) {
		auto item_metadb_handle = items[i];
		auto item_file_info = file_infos != NULL ? (*file_infos)[i] : &items[i]->get_info_ref()->info();

		auto key = key_provider_service->get_key(item_metadb_handle);
		auto tags = tag_extractor_service->get_tags_with_values(item_file_info);

		if (is_empty(tags))
			continue;

		serialized_tags[key] = json_serializer_service->serialize(key, tags);
		++exported_items_count;
	}
	tag_storage_service->save_tags(serialized_tags);

	console::printf(COMPONENT_NAME ": exported %d of %d", exported_items_count, items.get_count());
}
