#ifndef QFACTUREIMPL_H
#define QFACTUREIMPL_H
//
#include <QMainWindow>
#include <QString>
#include <QSqlDatabase>
#include "ui_Qfacture.h"
//
class QfactureImpl : public QMainWindow, public Ui::Qfacture
{
Q_OBJECT
public:
	QfactureImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
	
private slots:
	void on_aPass_returnPressed();
	void on_sYearCa_textChanged(QString );
	void on_sYearCa_editingFinished();
	void on_sYearCa_lostFocus();
	void on_sListCa_itemDoubleClicked(QListWidgetItem* item);
	void on_fArtLink_itemChanged(QTableWidgetItem* item);
	void on_fList_itemDoubleClicked(QListWidgetItem* item);
	void on_fCalc_clicked();
	void on_fSave_clicked();
	void on_fPrint_clicked();
	void on_fDel_clicked();
	void on_fNew_clicked();
	void on_fArtList_itemDoubleClicked(QListWidgetItem* item);
	void on_fClientList_itemDoubleClicked(QListWidgetItem* item);
	void on_aList_itemClicked(QListWidgetItem* item);
	void on_aNew_clicked();
	void on_aSave_clicked();
	void on_aDel_clicked();
	void on_cDel_clicked();
	void on_cList_itemClicked(QListWidgetItem* item);
	void on_action_propos_activated();
	void on_cSave_clicked();
	void on_cNew_clicked();
	void on_uChangeLogo_clicked();
	void on_uSave_clicked();
	void on_aConnect_clicked();
	void on_action_Quitter_triggered();
    
    void loadUserInfos();
    
    bool cListRefresh();
    bool fClientListRefresh();
    
    bool fArtListRefresh();
    bool aListRefresh();
    
    bool fListRefresh();

signals:
    void DBConnected();

protected:
	QString VERSION;
	QSqlDatabase db;
    
	bool fFlag;
    
    void createActions();
	bool MySQL_connect();
	bool fArtLinkRefresh();
	bool sListCaRefresh();
};
#endif


