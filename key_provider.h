#pragma once

class key_provider : public service_base {	
	pfc::string8 m_key_format;
	titleformat_object::ptr m_key_format_script_ptr;
	titleformat_object::ptr get_key_format_script();

public:	
	pfc::string8 get_key(metadb_handle_ptr item);
};
