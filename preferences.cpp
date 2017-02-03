#include "stdafx.h"

#include "shared.h"

static const GUID guid_advconfig_branch = { 0x7781149, 0xbad8, 0x4eb7,{ 0xbe, 0x3, 0x87, 0xf9, 0x52, 0x20, 0x83, 0x54 } };
static const GUID guid_cfg_key_format = { 0x99b5d9f8, 0xf7e6, 0x4c24,{ 0xb7, 0xc5, 0x33, 0xbd, 0xcd, 0x94, 0x5a, 0xde } };
static const GUID guid_cfg_synchronized_tags = { 0x9cc0d43c, 0x1d86, 0x414d,{ 0x96, 0x84, 0x79, 0x28, 0xe6, 0x82, 0xce, 0x7 } };
static const GUID guid_cfg_required_tags = { 0x88b516dd, 0xfc05, 0x41fe,{ 0xad, 0x21, 0xb4, 0xfb, 0xa8, 0x27, 0xf1, 0x65 } };
static const GUID guid_cfg_fetch_interval_seconds = { 0xa0e308d3, 0xfbce, 0x4ffe,{ 0x9d, 0xe1, 0x2a, 0x73, 0x27, 0xb8, 0x27, 0x14 } };


static advconfig_branch_factory g_advconfigBranch(COMPONENT_TITLE, guid_advconfig_branch, advconfig_branch::guid_branch_tagging, 0);
advconfig_string_factory_MT cfg_key_format("Key format", guid_cfg_key_format, guid_advconfig_branch, 0, "");
advconfig_string_factory_MT cfg_synchronized_tags("Synchronized tags", guid_cfg_synchronized_tags, guid_advconfig_branch, 0, "");
advconfig_string_factory_MT cfg_required_tags("Required tags", guid_cfg_required_tags, guid_advconfig_branch, 0, "");
advconfig_integer_factory cfg_fetch_interval_seconds("Fetch interval (seconds)", guid_cfg_fetch_interval_seconds, guid_advconfig_branch, 0, 60, 1, UINT64_MAX);
