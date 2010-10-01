#ifndef EDITABLESQLMODEL_H
#define EDITABLESQLMODEL_H

#include <QSqlTableModel>


class EditableSqlModel : public QSqlTableModel
{
    Q_OBJECT

    public:
        EditableSqlModel(QObject *parent, QSqlDatabase &db);

        Qt::ItemFlags flags(const QModelIndex &index) const;
 };

 #endif