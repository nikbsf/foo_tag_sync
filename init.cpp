#include "stdafx.h"
#include "shared.h"

class init_handler : public init_stage_callback {
public:
	void on_init_stage(t_uint32 stage) {
		if (stage != init_stages::after_library_init) 
			return;

		console::print(COMPONENT_NAME ": on_init()");
	}
};

static initquit_factory_t<init_handler> g_init_handler_factory;
