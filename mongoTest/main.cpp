#include "bson_doc.h"
#include "bson_array.h"
#include "mongocpp.h"
#include "mongo_pool.h"
#include <bcon.h>
#include <iostream>


int main()
{
#if 1
    std::string uri("mongodb://192.168.153.100:27017");
    mongoCpp::mongo_init init;

    mongoCpp::uri server(uri);
    mongoCpp::mongo_client_pool pool(server);
    mongoCpp::mongo_client_pool::pool_client_entry cli = pool.get_client();
    mongoCpp::collection coll = cli.get().get_database("db_name").get_collection("coll_name");

    {
        bsonCpp::doc_value keys = bsonCpp::make_document(bsonCpp::kvp("uid", 1));
        bsonCpp::doc_value opts = bsonCpp::make_document(bsonCpp::kvp("unique", true));
        if (!coll.createIndex(keys, opts))
        {
            std::cout << "createIndex failed" << std::endl;
        }
    }

    {
        bsonCpp::doc_value filter = bsonCpp::make_document(bsonCpp::kvp("uid", "npc"));
        bsonCpp::doc_value updater = bsonCpp::make_document(bsonCpp::kvp("$addToSet",
            bsonCpp::make_document(bsonCpp::kvp("res",
                bsonCpp::make_document(bsonCpp::kvp("$each", bsonCpp::make_array("abc", "def")))))));
        bsonCpp::doc_value opts = bsonCpp::make_document(bsonCpp::kvp("upsert", true));

        boost::optional<bsonCpp::doc_value> result = coll.update_one(filter, updater, opts);
    }
#else
    bsonCpp::doc_value doc1 = bsonCpp::make_document(bsonCpp::kvp("name", 111), bsonCpp::kvp("name1", "abc"));

    bson_t b;
    bson_init_static(&b, doc1.data(), doc1.size());
    std::cout << bson_as_json(&b, NULL) << std::endl;
#endif
    return 0;
}