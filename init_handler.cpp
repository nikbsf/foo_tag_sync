#include "stdafx.h"

#include "tag_importer.h"

extern service_ptr_t<tag_importer> g_tag_importer;
extern advconfig_integer_factory cfg_fetch_interval_seconds;

class updates_check_timer : public system_time_callback_impl {
public:
	void on_time_changed(t_filetimestamp newVal) override {
		g_tag_importer->fetch();
	}
};

static updates_check_timer g_updates_check_timer;

class init_handler : public initquit {
public:
	void on_init() override {
		auto fetch_interval  = cfg_fetch_interval_seconds.get();
		g_updates_check_timer.initialize_timer(fetch_interval * system_time_periods::second);
	}

	void on_quit() override {
		g_updates_check_timer.stop_timer();
	}
};

static initquit_factory_t<init_handler> g_init_handler_factory;
