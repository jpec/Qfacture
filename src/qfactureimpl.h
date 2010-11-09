#ifndef QFACTUREIMPL_H
#define QFACTUREIMPL_H

#include <QMainWindow>
#include <QString>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSettings>

#include "EditableSqlModel.h"
#include "ui_Qfacture.h"


class QfactureImpl : public QMainWindow, public Ui::Qfacture
{
    Q_OBJECT

public:
    QfactureImpl(QWidget *parent=0, Qt::WFlags f=0);
    ~QfactureImpl();

private slots:
    void on_fSearchClient_editingFinished();
    void on_tUndo_clicked();
    void on_tSave_clicked();
    void on_tOpen_clicked();
    void on_aPass_returnPressed();
    void on_sYearCa_lostFocus();
    void on_fArtLink_itemChanged(QTableWidgetItem* item);
    void on_fList_itemDoubleClicked(QTableWidgetItem* item);
    void on_fSave_clicked();
    void on_fPrint_clicked();
    void on_fDel_clicked();
    void on_fNew_clicked();
    void on_fArtList_itemDoubleClicked(QTableWidgetItem* item);
    void on_fClientList_itemDoubleClicked(QListWidgetItem* item);
    void on_aNew_clicked();
    void on_aSave_clicked();
    void on_aDel_clicked();
    void on_cDel_clicked();
    void on_action_propos_activated();
    void on_cSave_clicked();
    void on_cNew_clicked();
    void on_uChangeLogo_clicked();
    void on_uSave_clicked();
    void on_aConnect_clicked();
    void on_action_Quitter_triggered();

    void doQuit();
    void loadUserInfos();
    void refreshCustomersList();
    void refreshInvoiceCustomersList();
    void fArtLinkRefresh();
    void refreshInvoiceProductsList();
    void refreshInvoicesList();
    void updateInvoiceAmount();
    void sListCaRefresh();
    void sListNpRefresh();
    void refreshProductsList();

    void onCustomerSelected(const QModelIndex &item);
    void onProductSelected(const QModelIndex &item);

signals:
    void DBConnected();
    void clientSaved();
    void clientDeleted();
    void articleSaved();
    void articleDeleted();
    void factureSaved();
    void factureDeleted();
    void factureArticlesUpdated();

protected:
    QString VERSION;
    QSqlDatabase db;
    QSettings *settings;

    bool fFlag;

    EditableSqlModel *clients_model;
    EditableSqlModel *products_model;

    void createActions();
    void MySQLConnect();
    void readSettings();
    void writeSettings();

    bool confirm(const char * msg);

    QString compactDate(QString date);
    QString dateToDB(QDateEdit *date);
    QString makeFactureReference(QString number, QString date);
    QString getInvoiceHTMLTpl();
    QString setInvoiceHTMLTpl(QString Tpl);
};
#endif
