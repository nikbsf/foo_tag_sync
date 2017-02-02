#include "stdafx.h"

#include "tag_importer.h"


void tag_importer::fetch() {
	console::timer_scope timer("fetch ");

	tag_storage_service->fetch(m_fetched_tags.get());

	console::printf("items count: %d", m_fetched_tags->size());
}

service_ptr_t<tag_importer> g_tag_importer(new service_impl_t<tag_importer>());