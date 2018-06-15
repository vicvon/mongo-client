#include "mongo_collection.h"
#include "mongo_internal.h"
#include "bson_array.h"
#include "bson_doc.h"
#include <iostream>

namespace mongoCpp
{

struct collection::Impl
{
    Impl(mongoc_database_t *db, std::string name)
    {
        collection_ = mongoc_database_get_collection(db, name.c_str());
    }
    ~Impl()
    {
        mongoc_collection_destroy(collection_);
    }

    mongoc_collection_t * collection_;
};

collection::collection(void * db, std::string name) : impl_(new Impl(static_cast<mongoc_database_t *>(db), name))
{
}

collection::~collection()
{
}

cursor collection::aggregate(bsonCpp::doc_value & pipeline, bsonCpp::doc_value & options)
{
    bson_t pipe;
    bson_init_static(&pipe, pipeline.data(), pipeline.size());
    mongoc_cursor_t * curs;
    if (options.data() == NULL)
    {
        curs = mongoc_collection_aggregate(impl_->collection_, MONGOC_QUERY_NONE, &pipe, NULL, NULL);
    }
    else
    {
        bson_t opts;
        bson_init_static(&opts, options.data(), options.size());
        curs = mongoc_collection_aggregate(impl_->collection_, MONGOC_QUERY_NONE, &pipe, &opts, NULL);
    }
    
    return cursor(static_cast<void *>(curs));
}

bool collection::createIndex(bsonCpp::doc_value & keys, bsonCpp::doc_value & options)
{
    bson_t index_keys;
    bson_init_static(&index_keys, keys.data(), keys.size());

    char * index_name = mongoc_collection_keys_to_index_string(&index_keys);
    std::string name = index_name;
    bson_free(index_name);

    bsonCpp::arr_value indexes_opts = bsonCpp::make_array(bsonCpp::make_document(bsonCpp::kvp("key", keys),
                                                                                 bsonCpp::kvp("name", name)),
                                                          options);

    bsonCpp::doc_value cmd = bsonCpp::make_document(bsonCpp::kvp("createIndexes", mongoc_collection_get_name(impl_->collection_)), 
                                                    bsonCpp::kvp("indexes", indexes_opts));

    bson_t bson_cmd, reply;
    bson_error_t err;
    bson_init_static(&bson_cmd, cmd.data(), cmd.size());

    char * str = bson_as_json(&bson_cmd, NULL);
    std::cout << str << std::endl;
    bson_free(str);

    bool ret = mongoc_collection_write_command_with_opts(impl_->collection_, &bson_cmd, NULL, &reply, &err);

    char * reply_str = bson_as_json(&reply, NULL);
    std::cout << reply_str << std::endl;
    bson_free(reply_str);
    bson_destroy(&reply);

    return ret;
}

boost::optional<bsonCpp::doc_value> collection::update_one(bsonCpp::doc_value & selector, bsonCpp::doc_value & updater, bsonCpp::doc_value & options)
{
    bson_t filter, update, reply;
    bson_init_static(&filter, selector.data(), selector.size());
    bson_init_static(&update, updater.data(), updater.size());

    bool ret;
    bson_error_t err;
    if (options.data() == NULL)
    {
        ret = mongoc_collection_update_one(impl_->collection_, &filter, &update, NULL, &reply, &err);
    }
    else
    {
        bson_t opts;
        bson_init_static(&opts, options.data(), options.size());
        ret = mongoc_collection_update_one(impl_->collection_, &filter, &update, &opts, &reply, &err);
    }

    if (!ret)
    {
        return boost::none;
    }
    
    uint32_t buflen;
    uint8_t *buf;
    buf = bson_destroy_with_steal(&reply, true, &buflen);
    return boost::optional<bsonCpp::doc_value>(bsonCpp::doc_value(buf, buflen, bsonCpp::bson_free_deleter));
}

}