#include "stdafx.h"

#include "key_provider.h"
#include "tag_extractor.h"
#include "json_serializer.h"
#include "lmdb_tag_storage.h"
#include "leveldb_tag_storage.h"

class metadb_io_edit_callback_handler : public metadb_io_edit_callback {
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;
	service_ptr_t<tag_storage> tag_storage_service;
	service_ptr_t<json_serializer> json_serializer_service;

public:
	metadb_io_edit_callback_handler() : key_provider_service(new service_impl_t<key_provider>()),
										tag_extractor_service(new service_impl_t<tag_extractor>()),
										tag_storage_service(new service_impl_t<lmdb_tag_storage>()),
										json_serializer_service(new service_impl_t<json_serializer>()) {
	}

	void on_edited(metadb_handle_list_cref items, t_infosref before, t_infosref after) override {
		std::map<pfc::string8, pfc::string8> tags_by_keys;
		for (auto i = 0; i < items.get_count(); ++i) {
			auto key = key_provider_service->get_key(items[i]);
			auto tags = tag_extractor_service->get_tags_with_values(after[i]);
			tags_by_keys[key] = json_serializer_service->serialize(key, tags);
		}
		tag_storage_service->save_tags(tags_by_keys);
	}
};

static service_factory_single_t<metadb_io_edit_callback_handler> g_edit_handler_factory;
