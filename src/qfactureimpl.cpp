#include <QMessageBox>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QListWidget>
#include <QModelIndex>
#include "qfactureimpl.h"


//
QfactureImpl::QfactureImpl( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	setupUi(this);
	
	db = QSqlDatabase::addDatabase("QMYSQL");
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
	" * Kévin GOMEZ (contact@kevingomez.fr)\n"
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

/**
 * Réalise la connexion à la base de données MySQL
 *
 * @return bool Témoin du succès de la connexion
 */
bool QfactureImpl::MySQL_connect()
{
	QSqlQuery query;
	QPixmap pic;
	
	// Se connecter à la base MySQL
	db.setHostName(aServer->text());
	db.setPort(aPort->text().toInt());
	db.setUserName(aUser->text());
	db.setPassword(aPass->text());
	db.setDatabaseName(aDb->text());
	
	if (db.open())
		aFlag->setText(QString(trUtf8("Connexion en cours ...")));
	else {
		aFlag->setText(QString(trUtf8("Une erreur est survenue lors de la connexion !")));
		return false;
	}

	// Recupération des infos utilisateur
	query.exec("SELECT Name, Siret, Adress, Adress2, Zip, City, Phone, Mail, Home, Logo FROM user WHERE id = 1;");
	while (query.next()) {
		// infos de l'utilisateur
		uName->setText(query.value(0).toString());
		uSiret->setText(query.value(1).toString());
		uAdress->setText(query.value(2).toString());
		uAdress2->setText(query.value(3).toString());
		uZip->setText(query.value(4).toString());
		uCity->setText(query.value(5).toString());
		uPhone->setText(query.value(6).toString());
		uMail->setText(query.value(7).toString());
		uHome->setText(query.value(8).toString());
		// affichage du logo
		pic.loadFromData(query.value(9).toByteArray());
		uLogo->setPixmap(pic);
	}
	
	// rechargement de la liste des clients
	cListRefresh();
	
	return true;
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

/**
 * Change le logo de l'utilisateur/auto-entrepreneur. Appelé automatiquement
 * lors du clic sur le bouton dans l'ui
 *
 * @return void
 */
void QfactureImpl::on_uChangeLogo_clicked()
{
	QString image;
	QPixmap img_pixmap;
	QFile img_file;
	QSqlQuery query;
	
	// Sélection du logo
	image = QFileDialog::getOpenFileName(
										this,
										QString(trUtf8("Qfacture - Importer un logo...")),
										"", tr("Image Files (*.png *.jpg *.bmp)")
										);
	
	// pas d'image sélectionnée
	if (image.isNull())
		return;
	
	// Effacer le texte
	uLogo->text().clear();
	
	// Lecture du fichier image
	img_file.setFileName(image);
	if(!img_file.open(QIODevice::ReadOnly)) {
		uLogo->setText("Impossible d'ouvrir le fichier contenant le logo !");
		return;
	}
	
	// Enregistrement de l'image dans la base SQL
	query.prepare("UPDATE user SET Logo = :logo WHERE id = 1;");
	query.bindValue(":logo", img_file.readAll());
	query.exec();
	
	// Affichage de l'image
	img_pixmap.load(image);
	uLogo->setPixmap(img_pixmap);
	
	// Fermeture du fichier
	img_file.close();
}

/* Tab Clients ***************************************************************/

void QfactureImpl::on_cNew_clicked()
{
	// Nouveau client
	
	cGroupBox->setEnabled(true);
	cSave->setEnabled(true);
	cDel->setEnabled(false);
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
		cDel->setEnabled(false);
	} else {
		// Client existant (modification instance)
		QSqlQuery query;
		query.prepare(
			"UPDATE client "
			"SET Name = :name, Adress = :adress, Adress2 = :adress2, Zip = :zip, City = :city, Phone = :phone, Mail = :mail "
			"WHERE Id = :id "
		);
		query.bindValue(":id", cId->text());
		query.bindValue(":name", cName->text());
		query.bindValue(":adress", cAdress->text());
		query.bindValue(":adress2", cAdress2->text());
		query.bindValue(":zip", cZip->text());
		query.bindValue(":city", cCity->text());
		query.bindValue(":phone", cPhone->text());
		query.bindValue(":mail", cMail->text());
		query.exec();
		QString Test = query.executedQuery();
		query.finish();
		cSave->setEnabled(false);
		cDel->setEnabled(false);
	}
	cListRefresh();
}

bool QfactureImpl::cListRefresh()
{
	// Rafraichit la liste des clients 
	
	cList->clear();
	QSqlQuery query;
	query.prepare(
		"SELECT Id, Name, Adress, Adress2, Zip, City, Phone, Mail "
		"FROM client "
		"ORDER BY Name"
	);
	query.exec();
	while (query.next()) {
		// Ajout d'une ligne à la liste pour chaque client!
		QString Item = query.value(1).toString() 
			+ QString(" (") + query.value(2).toString()
			+ QString(" ") + query.value(3).toString()
			+ QString(" ") + query.value(4).toString()
			+ QString(" ") + query.value(5).toString()
			+ QString(" - ") + query.value(6).toString()
			+ QString(" - ") + query.value(7).toString()
			+ QString(") [") + query.value(0).toString() + QString("]")
			;
		cList->addItem(Item);
	}
	query.finish();
	return true;
}

void QfactureImpl::on_cList_itemClicked(QListWidgetItem* item)
{
	// Modification d'un client de la liste
	
	QString Text = item->text();
	QString Id = Text.section(" [", -1);
	Id = Id.replace(QString("]"), "");
	QSqlQuery query;
	query.prepare(
		"SELECT Id, Name, Adress, Adress2, Zip, City, Phone, Mail "
		"FROM client "
		"WHERE Id = :id"
	);
	query.bindValue(":id", Id);
	query.exec();
	while (query.next()) {
		cGroupBox->setEnabled(true);
		cSave->setEnabled(true);
		cDel->setEnabled(true);
		cId->setEnabled(false);
		cId->setText(query.value(0).toString());
		cName->setText(query.value(1).toString());
		cAdress->setText(query.value(2).toString());
		cAdress2->setText(query.value(3).toString());
		cZip->setText(query.value(4).toString());
		cCity->setText(query.value(5).toString());
		cPhone->setText(query.value(6).toString());
		cMail->setText(query.value(7).toString());
	}
	query.finish();
}

void QfactureImpl::on_cDel_clicked()
{
	// Supprimer un client
	
	QString msg = QString(trUtf8("Voulez-vous supprimer le client sélectionné ?\n\n")) ;
	if (QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		QSqlQuery query;
		query.prepare("DELETE FROM client WHERE Id = :id");
		query.bindValue(":id", cId->text());
		query.exec();
		query.finish();
		cId->setText(QString("new"));
		cName->setText(QString(""));
		cAdress->setText(QString(""));
		cAdress2->setText(QString(""));
		cZip->setText(QString(""));
		cCity->setText(QString(""));
		cPhone->setText(QString(""));
		cMail->setText(QString(""));
		cDel->setEnabled(false);
		cNew->setEnabled(true);
		cSave->setEnabled(false);
		cListRefresh();
	}
}

/* Tab Articles **************************************************************/

void QfactureImpl::on_aNew_clicked()
{
	// TODO
}

void QfactureImpl::on_aSave_clicked()
{
	// TODO
}

void QfactureImpl::on_aDel_clicked()
{
	// TODO
}

bool QfactureImpl::aListRefresh()
{
	// TODO
}

void QfactureImpl::on_aList_itemClicked(QListWidgetItem* item)
{
	// TODO
}
