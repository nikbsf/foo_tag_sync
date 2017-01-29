#include "stdafx.h"

#include "key_provider.h"
#include "tag_extractor.h"

class metadb_io_edit_callback_handler : public metadb_io_edit_callback {
	service_ptr_t<service_impl_t<key_provider>> key_provider_service;
	service_ptr_t<service_impl_t<tag_extractor>> tag_extractor_service;

public:
	metadb_io_edit_callback_handler() : key_provider_service(new service_impl_t<key_provider>()),
	                                    tag_extractor_service(new service_impl_t<tag_extractor>()) {
	}

	void on_edited(metadb_handle_list_cref items, t_infosref before, t_infosref after) override {
		for (auto i = 0; i < after.get_count(); ++i)
			process_item(items[i], after[i]);
	}

	void process_item(metadb_handle_ptr item, const file_info* info) const {
		console::formatter() << "edit: " << key_provider_service->get_key(item);

		auto tags = tag_extractor_service->get_tags_with_values(info);
		for (auto it = tags.begin(); it != tags.end(); ++it)
			console::formatter() << "name: " << it->first << ", value: " << it->second;
	}
};

static service_factory_single_t<metadb_io_edit_callback_handler> g_edit_handler_factory;
