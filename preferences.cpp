#include "stdafx.h"
#include "shared.h"

static const GUID guid_advconfig_branch = { 0x7781149, 0xbad8, 0x4eb7,{ 0xbe, 0x3, 0x87, 0xf9, 0x52, 0x20, 0x83, 0x54 } };
static const GUID guid_cfg_key_format = { 0x99b5d9f8, 0xf7e6, 0x4c24,{ 0xb7, 0xc5, 0x33, 0xbd, 0xcd, 0x94, 0x5a, 0xde } };

static advconfig_branch_factory g_advconfigBranch(COMPONENT_TITLE, guid_advconfig_branch, advconfig_branch::guid_branch_tagging, 0);
advconfig_string_factory cfg_key_format("Key format", guid_cfg_key_format, guid_advconfig_branch, 0, "");
