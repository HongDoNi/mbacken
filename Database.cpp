#include "Database.h"
using namespace b2be::utils;

//************
//* Database *
//************

/**
 * Constructor
 * 
 * @param dbSettings The database settings object
 * @param database The name of the database you initially wish to 'use'
 */ 
Database::Database(DBSettings* dbSettings, const string &database)
{
	_Host = dbSettings->GetHost();
	_Username = dbSettings->GetUsername();
	_Password = dbSettings->GetPassword();
	_Port = dbSettings->GetPort();
	_IsReadOnly = dbSettings->IsReadOnly();
	_Database = database;
	
	_Connection = NULL;
	_Transaction = NULL;
	_QuoteEscapeQuery = NULL;

	_IsError = false;
	_ErrorMessage = "";

	srand(time(NULL));
}

/**
 * Constructor
 *
 * @param host The name or IP of the database server
 * @param username The username to use to login to the database server
 * @param password The password for the username to use to login to the database server
 * @param port The port to use to connect to the database server
 * @param database The name of the database you initially wish to 'use'
 * @param isReadOnly Whether the connection to the database server should be read-only or not
 */
Database::Database(const string &host, const string &username, const string &password, int port, const string &database, bool isReadOnly)
{
	_Host = host;
	_Username = username;
	_Password = password;
	_Port = port;
	_Database = database;
	_IsReadOnly = isReadOnly;
	
	_Connection = NULL;
	_Transaction = NULL;
	_QuoteEscapeQuery = NULL;

	_IsError = false;
	_ErrorMessage = "";
}

/**
 * Destructor
 */
Database::~Database()
{
	if(_Connection != NULL) {
		std::cout << "del _Connection" << std::endl;
		delete _Connection;
	}
	
	if(_Transaction != NULL) {
		std::cout << "del _Transaction" << std::endl;
		delete _Transaction;	
	}
	
	if(_QuoteEscapeQuery != NULL) {
		std::cout << "del _QuoteEscapeQuery" << std::endl;
		delete _QuoteEscapeQuery;
	} 
}

/**
 * Attempts to connect to the database server
 */
void Database::Connect()
{
	delete _Connection;
	_Connection = NULL;
	
	_Connection = new mysqlpp::Connection(false);

	unsigned int totalWaitTime = 0;
	unsigned int waitTime = 0;
	for(unsigned int i = 0; (i < 10) && (totalWaitTime < _CONNECTION_TIMEOUT_); i++)
	{
		if(_Connection->connect(_Database.c_str(), _Host.c_str(), _Username.c_str(), _Password.c_str(), _Port))
		{
			return;	// connection success;
		}
		else
		{
			waitTime = rand()%9 + 2;	//wait for 2 - 10 sec before retry
			totalWaitTime += waitTime;
			if(totalWaitTime > _CONNECTION_TIMEOUT_)
			{
				waitTime -= (totalWaitTime - _CONNECTION_TIMEOUT_);
			}

			sleep(waitTime);
		}
	}

	_IsError = true;
	_ErrorMessage = "Failed to connect with db in " + String::ToString(_CONNECTION_TIMEOUT_) + " sec (";
	_ErrorMessage.append(_Connection->error());
	_ErrorMessage.append(")");

	return;
}

/**
 * Disconnects from the database server
 */
void Database::Disconnect()
{
	if(_Connection != NULL && _Connection->connected())
		_Connection->disconnect();
}

/**
 * Indicates whether we have connected to the database server
 * @return A bool indicating if we have connected to the database
 */
bool Database::IsConnected()
{
	if(_Connection != NULL)
		return _Connection->connected();
	return false;
}

/**
 * Indicates whether we can successfully ping the database server
 * @return A bool indicating if we can ping the database
 */
bool Database::Ping()
{
	if(_Connection != NULL)
		return _Connection->ping();
	return false;
}

/**
 * Returns the host database server
 * @return A string containing the database server host
 */
string Database::GetHost() const
{
	return _Host;
}

/**
 * Creates a new query object and returns it
 *
 * @param sqlStatement The SQL statement for the query
 * @return A new query object (can be NULL)
 */
DBQuery* Database::CreateQuery(const string &sqlStatement)
{
	if(_Connection == NULL)
	{
		_IsError = true;
		_ErrorMessage = "Failed to create query (not connected to a database server)";
		return NULL;
	}
	
	DBQuery* query = new DBQuery(sqlStatement, _Connection);
	return query;
}

/**
 * Creates a new non query object and returns it
 *
 * @param sqlStatement The SQL statement for the non query
 * @return A new non query object (can be NULL)
 */
