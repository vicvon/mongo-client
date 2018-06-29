#include "bson_doc.h"
#include "bson_array.h"
#include "mongocpp.h"
#include "mongo_pool.h"
#include <iostream>
#include <sstream>
#include <bson.h>

void process(mongoCpp::mongo_client_pool & pool)
{
    bsonCpp::doc_value match = bsonCpp::make_document(
        bsonCpp::kvp("$match", bsonCpp::make_document(bsonCpp::kvp("uid", "test"))));
    bsonCpp::doc_value unwind = bsonCpp::make_document(bsonCpp::kvp("$unwind", "$res"));
    bsonCpp::doc_value project = bsonCpp::make_document(
        bsonCpp::kvp("$project", bsonCpp::make_document(
            bsonCpp::kvp("length", bsonCpp::make_document(bsonCpp::kvp("$strLenCP", "$res"))), 
            bsonCpp::kvp("uid", 1), bsonCpp::kvp("version", 1))));
    bsonCpp::doc_value group = bsonCpp::make_document(
        bsonCpp::kvp("$group", bsonCpp::make_document(
            bsonCpp::kvp("_id", bsonCpp::make_document(
                bsonCpp::kvp("uid", "$uid"), bsonCpp::kvp("version", "$version"))),
            bsonCpp::kvp("totalsize", bsonCpp::make_document(bsonCpp::kvp("$sum", "$length"))),
            bsonCpp::kvp("arraysize", bsonCpp::make_document(bsonCpp::kvp("$sum", 1))))));
    
    bsonCpp::doc_value pipeline = bsonCpp::make_document(bsonCpp::kvp("pipeline",
        bsonCpp::make_array(match, unwind, project, group)));
    bsonCpp::doc_value opts;

    {
        mongoCpp::mongo_client_pool::pool_client_entry cli = pool.get_client();
        mongoCpp::collection coll = cli.get().get_database("db_name").get_collection("coll_name");

        std::vector<std::string> resource;
        for (int i = 0; i < 10; ++i)
        {
            std::stringstream str;

        }

        bsonCpp::arr_value arr = bsonCpp::make_array(resource);
        bsonCpp::doc_value insert = bsonCpp::make_document(bsonCpp::kvp("$addToSet",
            bsonCpp::make_document(bsonCpp::kvp("res",
                bsonCpp::make_document(bsonCpp::kvp("$each", arr))))));

        mongoCpp::cursor curs = coll.aggregate(pipeline, opts);
        try
        {
            int32_t version = 0;
            if (curs.next())
            {
                bsonCpp::view result = curs.getResult();
                version = result.get_value_as_doc("_id").get_value_as_int32("version");

                bsonCpp::doc_value filter = bsonCpp::make_document(
                    bsonCpp::kvp("uid", "test"), bsonCpp::kvp("version", version));
                bsonCpp::doc_value update = bsonCpp::make_document(insert,
                    bsonCpp::kvp("$set", bsonCpp::make_document(bsonCpp::kvp("version", ++version))));
            }
            else
            {
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}

int main()
{
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
                bsonCpp::make_document(bsonCpp::kvp("$each", bsonCpp::make_array("eee", "fff")))))),
            bsonCpp::kvp("$set", bsonCpp::make_document(bsonCpp::kvp("version", 1))));
        bsonCpp::doc_value opts = bsonCpp::make_document(bsonCpp::kvp("upsert", true));

        boost::optional<bsonCpp::doc_value> result = coll.update_one(filter, updater, opts);
    }

    {
        process(pool);
    }

#if 0
    {
        bsonCpp::doc_value filters = bsonCpp::make_document(bsonCpp::kvp("uid", "npc"));
        bsonCpp::doc_value opts;
        mongoCpp::cursor curs = coll.find(filters, opts);
        try
        {
            while (curs.next())
            {
                bsonCpp::view doc = curs.getResult();
                bsonCpp::view arr = doc.get_value_as_array("res");

                for (uint32_t i = 0; i < arr.count_keys(); ++i)
                {
                    std::stringstream str;
                    str << i;
                    std::cout << arr.get_value_as_utf8(str.str()) << std::endl;
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
#endif
    return 0;
}