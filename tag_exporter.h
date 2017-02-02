#pragma once

#include "tag_storage.h"
#include "json_serializer.h"
#include "tag_extractor.h"
#include "key_provider.h"
#include "lmdb_tag_storage.h"
#include "leveldb_tag_storage.h"

typedef leveldb_tag_storage t_tag_storage;

class tag_exporter : public service_base {
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;
	service_ptr_t<tag_storage> tag_storage_service;
	service_ptr_t<json_serializer> json_serializer_service;

	bool is_empty(const tags_data_t& tags);

public:
	tag_exporter() : key_provider_service(new service_impl_t<key_provider>()),
	                 tag_extractor_service(new service_impl_t<tag_extractor>()),
	                 tag_storage_service(new service_impl_t<t_tag_storage>()),
	                 json_serializer_service(new service_impl_t<json_serializer>()) {
	}

	void export_tags(metadb_handle_list_cref items, const pfc::list_base_const_t<const file_info*>* file_infos = NULL);
};
