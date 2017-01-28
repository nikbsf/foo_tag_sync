#include "stdafx.h"

#include "key_provider.h"

class metadb_io_edit_callback_handler : public metadb_io_edit_callback {
	service_ptr_t<service_impl_t<key_provider>> key_provider_service;

public:
	metadb_io_edit_callback_handler() : key_provider_service(new service_impl_t<key_provider>()) { }

	void on_edited(metadb_handle_list_cref items, t_infosref before, t_infosref after) override {
		for (auto i = 0; i < items.get_count(); ++i)
			process_item(items[i]);
	}

	void process_item(metadb_handle_ptr item) const {
		console::formatter() << "edit: " << key_provider_service->get_key(item);
	}
};

static service_factory_single_t<metadb_io_edit_callback_handler> g_edit_handler_factory;