DBNonQuery* Database::CreateNonQuery(const string &sqlStatement)
{
	if(_Connection == NULL)
	{
		_IsError = true;
		_ErrorMessage = "Failed to create query (not connected to a database server)";
		return NULL;
	}
	else if(_IsReadOnly)
	{
		_IsError = true;
		_ErrorMessage = "Failed to create query (connection is read-only)";
		return NULL;
	}
	
	DBNonQuery* query = new DBNonQuery(sqlStatement, _Connection);
	return query;
}

/**
 * Begins a new database transaction
 */
void Database::BeginTransaction()
{
	delete _Transaction;
		
	_Transaction = new mysqlpp::Transaction(*_Connection);
}

/**
 * Rolls back the current database transaction
 */
void Database::RollbackTransaction()
{
	if(_Transaction == NULL)
	{
		_IsError = true;
		_ErrorMessage = "Failed to rollback transaction (no transaction currently active)";
		return;
	}

	_Transaction->rollback();	
}

/**
 * Commits the current database transaction
 */
void Database::CommitTransaction()
{
	if(_Transaction == NULL)
	{
		_IsError = true;
		_ErrorMessage = "Failed to commit transaction (no transaction currently active)";
		return;
	}

	_Transaction->commit();	
}

/**
 * Check if an error has occurred or not
 * 
 * @return Return if error has occurred, otherwise false
 */
bool Database::IsError() const
{
	return _IsError;
}

/**
 * Get the message related to last error which occurred
 * 
 * @return Error message related to last error
 */
string Database::GetErrorMessage() const
{
	return _ErrorMessage;
}

/**
 * Resets the IsError and ErrorMessage details
 */
void Database::ResetError()
{
	_IsError = false;
	_ErrorMessage = "";
}

/**
 * Quotes and escapes the required characters in the provided string
 * 
 * @param aString String to quote and escape
 * @return Quoted and escaped string
 */ 
string Database::QuoteEscapeString(const string &aString)
{
	if(_Connection == NULL)
	{
		_IsError = true;
		_ErrorMessage = "Failed to quote & escape string (not connected to a database server)";
		return "";
	}

	if(_QuoteEscapeQuery == NULL)
		_QuoteEscapeQuery = new mysqlpp::Query(_Connection, false);
	
	_QuoteEscapeQuery->reset();
	(*_QuoteEscapeQuery) << mysqlpp::quote << aString;
	
	return _QuoteEscapeQuery->str();
}

//**************
//* DBSettings *
//**************

/**
 * Constructor
 *
 * @param host The name or IP of the database server
 * @param username The username to use to login to the database server
 * @param password The password for the username to use to login to the database server
 * @param port The port to use to connect to the database server
 * @param isReadOnly Whether the database server is read-only or not
 */ 

DBSettings::DBSettings() { } // zy add

DBSettings::DBSettings(const string &host, const string &username, const string &password, int port, bool isReadOnly)
{
	_Host = host;
	_Username = username;
	_Password = password;
	_Port = port;
	_IsReadOnly = isReadOnly;
}

/**
 * Destructor
 */
DBSettings::~DBSettings()
{ }

/**
 * Get the database server host name or IP
 *
 * @return The database server host name or IP
 */
string DBSettings::GetHost() const
{
	return _Host;
}

/**
 * Get the username to use to login
 * to the database server
 *
 * @return The username
 */
string DBSettings::GetUsername() const
{
	return _Username;
}

/**
 * Get the password for the username 
 * to use to login to the database server
 *
 * @return The password
 */
string DBSettings::GetPassword() const
{
	return _Password;
}

/**
 * Get the port to use to connect to
 * the database server
 *
 * @return The port
 */
int DBSettings::GetPort() const
{
	return _Port;
}

/**
 * Get whether the database server
 * is read-only or not
 *
 * @return True if the database server is read-only, otherwise false
 */
bool DBSettings::IsReadOnly() const
{
	return _IsReadOnly;
}

//***********
//* DBQuery *
//***********

/**
 * Constructor
 *
 * @param sqlStatement The SQL statement for the query
 * @param connection Database connection
 */
DBQuery::DBQuery(const string &sqlStatement, mysqlpp::Connection* connection)
{
	_SqlStatement = sqlStatement;
	_Connection = connection;
	_Query = NULL;
	
	_IsError = false;
	_ErrorMessage = "";
}

/**
 * Destructor
 */
DBQuery::~DBQuery()
{
	delete _Query;
}

