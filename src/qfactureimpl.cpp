#include <QMessageBox>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QSqlTableModel>
#include "qfactureimpl.h"

QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

//
QfactureImpl::QfactureImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
	setupUi(this);
}
//

/* Menu **********************************************************************/

void QfactureImpl::on_action_Quitter_triggered()
{
	// Quitter l'application
	
	QString msg = QString(trUtf8("Enregistrer les modifications avant de quitter?"));
	switch (QMessageBox::warning(this, "Qfacture", msg, QMessageBox::Yes, QMessageBox::Cancel, QMessageBox::No)) {
		case QMessageBox::Yes:
			if (true)
				db.close();
				qApp->quit();
			break;
		case QMessageBox::No:
			db.close();
			qApp->quit();
			break; 
  }
}

void QfactureImpl::on_action_propos_activated()
{
	// A propos
	QString msg = QString(trUtf8("Qfacture v0.1\n"
	"Le logiciel libre de facturation pour les Auto-Entrepreneurs!\n"
	"--\n"
	"Copyright 2010 : Julien PECQUEUR\n"
	"Licence : GPL\n"
	"Auteur : Julien PECQUEUR (jpec@julienpecqueur.com)\n"
	"Contributeur(s) :\n"
	" ...\n"
	"\n"));
	QMessageBox::about(this, "Qfacture", msg);
}

/* Tab Paramètres ************************************************************/

void QfactureImpl::on_aConnect_clicked()
{
	// Se connecter/déconnecter à la base MySQL
	
	QString Connected = QString(trUtf8("Connecté!"));
	QString Disconnected = QString(trUtf8("Déconnecté!"));
	QString Connect = QString(trUtf8("Connecter"));
	QString Disconnect = QString(trUtf8("Déconnecter"));
	
	if (aFlag->text() != Connected) {
		// Se connecter à la base
		if ( MySQL_connect() ) {
			aFlag->setText(Connected);
			aConnect->setText(Disconnect);
			uSave->setEnabled(true);
			uGroupBox->setEnabled(true);
			aServer->setEnabled(false);
			aPort->setEnabled(false);
			aUser->setEnabled(false);
			aPass->setEnabled(false);
			aDb->setEnabled(false);
		}
	} else {
		// Se déconnecter de la base
		QString msg = QString(trUtf8("Voulez-vous réellement vous déconnecter du serveur?"));
		if (QMessageBox::warning(this, "Qfacture", msg, QMessageBox::Yes, QMessageBox::Cancel, QMessageBox::No) == QMessageBox::Yes) {
			aFlag->setText(Disconnected);
			aConnect->setText(Connect);
			uSave->setEnabled(false);
			uGroupBox->setEnabled(false);
			aServer->setEnabled(true);
			aPort->setEnabled(true);
			aUser->setEnabled(true);
			aPass->setEnabled(true);
			aDb->setEnabled(true);
		}
	}
}

bool QfactureImpl::MySQL_connect()
{
	// Se connecter à la base MySQL
	
	db.setHostName(aServer->text());
	db.setPort(aPort->text().toInt());
	db.setUserName(aUser->text());
	db.setPassword(aPass->text());
	db.setDatabaseName(aDb->text());
	if (!db.open()) {
		aFlag->setText(QString(trUtf8("Une erreur est survenue lors de la connection!")));
		return false;
	} else {
		aFlag->setText(QString(trUtf8("Connection en cours...")));
	}
	
	// On recupération des infos utilisateur
	QSqlQuery query;
	query.exec("SELECT Name, Siret, Adress, Adress2, Zip, City, Phone, Mail, Home, Logo FROM user WHERE id = 1;");
	while (query.next()) {
		uName->setText(query.value(0).toString());
		uSiret->setText(query.value(1).toString());
		uAdress->setText(query.value(2).toString());
		uAdress2->setText(query.value(3).toString());
		uZip->setText(query.value(4).toString());
		uCity->setText(query.value(5).toString());
		uPhone->setText(query.value(6).toString());
		uMail->setText(query.value(7).toString());
		uHome->setText(query.value(8).toString());
		QByteArray ba1 = query.value(9).toByteArray();
		QPixmap pic;
		pic.loadFromData(ba1);
		uLogo->setPixmap(pic);
	}
	
	bool Res = tClient_refresh();
	return (true);
}

/**
 * Sauvegarde les informations sur l'utilisateur/Auto-entrepreneur
 * lors du clic sur le bouton d'enregistrement
 * 
 * @return void
 */
