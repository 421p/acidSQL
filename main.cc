#include "acidSQL.hpp"
#include <iostream>

using namespace acidSQL;

int main(int argc, char* argv[]) {
   
   SQLite conn; //connector class has 4 methods
   
   /**
   
    openConnection(const char* database);
    allows you yo open connection to db, you need to pass sqlite database name or path to it
    
    fetch(const char* sql_request)
    executes a sql request, returns a vector of pairs of 2 strings,
    first string is col name, second is value
    
    execute(const char* sql_request)
    executes a sql request, returns true or false
    
    closeConnection()
    nothing to say here
    
    methods throws a instance of adicSQLexception,
    so you need to use try..catch for handling.
   
   **/
   
   try {
        conn.openConnection("orhanaiser.dblite");
        
        //conn.execute("insert into blah blah blah");
        auto data = conn.fetch("select * from Events"); //data is std::vector<std::pair<std::string, std::string>> here
        
        conn.closeConnection();
        
        for(auto row : data){ // row is std::pair<std::string, std::string> here
            std::cout << row.first << " = " << row.second << std::endl;
        }
        
   } catch (adicSQLexception &e) {
       std::cout << e.what() << std::endl;
   }
   return 0;
}
