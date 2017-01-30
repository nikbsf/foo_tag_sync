#include "stdafx.h"

#include "tag_exporter.h"

class contextmenu_provider : public contextmenu_item_simple {
	service_ptr_t<tag_exporter> tag_exporter_service;

protected:
	contextmenu_provider() : tag_exporter_service(new service_impl_t<tag_exporter>()) {
	}

public:

	enum {
		cmd_export = 0,
		cmd_total
	};

	unsigned get_num_items() { return cmd_total; }

	void get_item_name(unsigned p_index, pfc::string_base& p_out) {
		switch (p_index) {
			case cmd_export: p_out = "Export tags";
				break;
			default: uBugCheck();
		}
	}

	void context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) {
		switch (p_index) {
			case cmd_export:
				tag_exporter_service->export_tags(p_data);
				break;
			default:
				uBugCheck();
		}
	}

	GUID get_item_guid(unsigned p_index) {
		static const GUID guid_export = {0xfe396c59, 0xc523, 0x458d,{0x99, 0x74, 0x3e, 0xe, 0x3c, 0x25, 0x8e, 0x1b}};

		switch (p_index) {
			case cmd_export: return guid_export;
			default: uBugCheck();
		}

	}

	bool get_item_description(unsigned p_index, pfc::string_base& p_out) {
		switch (p_index) {
			case cmd_export:
				p_out = "Export tags of selected items";
				return true;
			default:
				uBugCheck();
		}
	}
};

static contextmenu_item_factory_t<contextmenu_provider> g_contextmenu_provider_factory;
