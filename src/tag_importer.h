#pragma once

#include "tag_fetcher.h"
#include "key_provider.h"
#include "tag_extractor.h"
#include "json_serializer.h"


class tag_importer : public service_base {
	service_ptr_t<tag_fetcher> tag_fetcher_service;
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;
	service_ptr_t<json_serializer> json_serializer_service;

public:
	tag_importer(service_ptr_t<tag_fetcher> tag_fetcher,
	             service_ptr_t<key_provider> key_provider,
	             service_ptr_t<tag_extractor> tag_extractor,
	             service_ptr_t<json_serializer> json_serializer) : tag_fetcher_service(tag_fetcher),
	                                                               key_provider_service(key_provider),
	                                                               tag_extractor_service(tag_extractor),
	                                                               json_serializer_service(json_serializer) {
	}

	void import_tags(metadb_handle_list_cref items);
};
