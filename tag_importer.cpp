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
