#pragma once

#include "shared.h"
#include "key_provider.h"
#include "tag_extractor.h"
#include "tag_storage.h"
#include "json_serializer.h"


class tag_fetcher : public service_base {
public:
	enum status {
		actual = 1,
		not_actual,
		not_exported,
		exported
	};

	tag_fetcher(service_ptr_t<key_provider> key_provider,
	            service_ptr_t<tag_extractor> tag_extractor,
	            service_ptr_t<tag_storage> tag_storage,
	            service_ptr_t<json_serializer> json_serializer) : m_fetched_tags(new serialized_tags_dict_t()),
	                                                              m_exported_keys(new std::set<std::string>()),
	                                                              key_provider_service(key_provider),
	                                                              tag_extractor_service(tag_extractor),
	                                                              tag_storage_service(tag_storage),
	                                                              json_serializer_service(json_serializer) {
	}

	void fetch();
	bool is_fetched(pfc::string8 key);
	std::string get_data(pfc::string8 key);
	bool get_status(metadb_handle* handle, tag_fetcher::status& result) const;
	void notify_export(const serialized_tags_dict_t& serialized_tags_dict);	

private:
	std::shared_ptr<serialized_tags_dict_t> m_fetched_tags;
	std::shared_ptr<std::set<std::string>> m_exported_keys;

	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;
	service_ptr_t<tag_storage> tag_storage_service;
	service_ptr_t<json_serializer> json_serializer_service;
};
