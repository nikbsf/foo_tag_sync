#pragma once

#include "tag_fetcher.h"
#include "key_provider.h"
#include "tag_extractor.h"
#include "tag_storage.h"
#include "json_serializer.h"


class tag_exporter : public service_base {
	service_ptr_t<tag_fetcher> tag_fetcher_service;
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;
	service_ptr_t<tag_storage> tag_storage_service;
	service_ptr_t<json_serializer> json_serializer_service;

	bool is_empty(const tags_data_t& tags);

public:
	tag_exporter(service_ptr_t<tag_fetcher> tag_fetcher,
	             service_ptr_t<key_provider> key_provider,
	             service_ptr_t<tag_extractor> tag_extractor,
	             service_ptr_t<tag_storage> tag_storage,
	             service_ptr_t<json_serializer> json_serializer) : tag_fetcher_service(tag_fetcher),
	                                                               key_provider_service(key_provider),
	                                                               tag_extractor_service(tag_extractor),
	                                                               tag_storage_service(tag_storage),
	                                                               json_serializer_service(json_serializer) {
	}

	void export_tags(metadb_handle_list_cref items, const pfc::list_base_const_t<const file_info*>* file_infos = NULL);
};
