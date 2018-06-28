#include "bson_doc.h"
#include "bson_array.h"
#include "mongocpp.h"
#include "mongo_pool.h"
#include <bcon.h>
#include <iostream>
#include <sstream>

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
                bsonCpp::make_document(bsonCpp::kvp("$each", bsonCpp::make_array("eee", "fff")))))));
        bsonCpp::doc_value opts = bsonCpp::make_document(bsonCpp::kvp("upsert", true));

        boost::optional<bsonCpp::doc_value> result = coll.update_one(filter, updater, opts);
    }

    {
        bsonCpp::arr_value par = bsonCpp::make_array(
            bsonCpp::make_document(bsonCpp::kvp("$match", bsonCpp::make_document(bsonCpp::kvp("uid", "npc")))),
            bsonCpp::make_document(bsonCpp::kvp("$unwind", "$res")),
            bsonCpp::make_document(bsonCpp::kvp("$project", bsonCpp::make_document(bsonCpp::kvp("length", 
                bsonCpp::make_document(bsonCpp::kvp("$strLenCP", "$res"))),
                bsonCpp::make_document(bsonCpp::kvp("uid", 1))))),
            bsonCpp::make_document(bsonCpp::kvp("$group", bsonCpp::make_document(bsonCpp::kvp("_id", "$uid"),
                bsonCpp::kvp("totalsize", bsonCpp::make_document(bsonCpp::kvp("$sum", "$length"))),
                bsonCpp::kvp("arraysize", bsonCpp::make_document(bsonCpp::kvp("$sum", 1)))))));
        bsonCpp::doc_value pipeline = bsonCpp::make_document(bsonCpp::kvp("pipeline", par));
        bsonCpp::doc_value opts;

        mongoCpp::cursor curs = coll.aggregate(pipeline, opts);
        try
        {
            while (curs.next())
            {
                bsonCpp::view doc = curs.getResult();
                int32_t totalsize = doc.get_value_as_int32("totalsize");
                std::cout << "totalsize: " << totalsize << std::endl;
                std::cout << "id: " << doc.get_value_as_utf8("_id") << std::endl;
            }
        }
        catch (std::exception & e)
        {
            std::cout << e.what() << std::endl;
        }
    }

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
    return 0;
}