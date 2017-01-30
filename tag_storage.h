#pragma once

#include "shared.h"

class tag_storage : public service_base {
public:
	virtual void save_tags(serialized_tags_dict_t serialized_tags_dict) = 0;
};


