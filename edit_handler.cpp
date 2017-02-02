#include "stdafx.h"

#include "tag_exporter.h"

class edit_handler : public metadb_io_edit_callback {
	service_ptr_t<tag_exporter> tag_exporter_service;

public:
	edit_handler() : tag_exporter_service(new service_impl_t<tag_exporter>()) {
	}

	void on_edited(metadb_handle_list_cref items, t_infosref before, t_infosref after) override {
		tag_exporter_service->export_tags(items, &after);
	}
};

static service_factory_single_t<edit_handler> g_edit_handler_factory;
