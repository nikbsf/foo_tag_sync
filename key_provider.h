#pragma once

class key_provider : public service_base {
	titleformat_object::ptr get_key_format_script() const;

public:
	pfc::string8 get_key(metadb_handle_ptr item) const;
};
