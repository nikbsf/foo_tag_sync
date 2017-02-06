#include "stdafx.h"

#include "tag_importer.h"


void tag_importer::fetch() {
	console::timer_scope timer(COMPONENT_NAME": fetch: ");

	fts_try([&] {
			std::shared_ptr<serialized_tags_dict_t> fetched_tags(tag_storage_service->fetch(*m_fetched_tags));

			for (auto it = fetched_tags->begin(); it != fetched_tags->end(); ++it)
				(*m_fetched_tags)[it->first] = it->second;

			// This is a hack, refreshing all items instead of using metadb_io_callback breaks the API contract.
			// It's still quite fast though, and the fetch operation is not designed to be performed too often.
			if (m_fetched_tags->size()) {
				console::timer_scope timer(COMPONENT_NAME": refresh: ");

				pfc::list_t<metadb_handle_ptr> handles;
				static_api_ptr_t<playlist_manager>()->get_all_items(handles);
				static_api_ptr_t<metadb_io>()->dispatch_refresh(handles);
			}

			console::printf(COMPONENT_NAME": items count: %d", m_fetched_tags->size());
		});
}

void tag_importer::import_tags(metadb_handle_list_cref items) {
	pfc::list_t<file_info_impl> file_infos;
	file_infos.set_size(items.get_count());

	for (auto i = 0; i < items.get_count(); ++i) {
		auto handle = items[i];
		auto& info = file_infos[i];
		if (!handle->get_info(info))
			continue;

		auto key = key_provider_service->get_key(handle);
		if (m_fetched_tags->find(key.c_str()) == m_fetched_tags->end()) {
			console::printf(COMPONENT_NAME": no data for key: %s", key.c_str());
			continue;
		}
		else {
			auto serialized_tags_data = (*m_fetched_tags)[key.get_ptr()];
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

tag_importer::status tag_importer::get_status(pfc::string8 key, const file_info* info) const {
	if (m_fetched_tags->size() == 0)
		return status::actual;

	if (m_fetched_tags->find(key.get_ptr()) == m_fetched_tags->end())
		return status::not_exported;

	auto tags_data = tag_extractor_service->get_tags_data(info);
	auto serialized_data = json_serializer_service->serialize(key, tags_data);
	return serialized_data == (*m_fetched_tags)[key.get_ptr()] ? status::actual : status::not_actual;
}

void tag_importer::invalidate(const serialized_tags_dict_t& serialized_tags_dict) {
	for (auto it = serialized_tags_dict.begin(); it != serialized_tags_dict.end(); ++it)
		m_fetched_tags->erase(it->first);
}

service_ptr_t<tag_importer> g_tag_importer(new service_impl_t<tag_importer>());
