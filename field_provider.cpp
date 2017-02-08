#include "stdafx.h"

#include "tag_importer.h"
#include "tag_fetcher.h"

#define FIELD_NAME "foo_tag_sync_status";

extern service_ptr_t<tag_fetcher> g_tag_fetcher;

class field_provider : public metadb_display_field_provider {
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;

public:
	field_provider() : key_provider_service(new service_impl_t<key_provider>()),
	                   tag_extractor_service(new service_impl_t<tag_extractor>()) {
	}

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
				if (g_tag_fetcher->get_status(handle, status)) {
					out->write_int(titleformat_inputtypes::meta, status);
					return true;
				}
				return false;
			default: uBugCheck();
		}
	}
};

static service_factory_single_t<field_provider> g_field_provider_factory;
