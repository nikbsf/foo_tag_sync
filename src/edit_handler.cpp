#include "stdafx.h"

#include "service_container.h"

extern service_ptr_t<service_container> g_service_container;

class edit_handler : public metadb_io_edit_callback {
public:
	void on_edited(metadb_handle_list_cref items, t_infosref before, t_infosref after) override {
		g_service_container->get_tag_exporter()->export_tags(items, &after);
	}
};

static service_factory_single_t<edit_handler> g_edit_handler_factory;
