#ifndef BASEMEDIAMODEL_H
#define BASEMEDIAMODEL_H

#include <QObject>
#include <QtSparql/QSparqlConnection>
#include <QtSparql/QSparqlQuery>
#include <QtSparql/QSparqlQueryModel>
#include <QtSparql/QSparqlQuery>
#include "DHAVN_AppFileManager_CStateManager.h"

class BaseMediaModel : public QObject
{
    Q_OBJECT
public:
    explicit BaseMediaModel(QObject *parent = 0)
        : QObject(parent)
        , model(new QSparqlQueryModel)
    {

    }

    virtual void exec() = 0;
    virtual void cancel() = 0;
    virtual EMediaDataRequestType getRequestType() = 0;

protected:
    QScopedPointer<QSparqlQueryModel> model;
};

#endif // BASEMEDIAMODEL_H
