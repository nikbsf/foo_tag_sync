#include "stdafx.h"

#include "service_container.h"
#include "leveldb_tag_storage.h"
#include "lmdb_tag_storage.h"

service_ptr_t<service_container> g_service_container(new service_impl_single_t<service_container>());

service_ptr_t<tag_fetcher> service_container::get_tag_fetcher() {
	if (tag_fetcher_service.is_empty())
		tag_fetcher_service.attach(new service_impl_single_t<tag_fetcher>(get_key_provider(),
		                                                                  get_tag_extractor(),
		                                                                  get_tag_storage(),
		                                                                  get_json_serializer()));
	return tag_fetcher_service;
}

service_ptr_t<tag_exporter> service_container::get_tag_exporter() {
	if (tag_exporter_service.is_empty())
		tag_exporter_service.attach(new service_impl_single_t<tag_exporter>(get_tag_fetcher(),
		                                                                    get_key_provider(),
		                                                                    get_tag_extractor(),
		                                                                    get_tag_storage(),
		                                                                    get_json_serializer()));
	return tag_exporter_service;
}

service_ptr_t<tag_importer> service_container::get_tag_importer() {
	if (tag_importer_service.is_empty())
		tag_importer_service.attach(new service_impl_single_t<tag_importer>(get_tag_fetcher(),
		                                                                    get_key_provider(),
		                                                                    get_tag_extractor(),
		                                                                    get_json_serializer()));
	return tag_importer_service;
}

service_ptr_t<key_provider> service_container::get_key_provider() {
	if (key_provider_service.is_empty())
		key_provider_service.attach(new service_impl_t<key_provider>());
	return key_provider_service;
}

service_ptr_t<tag_extractor> service_container::get_tag_extractor() {
	if (tag_extractor_service.is_empty())
		tag_extractor_service.attach(new service_impl_t<tag_extractor>());
	return tag_extractor_service;
}

service_ptr_t<json_serializer> service_container::get_json_serializer() {
	if (json_serializer_service.is_empty())
		json_serializer_service.attach(new service_impl_t<json_serializer>());
	return json_serializer_service;
}

service_ptr_t<tag_storage> service_container::get_tag_storage() {
	if (tag_storage_service.is_empty())
		tag_storage_service.attach(new service_impl_t<leveldb_tag_storage>());
	return tag_storage_service;
}