void QfactureImpl::on_uSave_clicked()
{
	QSqlQuery query;
    
    // création de la requête
	query.prepare(
		"UPDATE user "
		"SET Name = :name, Siret = :siret, Adress = :adress, Adress2 = :adress2, "
		"    Zip = :zip, City = :city, Phone = :phone, Mail = :mail, Home = :home "
		"WHERE id = 1"
	);
    
    // définition des paramètres
	query.bindValue(":name", uName->text());
	query.bindValue(":siret", uSiret->text());
	query.bindValue(":adress", uAdress->text());
	query.bindValue(":adress2", uAdress2->text());
	query.bindValue(":zip", uZip->text());
	query.bindValue(":city", uCity->text());
	query.bindValue(":phone", uPhone->text());
	query.bindValue(":mail", uMail->text());
	query.bindValue(":home", uHome->text());
    
    // exécution de la requête
	if(query.exec())
		statusbar->showMessage(trUtf8("Modifications enregistrées avec succès."), 3000);
    else
        statusbar->showMessage(trUtf8("Erreur lors de la sauvegarde des paramètres !"));
}

void QfactureImpl::on_uChangeLogo_clicked()
{
	// Sélection du logo
	
	QString image = QFileDialog::getOpenFileName(this, QString(trUtf8("Qfacture - Importer un logo...")), "", tr("Image Files (*.png *.jpg *.bmp)"));
	if (image.isNull())
		return;
	// Effacer le texte	
	uLogo->text().clear();
	// Lecture du fichier image 
	QByteArray ba;	
	QFile f(image);
	if(f.open(QIODevice::ReadOnly))
	{
		ba = f.readAll();
		f.close();
	}	
	// Enregistrement de l'image dans la base SQL
	QSqlQuery query;
	query.prepare("UPDATE user SET Logo = :logo WHERE id = 1");
	query.bindValue(":logo", ba);
	query.exec();
	// Affichage de l'image
	QPixmap pixmap(image);
	uLogo->setPixmap(pixmap);
}

/* Tab Clients ***************************************************************/

void QfactureImpl::on_cNew_clicked()
{
	// Nouveau client
	
	cGroupBox->setEnabled(true);
	cSave->setEnabled(true);
	cId->setEnabled(false);
	cId->setText(QString("new"));
	cName->setText(QString(""));
	cAdress->setText(QString(""));
	cAdress2->setText(QString(""));
	cZip->setText(QString(""));
	cCity->setText(QString(""));
	cPhone->setText(QString(""));
	cMail->setText(QString(""));
}

void QfactureImpl::on_cSave_clicked()
{
	// Enregistrer nouveau client
	
	if (cId->text() == QString("new")) {
		// Nouveau client (création instance)
		QSqlQuery query;
		query.prepare(
			"INSERT INTO client(Name, Adress, Adress2, Zip, City, Phone, Mail) "
			"VALUES(:name, :adress, :adress2, :zip, :city, :phone, :mail)"
		);
		query.bindValue(":name", cName->text());
		query.bindValue(":adress", cAdress->text());
		query.bindValue(":adress2", cAdress2->text());
		query.bindValue(":zip", cZip->text());
		query.bindValue(":city", cCity->text());
		query.bindValue(":phone", cPhone->text());
		query.bindValue(":mail", cMail->text());
		query.exec();
		QString Id;
		Id = query.lastInsertId().toString();
		query.finish();
		cId->setText(QString("new"));
		cName->setText(QString(""));
		cAdress->setText(QString(""));
		cAdress2->setText(QString(""));
		cZip->setText(QString(""));
		cCity->setText(QString(""));
		cPhone->setText(QString(""));
		cMail->setText(QString(""));
		cSave->setEnabled(false);
		cTable->reset();
	} else {
		// Client existant (modification instance)
		
	}
}

bool QfactureImpl::tClient_refresh()
{
	// Rafraichit la table client 

	QSqlTableModel *model = new QSqlTableModel;
	model->setTable("client");
	model->setEditStrategy(QSqlTableModel::OnRowChange);
	model->select();
	model->setHeaderData(0, Qt::Horizontal, QString(trUtf8("IdClient")));
	model->setHeaderData(1, Qt::Horizontal, QString(trUtf8("Nom")));
	model->setHeaderData(2, Qt::Horizontal, QString(trUtf8("Adresse")));
	model->setHeaderData(3, Qt::Horizontal, QString(trUtf8("Complément")));
	model->setHeaderData(4, Qt::Horizontal, QString(trUtf8("Code postal")));
	model->setHeaderData(5, Qt::Horizontal, QString(trUtf8("Ville")));
	model->setHeaderData(6, Qt::Horizontal, QString(trUtf8("Téléphone")));
	model->setHeaderData(7, Qt::Horizontal, QString(trUtf8("Email")));
	cTable->setModel(model);
	cTable->show();

	return true;
}


