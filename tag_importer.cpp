#include "stdafx.h"

#include "tag_importer.h"
#include "tag_fetcher.h"

extern service_ptr_t<tag_fetcher> g_tag_fetcher;

void tag_importer::import_tags(metadb_handle_list_cref items) {
	pfc::list_t<file_info_impl> file_infos;
	file_infos.set_size(items.get_count());

	for (auto i = 0; i < items.get_count(); ++i) {
		auto handle = items[i];
		auto& info = file_infos[i];
		if (!handle->get_info(info))
			continue;

		auto key = key_provider_service->get_key(handle);
		if (g_tag_fetcher->is_fetched(key)) {
			console::printf(COMPONENT_NAME": no data for key: %s", key.c_str());
			continue;
		}
		else {
			auto serialized_tags_data = g_tag_fetcher->get_data(key);
			auto tags_data = json_serializer_service->deserialize(serialized_tags_data);
			tag_extractor_service->set_tags_data(&info, tags_data);
		}
	}

	static_api_ptr_t<metadb_io_v2>()->update_info_async_simple(items,
	                                                           pfc::ptr_list_const_array_t<const file_info, file_info_impl *>(file_infos.get_ptr(), file_infos.get_count()),
	                                                           core_api::get_main_window(),
	                                                           NULL,
	                                                           NULL);
}
