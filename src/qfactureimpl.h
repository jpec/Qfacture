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
  /* callbacks */
  void on_aPass_returnPressed();
  void on_sYearCa_lostFocus();
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
  /* methods */
  void loadUserInfos();
  void cListRefresh();
  void fClientListRefresh();
  void fArtLinkRefresh();
  void fArtListRefresh();
  void fListRefresh();
  void fUpdateAmount();
  bool sListCaRefresh();
  void aListRefresh();

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
  bool fFlag;
  void createActions();
  bool MySQL_connect();
  QString compactDate(QString date);
  QString dateToDB(QDateEdit *date);
  QString makeFactureReference(QString number, QString date);
};
#endif


