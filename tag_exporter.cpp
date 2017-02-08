#include "stdafx.h"

#include "tag_exporter.h"
#include "tag_importer.h"
#include "tag_fetcher.h"

extern service_ptr_t<tag_fetcher> g_tag_fetcher;

void tag_exporter::export_tags(metadb_handle_list_cref items, const pfc::list_base_const_t<const file_info*>* file_infos) {
	serialized_tags_dict_t serialized_tags;

	pfc::hires_timer timer;
	timer.start();

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
		g_tag_fetcher->notify_export(serialized_tags);
		static_api_ptr_t<metadb_io>()->dispatch_refresh(items);
		console::printf(COMPONENT_NAME": exported %d of %d. time: %s", exported_items_count, items.get_count(), timer.queryString(2).c_str());
	});
}
