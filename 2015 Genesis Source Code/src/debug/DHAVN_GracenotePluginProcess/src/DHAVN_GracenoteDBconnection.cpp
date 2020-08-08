#include <DHAVN_GracenoteDBconnection.h>

/*
 *******************************************************************
 * Function	: createConnection
 *
 * Description 	: Creates Database connection
 *
 * Parameters 	: QString - Database path, QString - connectionName
 *                        QSqlDatabase - Database handle
 *
 * Returns 	: bool - true if connection to DB can be established
 *
 *******************************************************************
 */
bool createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle)
{
    USE_LOG_UTIL
    LOG_INIT_V2("Media_GracenotePluginProcess");

    bool retVal = false;
	 //Added by Puneet for fixing GPP crash issue 2013.06.12
    LOG_TRACE << "::createConnection: Sem trying to Acquire with available = " << sem.available() <<  LOG_ENDL;
    sem.acquire(1);
    LOG_TRACE << "::createConnection: Sem Acquired " << LOG_ENDL;

    if(QSqlDatabase::contains(connectionName))
    {
        LOG_TRACE << "::createConnection: already have this connection name"<< LOG_ENDL;

        dBHandle = QSqlDatabase::database(connectionName);
        dBHandle.setDatabaseName(dbPath);
        if(dBHandle.isOpen())
        {
            LOG_TRACE << "::createConnection: already opened successfully"<< LOG_ENDL;
            retVal = true;
        }
        else
        {
            if (!dBHandle.open())
            {
				 //Added by Puneet for fixing GPP crash issue 2013.06.12
                sem.release(1);
                LOG_TRACE << "::createConnection: Sem Released on Error Avail = " << sem.available()<< LOG_ENDL;
                LOG_TRACE << "::createConnection: not opened successfully"<< LOG_ENDL;
                retVal = false;
            }
            else
            {
                LOG_TRACE << "::createConnection: opened successfully"<< LOG_ENDL;
                retVal = true;
            }
        }
    }
    else
    {
        LOG_TRACE << "::createConnection: not already have this connection name"<< LOG_ENDL;
        dBHandle = QSqlDatabase::addDatabase("QSQLITE",connectionName);
        dBHandle.setDatabaseName(dbPath);
        if (!dBHandle.open())
        {
			 //Added by Puneet for fixing GPP crash issue 2013.06.12
            sem.release(1);
            LOG_TRACE << "::createConnection: Sem Released on Error, Avail = " << sem.available() << LOG_ENDL;
            LOG_TRACE << "::createConnection: not opened successfully"<< LOG_ENDL;
            retVal = false;
        }
        else
        {

            LOG_TRACE << "::createConnection: opened successfully"<< LOG_ENDL;
            retVal = true;
        }
    }
    return retVal;
}

//Added by Puneet for fixing GPP crash issue 2013.06.12
////by aettie for Static Test Defect 201300711
void closeConnection(QSqlDatabase &dBHandle)
{
    USE_LOG_UTIL
    LOG_INIT_V2("Media_GracenotePluginProcess");

    dBHandle.close();
    sem.release(1);
    LOG_TRACE << "::closeConnection: Sem Released on request of close with avail = " << sem.available() << LOG_ENDL;
}
