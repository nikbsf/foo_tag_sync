#pragma once

#include "shared.h"
#include "tag_storage.h"
#include "leveldb_tag_storage.h"
#include "key_provider.h"
#include "tag_extractor.h"
#include "json_serializer.h"

typedef leveldb_tag_storage t_tag_storage;

class tag_fetcher : public service_base {
	std::shared_ptr<serialized_tags_dict_t> m_fetched_tags;
	service_ptr_t<tag_storage> tag_storage_service;
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;
	service_ptr_t<json_serializer> json_serializer_service;

public:
	enum status {
		actual = 1,
		not_actual,
		not_exported
	};

	tag_fetcher() : m_fetched_tags(new serialized_tags_dict_t()),
	                key_provider_service(new service_impl_t<key_provider>()),
	                tag_extractor_service(new service_impl_t<tag_extractor>()),
	                tag_storage_service(new service_impl_t<t_tag_storage>()),
	                json_serializer_service(new service_impl_t<json_serializer>()) {
	}

	void fetch();

	bool is_fetched(pfc::string8 key);

	std::string get_data(pfc::string8 key);

	bool get_status(metadb_handle* handle, tag_fetcher::status& result) const;

	void invalidate(const serialized_tags_dict_t& serialized_tags_dict);
};
