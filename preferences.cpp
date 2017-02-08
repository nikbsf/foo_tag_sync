#include "stdafx.h"

#include "shared.h"

static const GUID guid_advconfig_branch = { 0x7781149, 0xbad8, 0x4eb7,{ 0xbe, 0x3, 0x87, 0xf9, 0x52, 0x20, 0x83, 0x54 } };
static const GUID guid_cfg_storage_path = { 0xc021b1, 0x662c, 0x440a,{ 0xbd, 0x5, 0xb5, 0x21, 0x11, 0x5, 0xfe, 0x3c } };
static const GUID guid_cfg_key_format = { 0x99b5d9f8, 0xf7e6, 0x4c24,{ 0xb7, 0xc5, 0x33, 0xbd, 0xcd, 0x94, 0x5a, 0xde } };
static const GUID guid_cfg_synchronized_tags = { 0x9cc0d43c, 0x1d86, 0x414d,{ 0x96, 0x84, 0x79, 0x28, 0xe6, 0x82, 0xce, 0x7 } };
static const GUID guid_cfg_required_tags = { 0x88b516dd, 0xfc05, 0x41fe,{ 0xad, 0x21, 0xb4, 0xfb, 0xa8, 0x27, 0xf1, 0x65 } };
static const GUID guid_cfg_fetch_interval_seconds = { 0xa0e308d3, 0xfbce, 0x4ffe,{ 0x9d, 0xe1, 0x2a, 0x73, 0x27, 0xb8, 0x27, 0x14 } };
static const GUID guid_cfg_refresh_on_fetch = { 0x884d8398, 0x53e0, 0x48f2,{ 0xac, 0x4, 0xb1, 0x48, 0xc8, 0x9f, 0xbf, 0x4b } };

enum {
	order_storage_path,
	order_key_format,
	order_synchronized_tags,
	order_required_tags,
	order_fetch_interval_seconds,
	order_refresh_on_fetch
};

static advconfig_branch_factory g_advconfigBranch(COMPONENT_TITLE, guid_advconfig_branch, advconfig_branch::guid_branch_tagging, 0);
advconfig_string_factory_MT cfg_storage_path("Storage path", guid_cfg_storage_path, guid_advconfig_branch, order_storage_path, "");
advconfig_string_factory_MT cfg_key_format("Key format", guid_cfg_key_format, guid_advconfig_branch, order_key_format, "");
advconfig_string_factory_MT cfg_synchronized_tags("Synchronized tags", guid_cfg_synchronized_tags, guid_advconfig_branch, order_synchronized_tags, "");
advconfig_string_factory_MT cfg_required_tags("Required tags", guid_cfg_required_tags, guid_advconfig_branch, order_required_tags, "");
advconfig_integer_factory cfg_fetch_interval_seconds("Fetch interval (seconds)", guid_cfg_fetch_interval_seconds, guid_advconfig_branch, order_fetch_interval_seconds, 60, 1, UINT64_MAX);
advconfig_checkbox_factory cfg_refresh_on_fetch("Refresh on fetch", guid_cfg_refresh_on_fetch, guid_advconfig_branch, order_refresh_on_fetch, false);
