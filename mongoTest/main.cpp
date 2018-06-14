#include "bson_doc.h"
#include "bson_array.h"
#include "mongocpp.h"
#include "mongo_pool.h"
#include <bcon.h>
#include <iostream>


int main()
{
    std::string uri("mongodb://192.168.153.100:27017");
    mongoCpp::mongo_init init;

    mongoCpp::uri server(uri);
    mongoCpp::mongo_client_pool pool(server);
    mongoCpp::mongo_client_pool::pool_client_entry cli = pool.get_client();

    return 0;
}