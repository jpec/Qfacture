#include <QSqlTableModel>
#include "EditableSqlModel.h"


EditableSqlModel::EditableSqlModel(QObject *parent, QSqlDatabase &db)
    : QSqlTableModel(parent, db)
{
	setEditStrategy(QSqlTableModel::OnFieldChange);
}

Qt::ItemFlags EditableSqlModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    
    if (index.column() != 0)
        flags |= Qt::ItemIsEditable;
    
    return flags;
}