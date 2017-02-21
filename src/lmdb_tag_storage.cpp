#include "stdafx.h"

#include <lmdb.h>
#include "lmdb/lmdb++.h"
#include "lmdb_tag_storage.h"

extern advconfig_string_factory_MT cfg_storage_path;

void lmdb_tag_storage::save(const serialized_tags_dict_t& serialized_tags_dict) {
	auto env = lmdb::env::create();
	env.set_mapsize(2UL * 1024UL * 1024UL);
	env.set_flags(MDB_CREATE);	

	auto storage_path = get_setting(cfg_storage_path);
	if (storage_path.is_empty())
		throw fts_exception("Storage path not set");

	try {		
		env.open(storage_path, MDB_CREATE);
	}
	catch(lmdb::runtime_error e) {
		throw fts_exception(e.what());
	}

	auto wtxn = lmdb::txn::begin(env);
	auto dbi = lmdb::dbi::open(wtxn, nullptr);

	for (auto it = serialized_tags_dict.begin(); it != serialized_tags_dict.end(); ++it) {
		auto key = it->first.c_str();
		auto serialized_tags = it->second.c_str();
		dbi.put(wtxn, key, serialized_tags);
	}		
	wtxn.commit();
}

serialized_tags_dict_t* lmdb_tag_storage::fetch(serialized_tags_dict_t& existing) {
	auto env = lmdb::env::create();

	auto storage_path = get_setting(cfg_storage_path);
	try {
		env.open(storage_path, MDB_CREATE);
	}
	catch (lmdb::runtime_error e) {
		throw fts_exception(e.what());
	}

	auto rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
	auto dbi = lmdb::dbi::open(rtxn, nullptr);
	auto cursor = lmdb::cursor::open(rtxn, dbi);

	auto result = new serialized_tags_dict_t();

	std::string key, value;
	while (cursor.get(key, value, MDB_NEXT)) {
		if (existing[key] != value)
			(*result)[key] = value;		
	}
	cursor.close();
	rtxn.abort();

	return result;
}