/**
 * Executes the query and returns the result
 *
 * @return The result of the db query (can be NULL)
 */
DBQueryResult* DBQuery::Execute()
{
	if(!_Query) delete _Query;
		
	_Query = new mysqlpp::Query(_Connection, false, _SqlStatement.c_str());
	mysqlpp::StoreQueryResult storeQueryResult = _Query->store();
	
	if(!storeQueryResult)
	{
		_IsError = true;
		_ErrorMessage = "Failed to execute query (";
		_ErrorMessage.append(_Query->error());
		_ErrorMessage.append(")");
		return NULL;
	}

	if (storeQueryResult) {
            std::cout.setf(std::ios::left);
            std::cout <<
            std::setw(31) << "name" <<
            std::setw(10) << "age" <<
            std::endl;

            mysqlpp::StoreQueryResult::const_iterator it;

            for (it = storeQueryResult.begin(); it != storeQueryResult.end(); ++it) {
                mysqlpp::Row row = *it;
                std::cout <<
                std::setw(31) << row[0] <<
                std::setw(10) << row[1] <<
                std::endl;
            }
    }
	DBQueryResult* result = new DBQueryResult(new mysqlpp::StoreQueryResult(storeQueryResult));
	return result;
}

/**
 * Check if an error has occurred or not
 * 
 * @return Return if error has occurred, otherwise false
 */
bool DBQuery::IsError() const
{
	return _IsError;
}

/**
 * Get the message related to last error which occurred
 * 
 * @return Error message related to last error
 */
string DBQuery::GetErrorMessage() const
{
	return _ErrorMessage;
}

/**
 * Resets the IsError and ErrorMessage details
 */
void DBQuery::ResetError()
{
	_IsError = false;
	_ErrorMessage = "";
}

//**************
//* DBNonQuery *
//**************

/**
 * Constructor
 *
 * @param sqlStatement The SQL statement for the non query
 * @param connection Database connection
 */
DBNonQuery::DBNonQuery(const string &sqlStatement, mysqlpp::Connection* connection)
{
	_SqlStatement = sqlStatement;
	_Connection = connection;
	_Query = NULL;
	
	_IsError = false;
	_ErrorMessage = "";
}

/**
 * Destructor
 */
DBNonQuery::~DBNonQuery()
{
	delete _Query;
}

/**
 * Executes the non query
 */
void DBNonQuery::Execute()
{
	delete _Query;

	_Query = new mysqlpp::Query(_Connection, false, _SqlStatement.c_str());

	if(!_Query->execute())
	{
		_IsError = true;
		_ErrorMessage = "Failed to execute query (";
		_ErrorMessage.append(_Query->error());
		_ErrorMessage.append(")");
		return;
	}
}

/**
 * Get the number of rows affected by
 * execution of the non query.
 * 
 * @return The number of rows affected
 */
long DBNonQuery::GetAffectedRowCount()
{
	if(_Query == NULL)
		return -1;
		
	return _Query->affected_rows();
}

/**
 * Get ID generated for an AUTO_INCREMENT
 * column (if the non query was an INSERT)
 *
 * @return The ID generated for the last INSERT
 */
long DBNonQuery::GetLastInsertID()
{
	if(_Query == NULL)
		return -1;
		
	return _Query->insert_id();
}

/**
 * Check if an error has occurred or not
 * 
 * @return Return if error has occurred, otherwise false
 */
bool DBNonQuery::IsError() const
{
	return _IsError;
}

/**
 * Get the message related to last error which occurred
 * 
 * @return Error message related to last error
 */
string DBNonQuery::GetErrorMessage() const
{
	return _ErrorMessage;
}

/**
 * Resets the IsError and ErrorMessage details
 */
void DBNonQuery::ResetError()
{
	_IsError = false;
	_ErrorMessage = "";
}

//*****************
//* DBQueryResult *
//*****************

/**
 * Constructor
 * 
 * @param result The store query result
 */
DBQueryResult::DBQueryResult(mysqlpp::StoreQueryResult* result)
{
	// _Result = result;
	_CurrentRow = -1;
	
	_IsError = false;
	_ErrorMessage = "";
}
 
/**
 * Destructor
 */
DBQueryResult::~DBQueryResult()
{
	if(_Result != NULL) {
		std::cout << "~DBQueryResult" << std::endl;
		delete _Result;
	}
	else std::cout << "_Result is NULL" << std::endl;
}

/**
 * Get the number of rows in the result
 *
 * @return The number of rows in the result
 */
long DBQueryResult::GetRowCount()
{
	return _Result->num_rows();
}

