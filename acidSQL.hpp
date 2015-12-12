#include "sqlite3.h"
#include <exception>
#include <string>
#include <vector>
#include <utility>

namespace acidSQL {

    class adicSQLexception: public std::exception { //out cool exception
    
        std::string whatHappened;    
        
        public:
        
            virtual const char* what() const throw() {
                return whatHappened.c_str();
            }
            adicSQLexception(const char* str):whatHappened(str){}
            adicSQLexception(std::string str):whatHappened(str){}
    };
        
    class SQLite_DataStorage{ //out data is stored here
        public:
            std::vector<std::pair<std::string, std::string>> tableData;
            SQLite_DataStorage clear(){
                tableData.clear();
                return *this;
            }
    }dataStorage;
    
    class SQLite_Speaker{ // callback class
        
        public:
            static int callback(void *data, int argc, char **argv, char **azColName){
    
               for(int i=0; i<argc; i++){
                  dataStorage.tableData.emplace_back(azColName[i], argv[i] ? argv[i] : "NULL");
               }
               return 0;
            }
    };
    
    class SQLite{
        
        private:
            sqlite3 *db;
            char *zErrMsg = 0;
            int rc;
        
        public:
    
            SQLite openConnection(const char* dbname){
                
                if(sqlite3_open(dbname, &this->db)) throw adicSQLexception("Can't open database.");
                
                return *this;
            }
            
            SQLite closeConnection(){
                sqlite3_close(db);
                return *this;
            }
            
            std::vector<std::pair<std::string, std::string>> fetch(const char* request){
                if(
                    sqlite3_exec(db, request, SQLite_Speaker::callback, NULL, &this->zErrMsg)
                    != SQLITE_OK
                ) throw adicSQLexception("Something goes terribly wrong, actually: " + std::string(zErrMsg));
                
                auto returnable = dataStorage.tableData;
                dataStorage.clear();
                
                return returnable;
            }
            
            bool execute(const char* request) {
                 if(
                    sqlite3_exec(db, request, SQLite_Speaker::callback, NULL, &this->zErrMsg)
                    != SQLITE_OK
                ) { throw adicSQLexception("Something goes terribly wrong, actually: " + std::string(zErrMsg)); return false; }
                
                return true;
            }
            
    };

}
