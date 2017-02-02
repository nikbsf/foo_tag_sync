#include "stdafx.h"

#include "lmdb++.h"
#include "lmdb_tag_storage.h"

void lmdb_tag_storage::save(const serialized_tags_dict_t& serialized_tags_dict) {
	auto env = lmdb::env::create();
	env.set_flags(MDB_CREATE);	
	try {		
		env.open("C:\\xxx", MDB_CREATE);
	}
	catch(lmdb::runtime_error e) {
		console::formatter() << "error: " << e.code() << " " << e.what();
	}

	auto wtxn = lmdb::txn::begin(env);
	auto dbi = lmdb::dbi::open(wtxn, nullptr);

	for (auto it = serialized_tags_dict.begin(); it != serialized_tags_dict.end(); ++it) {
		auto key = it->first;
		auto serialized_tags = it->second;
		dbi.put(wtxn, key, serialized_tags);
	}		
	wtxn.commit();
}
