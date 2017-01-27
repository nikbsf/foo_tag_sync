#include "stdafx.h"

class myitem : public contextmenu_item_simple {
public:
	enum {
		cmd_test1 = 0,
		cmd_test2,
		cmd_total
	};
	unsigned get_num_items() {return cmd_total;}
	void get_item_name(unsigned p_index,pfc::string_base & p_out) {
		switch(p_index) {
			case cmd_test1: p_out = ""; break;
			case cmd_test2: p_out = ""; break;
			default: uBugCheck();
		}
	}
	void context_command(unsigned p_index,metadb_handle_list_cref p_data,const GUID& p_caller) {
		switch(p_index) {
			case cmd_test1:
				break;
			case cmd_test2:
				break;
			default:
				uBugCheck();
		}
	}
	GUID get_item_guid(unsigned p_index) {
		// {FE396C59-C523-458D-9974-3E0E3C258E1B}
		static const GUID guid_test1 = { 0xfe396c59, 0xc523, 0x458d,{ 0x99, 0x74, 0x3e, 0xe, 0x3c, 0x25, 0x8e, 0x1b } };
		// {1AA514B0-3A1F-4B73-91BD-2DCAD8E3C0C7}
		static const GUID guid_test2 = { 0x1aa514b0, 0x3a1f, 0x4b73,{ 0x91, 0xbd, 0x2d, 0xca, 0xd8, 0xe3, 0xc0, 0xc7 } };

		switch(p_index) {
			case cmd_test1: return guid_test1;
			case cmd_test2: return guid_test2;
			default: uBugCheck();
		}

	}
	bool get_item_description(unsigned p_index,pfc::string_base & p_out) {
		switch(p_index) {
			case cmd_test1:
				p_out = "";
				return true;
			case cmd_test2:
				p_out = "";
				return true;
			default:
				uBugCheck();
		}
	}
};

static contextmenu_item_factory_t<myitem> g_myitem_factory;

