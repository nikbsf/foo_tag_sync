#pragma once

#include "tag_fetcher.h"
#include "tag_importer.h"
#include "tag_exporter.h"
#include "key_provider.h"
#include "tag_extractor.h"
#include "json_serializer.h"
#include "tag_storage.h"

class service_container : public service_base {
public:
	service_ptr_t<tag_fetcher> get_tag_fetcher();
	service_ptr_t<tag_importer> get_tag_importer();
	service_ptr_t<tag_exporter> get_tag_exporter();
	service_ptr_t<key_provider> get_key_provider();
	service_ptr_t<tag_extractor> get_tag_extractor();
	service_ptr_t<json_serializer> get_json_serializer();
	service_ptr_t<tag_storage> get_tag_storage();

private:
	service_ptr_t<tag_fetcher> tag_fetcher_service;
	service_ptr_t<tag_importer> tag_importer_service;
	service_ptr_t<tag_exporter> tag_exporter_service;
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;
	service_ptr_t<json_serializer> json_serializer_service;
	service_ptr_t<tag_storage> tag_storage_service;
};
