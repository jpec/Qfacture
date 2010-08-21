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
	void on_action_propos_activated();
	void on_cSave_clicked();
	void on_cNew_clicked();
	void on_uChangeLogo_clicked();
	void on_uSave_clicked();
	void on_aConnect_clicked();
	void on_action_Quitter_triggered();
	bool tClient_refresh();
	bool MySQL_connect();

};
#endif


