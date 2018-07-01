#include "bson_doc.h"
#include "bson_array.h"
#include "mongocpp.h"
#include "mongo_pool.h"
#include <iostream>
#include <sstream>
#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

std::string random_str(boost::uniform_int<> & ui, boost::mt19937 & seed, int length)
{
    std::string result = "";
    for (int i = 0; i < length; ++i)
    {
        int randNum = ui(seed);
        int index = randNum % 2;

        char ch;
        switch (index)
        {
            case 0:
                ch = 'A' + randNum % 26;
                break;
            case 1:
                ch = 'a' + randNum % 26;
                break;
        }
        result += ch;
    }
    return result;
}

void process(mongoCpp::mongo_client_pool & pool)
{
    boost::mt19937 rng(time(NULL));
    boost::uniform_int<> ui(1, 10);

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

    bsonCpp::doc_value insert_opts = bsonCpp::make_document(bsonCpp::kvp("upsert", true));
    bsonCpp::doc_value pipeline_opts;

    int loops = 0;
    while(1)
    {
        if (loops < 1000)
        {
            ++loops;
        }
        else
        {
            break;
        }

        mongoCpp::mongo_client_pool::pool_client_entry cli = pool.get_client();
        mongoCpp::collection coll = cli.get().get_database("db_name").get_collection("coll_name");

        std::vector<std::string> resource;
        for (int i = 0; i < 10; ++i)
        {
            int len = ui(rng);
            resource.push_back(random_str(ui, rng, len));
        }

        bsonCpp::arr_value arr = bsonCpp::make_array(resource);
        bsonCpp::doc_value insert = bsonCpp::make_document(bsonCpp::kvp("$addToSet",
            bsonCpp::make_document(bsonCpp::kvp("res",
                bsonCpp::make_document(bsonCpp::kvp("$each", arr))))));

        mongoCpp::cursor curs = coll.aggregate(pipeline, pipeline_opts);
        try
        {
            int32_t version = 0;
            if (curs.next())
            {
                bsonCpp::view result = curs.getResult();
                version = result.get_value_as_doc("_id").get_value_as_int32("version");
                int32_t totalsize = result.get_value_as_int32("totalsize");
                int32_t arraysize = result.get_value_as_int32("arraysize");

                if (totalsize < 50000 && arraysize < 1000)
                {
                    bsonCpp::doc_value filter = bsonCpp::make_document(
                        bsonCpp::kvp("uid", "test"), bsonCpp::kvp("version", version));
                    bsonCpp::doc_value update = bsonCpp::make_document(insert,
                        bsonCpp::kvp("$set", bsonCpp::make_document(bsonCpp::kvp("version", ++version))));

                    coll.update_one(filter, update, insert_opts);
                }
            }
            else
            {
                bsonCpp::doc_value filter = bsonCpp::make_document(bsonCpp::kvp("uid", "test"));
                bsonCpp::doc_value update = bsonCpp::make_document(insert,
                    bsonCpp::kvp("$set", bsonCpp::make_document(bsonCpp::kvp("version", 1))));

                coll.update_one(filter, update, insert_opts);
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
    std::string uri("mongodb://192.168.122.130:27017");
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
        std::vector<boost::shared_ptr<boost::thread> > thread_vec;
        for (int i = 0; i < 10; ++i)
        {
            thread_vec.push_back(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(process, pool))));
        }

        for (int i = 0; i < 10; ++i)
        {
            thread_vec[i]->join();
        }

        std::cout << "process over all threads" << std::endl;
    }

    return 0;
}