#pragma once

#pragma once

#include "json_serializer.h"
#include "key_provider.h"
#include "tag_extractor.h"


class tag_importer : public service_base {
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;
	service_ptr_t<json_serializer> json_serializer_service;

public:
	tag_importer() : key_provider_service(new service_impl_t<key_provider>()),
	                 tag_extractor_service(new service_impl_t<tag_extractor>()),
	                 json_serializer_service(new service_impl_t<json_serializer>()) {
	}

	void import_tags(metadb_handle_list_cref items);
};
