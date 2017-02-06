#include "stdafx.h"

#include "tag_fetcher.h"
#include "leveldb_tag_storage.h"

void tag_fetcher::fetch() {
	pfc::hires_timer timer, refresh_timer;
	fts_try([&] {
			timer.start();
			std::shared_ptr<serialized_tags_dict_t> fetched_tags(tag_storage_service->fetch(*m_fetched_tags));

			for (auto it = fetched_tags->begin(); it != fetched_tags->end(); ++it)
				(*m_fetched_tags)[it->first] = it->second;

			m_exported_keys->clear();

			auto fetch_time_str = timer.queryString(2);

			// This is a hack, refreshing all items instead of using metadb_io_callback breaks the API contract.
			// It's still quite fast though, and the fetch operation is not designed to be performed too often.
			if (m_fetched_tags->size()) {
				refresh_timer.start();

				pfc::list_t<metadb_handle_ptr> handles;
				static_api_ptr_t<playlist_manager>()->get_all_items(handles);
				static_api_ptr_t<metadb_io>()->dispatch_refresh(handles);
			}

			console::printf(COMPONENT_NAME": fetched: %d. cached: %d. fetch: %s. refresh: %s. total: %s",
			                              fetched_tags->size(),
			                              m_fetched_tags->size(),
			                              fetch_time_str.c_str(),
			                              refresh_timer.queryString(2).c_str(),
			                              timer.queryString(2).c_str()
			);
		});
}

bool tag_fetcher::get_status(metadb_handle* handle, tag_fetcher::status& result) const {
	auto info = &handle->get_info_ref()->info();

	if (!tag_extractor_service->is_exportable(info))
		return false;

	if (m_fetched_tags->size() == 0)
		return false;

	auto key = key_provider_service->get_key(handle);
	if (m_exported_keys->find(key.c_str()) != m_exported_keys->end()) {
		result = status::exported;
		return true;
	}

	bool is_fetched = m_fetched_tags->find(key.c_str()) != m_fetched_tags->end();
	bool is_empty = tag_extractor_service->is_empty(info);

	if (!is_fetched) {
		if (is_empty)
			return false;
		else
			result = status::not_exported;
	}
	else {
		// todo comparer
		auto tags_data = tag_extractor_service->get_tags_data(info);
		auto serialized_data = json_serializer_service->serialize(key, tags_data);
		result = serialized_data == (*m_fetched_tags)[key.get_ptr()] ? status::actual : status::not_actual;
	}
	return true;
}

bool tag_fetcher::is_fetched(pfc::string8 key) {
	return m_fetched_tags->find(key.c_str()) == m_fetched_tags->end();
}

std::string tag_fetcher::get_data(pfc::string8 key) {
	return (*m_fetched_tags)[key.c_str()];
}

void tag_fetcher::notify_export(const serialized_tags_dict_t& serialized_tags_dict) {
	for (auto it = serialized_tags_dict.begin(); it != serialized_tags_dict.end(); ++it)
		m_exported_keys->insert(it->first.c_str());
}

service_ptr_t<tag_fetcher> g_tag_fetcher(new service_impl_t<tag_fetcher>());
