#include <QSqlTableModel>
#include "EditableSqlModel.h"


EditableSqlModel::EditableSqlModel(QObject *parent, QSqlDatabase &db)
    : QSqlTableModel(parent, db)
{
    refresh();
}

Qt::ItemFlags EditableSqlModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    
    if (index.column() != 0)
        flags |= Qt::ItemIsEditable;
    
    return flags;
}

void EditableSqlModel::refresh()
{
    clear();
    
    setEditStrategy(QSqlTableModel::OnFieldChange);
    setTable("client");
    
    setHeaderData(0, Qt::Horizontal, trUtf8("ID"));
    setHeaderData(1, Qt::Horizontal, trUtf8("Nom"));
    setHeaderData(2, Qt::Horizontal, trUtf8("Adresse"));
    setHeaderData(3, Qt::Horizontal, trUtf8("Complément"));
    setHeaderData(4, Qt::Horizontal, trUtf8("Code postal"));
    setHeaderData(5, Qt::Horizontal, trUtf8("Ville"));
    setHeaderData(6, Qt::Horizontal, trUtf8("Téléphone"));
    setHeaderData(7, Qt::Horizontal, trUtf8("Mail"));
    
    select();
}