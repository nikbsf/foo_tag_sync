#include "stdafx.h"

#include "tag_importer.h"


void tag_importer::fetch() {
	console::timer_scope timer(COMPONENT_NAME": fetch: ");

	fts_try([&] {
		tag_storage_service->fetch(m_fetched_tags.get());
		console::printf(COMPONENT_NAME": items count: %d", m_fetched_tags->size());
	});	
}

bool tag_importer::is_actual(pfc::string8 key, const file_info* info) {
	if (m_fetched_tags->size() == 0)
		return true;

	auto tags_data = tag_extractor_service->get_tags_data(info);
	auto serialized_data = json_serializer_service->serialize(key, tags_data);
	return serialized_data == (*m_fetched_tags)[key.get_ptr()];
}

service_ptr_t<tag_importer> g_tag_importer(new service_impl_t<tag_importer>());
