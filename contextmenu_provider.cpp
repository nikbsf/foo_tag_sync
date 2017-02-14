#include "stdafx.h"

#include "service_container.h"

extern service_ptr_t<service_container> g_service_container;

class contextmenu_provider : public contextmenu_item_simple {
public:
	enum {
		cmd_export = 0,
		cmd_import,
		cmd_total
	};

	unsigned get_num_items() { return cmd_total; }

	void get_item_name(unsigned p_index, pfc::string_base& p_out) {
		switch (p_index) {
			case cmd_export: p_out = "Export tags";
				break;
			case cmd_import: p_out = "Import tags";
				break;
			default: uBugCheck();
		}
	}

	void context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) {
		switch (p_index) {
			case cmd_export:
				g_service_container->get_tag_exporter()->export_tags(p_data);
				break;
			case cmd_import:
				g_service_container->get_tag_importer()->import_tags(p_data);
				break;
			default:
				uBugCheck();
		}
	}

	GUID get_item_guid(unsigned p_index) {
		static const GUID guid_export = {0xfe396c59, 0xc523, 0x458d,{0x99, 0x74, 0x3e, 0xe, 0x3c, 0x25, 0x8e, 0x1b}};
		static const GUID guid_import = {0x2a678929, 0x7269, 0x4c80,{0x81, 0x23, 0x33, 0x68, 0xac, 0xe3, 0x63, 0x44}};


		switch (p_index) {
			case cmd_export: return guid_export;
			case cmd_import: return guid_import;
			default: uBugCheck();
		}

	}

	bool get_item_description(unsigned p_index, pfc::string_base& p_out) {
		switch (p_index) {
			case cmd_export:
				p_out = "Export tags of selected items";
				return true;
			case cmd_import:
				p_out = "Import tags of selected items";
				return true;
			default:
				uBugCheck();
		}
	}
};

static contextmenu_item_factory_t<contextmenu_provider> g_contextmenu_provider_factory;
