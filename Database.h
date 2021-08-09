#ifndef Database_H
#define Database_H

#include <mysql++/mysql++.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <iostream> // zy add

#include "String.h"
using namespace std;

#define _CONNECTION_TIMEOUT_	60

namespace b2be
{
	namespace utils
	{
		/**
		 * Provides functionality for getting access
		 * to the results gotten back as the result
		 * of a database query.
		 */
		class DBQueryResult
		{
			public:
				DBQueryResult(mysqlpp::StoreQueryResult* result);
				~DBQueryResult();		
				
				//Row related methods
				long GetRowCount();
				bool IsMoreRows();
				void NextRow();
				void ResetRow();
				
				//Column related methods
				bool IsColumnValueNull(int index);
				bool IsColumnValueNull(const string &columnName);
				string GetColumnValueString(int index);
				string GetColumnValueString(const string &columnName);
				int GetColumnValueInt(int index);
				int GetColumnValueInt(const string &columnName);
				long GetColumnValueLong(int index);
				long GetColumnValueLong(const string &columnName);
				double GetColumnValueDouble(int index);
				double GetColumnValueDouble(const string &columnName);
			
				//Error related methods
				bool IsError() const;
				string GetErrorMessage() const;
				void ResetError();		
			
			private:
				mysqlpp::StoreQueryResult* _Result;
				long _CurrentRow;
				
				bool _IsError;
				string _ErrorMessage;
		};

		/**
		 * Provides functionality for querying the database
		 * and then getting back the result.
		 */
		class DBQuery
		{
			public:
				DBQuery(const string &sqlStatement, mysqlpp::Connection* connection);
				~DBQuery();
				
				DBQueryResult* Execute();
				
				//Error related methods
				bool IsError() const;
				string GetErrorMessage() const;
				void ResetError();	
				
			private:
				string _SqlStatement;
				mysqlpp::Connection* _Connection;
				mysqlpp::Query* _Query;
				
				bool _IsError;
				string _ErrorMessage;
		};

		/**
		 * Provides functionality for a non query (eg. INSERT, UPDATE, etc)
		 * against the database and then checking the outcome.
		 */
		class DBNonQuery
		{
			public:
				DBNonQuery(const string &sqlStatement, mysqlpp::Connection* connection);
				~DBNonQuery();
				
				void Execute();
				long GetAffectedRowCount();
				long GetLastInsertID();		
				
				//Error related methods
				bool IsError() const;
				string GetErrorMessage() const;
				void ResetError();	
				
			private:
				string _SqlStatement;
				mysqlpp::Connection* _Connection;
				mysqlpp::Query* _Query;
				
				bool _IsError;
				string _ErrorMessage;
		};


		/** 
		 * Represents the settings for a database 
		 * server instance.
		 */
		class DBSettings
		{
			public:

				DBSettings(); // zy add
				DBSettings(const string &host, const string &username, const string &password, int port, bool isReadOnly = false);
				~DBSettings();

				string GetHost() const;
				string GetUsername() const;
				string GetPassword() const;
				int GetPort() const;
				bool IsReadOnly() const;		

			private:
				string _Host;
				string _Username;
				string _Password;
				int _Port;
				bool _IsReadOnly;
		};

		/**
		 * Provides functionality to allow programs
		 * to interact with a MySQL database server.
		 */
		class Database
		{
			public:
				
				Database(DBSettings* dbSettings, const string &database);
				Database(const string &host, const string &username, const string &password, int port, const string &database, bool isReadOnly = false);	
				~Database();
				
				//Connection methods
				void Connect();
				void Disconnect();				
				bool IsConnected();
				bool Ping();
				string GetHost() const;
				
				//Query & non-query methods
				DBQuery* CreateQuery(const string &sqlStatement);
				DBNonQuery* CreateNonQuery(const string &sqlStatement);
				
				//Transaction methods
				void BeginTransaction();
				void RollbackTransaction();
				void CommitTransaction();
				
				//Error related methods
				bool IsError() const;
				string GetErrorMessage() const;
				void ResetError();		
				
				//Other methods
				string QuoteEscapeString(const string &aString);
	
			private:
				string _Host;
				string _Username;
				string _Password;
				int _Port;
				string _Database;
				bool _IsReadOnly;
			
				mysqlpp::Connection* _Connection;
				mysqlpp::Transaction* _Transaction;
				mysqlpp::Query* _QuoteEscapeQuery;
				
				bool _IsError;
				string _ErrorMessage;
		};
	}
}

#endif