/**
 * Get whether or not any more rows
 * remain to be gotten (via NextRow())
 *
 * @return True if there are more rows to be gotten, otherwise false
 */
bool DBQueryResult::IsMoreRows()
{
	if(_CurrentRow == (GetRowCount() - 1))
		return false;
	else 
		return true;
}

/**
 * Moves the internal pointer to 
 * the next row in the result
 */
void DBQueryResult::NextRow()
{
	if(!IsMoreRows())
	{
		_IsError = true;
		_ErrorMessage = "Failed to move to next row (there are no more rows remaining)";
		return;
	}
	
	_CurrentRow++;
}

/**
 * Resets the internal pointer
 * back to the first row
 */
void DBQueryResult::ResetRow()
{
	_CurrentRow = -1;
}

/**
 * Gets whether the value in the column in the current row
 * is NULL or not.
 * 
 * @param index The index of the column
 * @return True if the column value is NULL, otherwise false
 */
bool DBQueryResult::IsColumnValueNull(int index)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return false;
	}

	if((*_Result)[_CurrentRow][index] == mysqlpp::null)
		return true;
	else
		return false;
}

/**
 * Gets whether the value in the column in the current row
 * is NULL or not.
 * 
 * @param columnName The name of the column
 * @return True if the column value is NULL, otherwise false
 */
bool DBQueryResult::IsColumnValueNull(const string &columnName)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return false;
	}

	if((*_Result)[_CurrentRow][columnName.c_str()] == mysqlpp::null)
		return true;
	else
		return false;
}

/**
 * Gets the value of the column in the current row as a string
 * 
 * @param index The index of the column
 * @return The value of the specified column
 */
string DBQueryResult::GetColumnValueString(int index)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return "";
	}
	
	return (string)(*_Result)[_CurrentRow][index];
}

/**
 * Gets the value of the column in the current row as a string
 * 
 * @param columnName The name of the column
 * @return The value of the specified column
 */
string DBQueryResult::GetColumnValueString(const string &columnName)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return "";
	}
	
	return (string)(*_Result)[_CurrentRow][columnName.c_str()];
}

/**
 * Gets the value of the column in the current row as an int
 * 
 * @param index The index of the column
 * @return The value of the specified column
 */
int DBQueryResult::GetColumnValueInt(int index)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return 0;
	}
	
	return (*_Result)[_CurrentRow][index];
}

/**
 * Gets the value of the column in the current row as an int
 * 
 * @param columnName The name of the column
 * @return The value of the specified column
 */
int DBQueryResult::GetColumnValueInt(const string &columnName)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return 0;
	}
	
	return (*_Result)[_CurrentRow][columnName.c_str()];
}

/**
 * Gets the value of the column in the current row as a long
 * 
 * @param index The index of the column
 * @return The value of the specified column
 */
long DBQueryResult::GetColumnValueLong(int index)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return 0;
	}
	
	return (*_Result)[_CurrentRow][index];
}

/**
 * Gets the value of the column in the current row as a long
 * 
 * @param columnName The name of the column
 * @return The value of the specified column
 */
long DBQueryResult::GetColumnValueLong(const string &columnName)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return 0;
	}
	
	return (*_Result)[_CurrentRow][columnName.c_str()];
}

/**
 * Gets the value of the column in the current row as a double
 * 
 * @param index The index of the column
 * @return The value of the specified column
 */
double DBQueryResult::GetColumnValueDouble(int index)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return 0;
	}
	
	return (*_Result)[_CurrentRow][index];
}

/**
 * Gets the value of the column in the current row as a double
 * 
 * @param columnName The name of the column
 * @return The value of the specified column
 */
double DBQueryResult::GetColumnValueDouble(const string &columnName)
{
	if(_CurrentRow == -1)
	{
		_IsError = true;
		_ErrorMessage = "Failed to get column value ('NextRow' hasn't been called)";
		return 0;
	}
	
	return (*_Result)[_CurrentRow][columnName.c_str()];
}

/**
 * Check if an error has occurred or not
 * 
 * @return Return if error has occurred, otherwise false
 */
bool DBQueryResult::IsError() const
{
	return _IsError;
}

/**
 * Get the message related to last error which occurred
 * 
 * @return Error message related to last error
 */
string DBQueryResult::GetErrorMessage() const
{
	return _ErrorMessage;
}

/**
 * Resets the IsError and ErrorMessage details
 */
void DBQueryResult::ResetError()
{
	_IsError = false;
	_ErrorMessage = "";
}
