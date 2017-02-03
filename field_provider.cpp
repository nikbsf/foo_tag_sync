#include "stdafx.h"

#include "tag_importer.h"

#define FIELD_NAME "foo_tag_sync_status";

extern service_ptr_t<tag_importer> g_tag_importer;

class field_provider : public metadb_display_field_provider {
	service_ptr_t<key_provider> key_provider_service;
	service_ptr_t<tag_extractor> tag_extractor_service;

	bool try_get_actual(metadb_handle* handle, tag_importer::status& result) {
		auto key = key_provider_service->get_key(handle);
		auto file_info = &handle->get_info_ref()->info();
		if (!tag_extractor_service->is_exportable(file_info) || tag_extractor_service->is_empty(file_info))
			return false;

		result = g_tag_importer->get_status(key, file_info);
		return true;
	}

public:
	field_provider() : key_provider_service(new service_impl_t<key_provider>()),
	                   tag_extractor_service(new service_impl_t<tag_extractor>()) {
	}

	enum {
		field_actual = 0,
		field_total
	};

	t_uint32 get_field_count() override {
		return field_total;
	}

	void get_field_name(t_uint32 index, pfc::string_base& out) override {
		switch (index) {
			case field_actual:
				out = FIELD_NAME;
				break;
			default: uBugCheck();
		}
	}

	bool process_field(t_uint32 index, metadb_handle* handle, titleformat_text_out* out) override {
		tag_importer::status status;
		switch (index) {
			case field_actual:
				if (try_get_actual(handle, status)) {
					out->write_int(titleformat_inputtypes::meta, status);
					return true;
				}
				return false;
			default: uBugCheck();
		}
	}
};

static service_factory_single_t<field_provider> g_field_provider_factory;
