#include "stdafx.h"

#include "shared.h"

extern advconfig_string_factory cfg_key_format;

class init_handler : public init_stage_callback {
public:
	void on_init_stage(t_uint32 stage) override
	{
		if (stage != init_stages::after_library_init) 
			return;

		console::print(COMPONENT_NAME ": on_init()");
	}
};

static initquit_factory_t<init_handler> g_init_handler_factory;
