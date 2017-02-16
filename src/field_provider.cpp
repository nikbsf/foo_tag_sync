#include "stdafx.h"

#include "service_container.h"

#define FIELD_NAME "foo_tag_sync_status";

extern service_ptr_t<service_container> g_service_container;

class field_provider : public metadb_display_field_provider {
public:
	enum {
		field_status = 0,
		field_total
	};

	t_uint32 get_field_count() override {
		return field_total;
	}

	void get_field_name(t_uint32 index, pfc::string_base& out) override {
		switch (index) {
			case field_status:
				out = FIELD_NAME;
				break;
			default: uBugCheck();
		}
	}

	bool process_field(t_uint32 index, metadb_handle* handle, titleformat_text_out* out) override {
		tag_fetcher::status status;
		switch (index) {
			case field_status:
				if (g_service_container->get_tag_fetcher()->get_status(handle, status)) {
					out->write_int(titleformat_inputtypes::meta, status);
					return true;
				}
				return false;
			default: uBugCheck();
		}
	}
};

static service_factory_single_t<field_provider> g_field_provider_factory;
