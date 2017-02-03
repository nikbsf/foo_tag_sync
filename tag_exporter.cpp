#include "stdafx.h"

#include "tag_exporter.h"


void tag_exporter::export_tags(metadb_handle_list_cref items, const pfc::list_base_const_t<const file_info*>* file_infos) {
	serialized_tags_dict_t serialized_tags;

	console::timer_scope timer(COMPONENT_NAME": export: ");

	auto exported_items_count = 0;
	for (auto i = 0; i < items.get_count(); ++i) {
		auto item_metadb_handle = items[i];
		auto file_info = file_infos != NULL ? (*file_infos)[i] : &items[i]->get_info_ref()->info();

		if (!tag_extractor_service->is_exportable(file_info) || tag_extractor_service->is_empty(file_info))
			continue;

		auto key = key_provider_service->get_key(item_metadb_handle);
		auto tags = tag_extractor_service->get_tags_data(file_info);

		serialized_tags[key.get_ptr()] = json_serializer_service->serialize(key, tags);
		++exported_items_count;
	}

	fts_try([&] {
		tag_storage_service->save(serialized_tags);
		console::printf(COMPONENT_NAME": exported %d of %d", exported_items_count, items.get_count());
	});	
}
