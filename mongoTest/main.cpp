#include "bson_doc.h"
#include "bson_array.h"
#include <bcon.h>
#include <iostream>


int main()
{
    std::string name = "name";
    bsonCpp::arr_value doc1 = bsonCpp::make_array(name, std::string("abcd"));
    bsonCpp::doc_value doc = bsonCpp::make_document(std::string("key"), doc1);
    bson_t bson;
    bson_init_static(&bson, doc.data(), doc.size());
    char * json = bson_as_json(&bson, NULL);
    std::cout << json << std::endl;
    bson_free(json);
    bson_destroy(&bson);

    return 0;
}