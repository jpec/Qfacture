#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QSqlTableModel>
#include <QListWidget>
#include <QModelIndex>
#include <QTableWidget>
#include <QDate>
#include "qfactureimpl.h"


//
QfactureImpl::QfactureImpl( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	setupUi(this);
	
	VERSION = QString(trUtf8("v0.1-alpha"));
	db = QSqlDatabase::addDatabase("QMYSQL");
}
//

/* Menu **********************************************************************/

void QfactureImpl::on_action_Quitter_triggered()
{
	// Quitter l'application
	
	QString msg = QString(trUtf8("Voulez vous quitter l'application ?"));
	switch (QMessageBox::warning(this, "Qfacture", msg, QMessageBox::Yes, QMessageBox::No)) {
		case QMessageBox::Yes:
			if (true)
				db.close();
				qApp->quit();
			break;
		case QMessageBox::No:
			break;
  }
}

void QfactureImpl::on_action_propos_activated()
{
	// A propos
	QString msg = QString(trUtf8("Qfacture :VERSION\n"
	"Le logiciel libre de facturation pour les Auto-Entrepreneurs!\n"
	"--\n"
	"Copyright 2010 : Julien PECQUEUR\n"
	"Licence : GPL\n"
	"Auteur : Julien PECQUEUR <jpec@julienpecqueur.com>\n"
	"Contributeur(s) :\n"
	" * Module génération PDF : Kévin GOMEZ <contact@kevingomez.fr>\n"
	" * Logo : Kevin MACPHAIL <http://kmacphail.blogspot.com>\n"
	"\n"));
	msg.replace(":VERSION", VERSION);
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
	fClientListRefresh();
	
	// rechargement de la liste des articles
	aListRefresh();
	fArtListRefresh();
	
	// rechargement de la liste des factures
	fListRefresh();
	
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
	
	// Message status bar
	statusbar->showMessage(trUtf8("Le logo a été enregistré avec succès."), 3000);
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
		query.prepare("SELECT Name FROM client WHERE Name = :name");
		query.bindValue(":name", cName->text());
		query.exec();
		int Res = query.size();
		query.finish();
		if (Res > 0) {
			// Doublon
			QString msg = QString(trUtf8("Le nom saisi existe déjà dans la base!"));
			QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Ok);
		} else {
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
			statusbar->showMessage(trUtf8("Le nouveau client a été enregistré avec succès."), 3000);
		}
	} else {
		// Client existant (modification instance)
		QSqlQuery query;
				query.prepare("SELECT Name FROM client WHERE Name = :name");
		query.bindValue(":name", cName->text());
		query.exec();
		int Res = query.size();
		query.finish();
		if (Res > 0) {
			// Doublon
			QString msg = QString(trUtf8("Le nom saisi existe déjà dans la base!"));
			QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Ok);
		} else {
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
			statusbar->showMessage(trUtf8("Les modifications ont été enregistrées avec succès."), 3000);
		}
	}
	cListRefresh();
	fClientListRefresh();
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
			+ QString(trUtf8(" | ")) + query.value(2).toString()
			+ QString(trUtf8(" ")) + query.value(3).toString()
			+ QString(trUtf8(" ")) + query.value(4).toString()
			+ QString(trUtf8(" ")) + query.value(5).toString()
			+ QString(trUtf8(" - ")) + query.value(6).toString()
			+ QString(trUtf8(" - ")) + query.value(7).toString()
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
	int NameLen = Text.indexOf(QString(" | "), 0);
	QString Name = Text.mid(0, NameLen);
	QSqlQuery query;
	query.prepare(
		"SELECT Id, Name, Adress, Adress2, Zip, City, Phone, Mail "
		"FROM client "
		"WHERE Name = :name "
	);
	query.bindValue(":name", Name);
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
		fClientListRefresh();
		statusbar->showMessage(trUtf8("Le client a été supprimé avec succès."), 3000);
	}
}

/* Tab Articles **************************************************************/

void QfactureImpl::on_aNew_clicked()
{
	// Nouvel article
	
	artGroupBox->setEnabled(true);
	aSave->setEnabled(true);
	aDel->setEnabled(false);
	aId->setEnabled(false);
	aId->setText(QString("new"));
	aName->setText(QString(""));
	aPrice->setText(QString("0.00"));
	aCom->setText(QString(""));
}

void QfactureImpl::on_aSave_clicked()
{
	// Enregistrer nouvel article
	if (aId->text() == QString("new")) {
		// Nouvel article (création instance)
		QSqlQuery query;
		query.prepare("SELECT Name FROM article WHERE Name = :name");
		query.bindValue(":name", aName->text());
		query.exec();
		int Res = query.size();
		query.finish();
		if (Res > 0) {
			// Doublon
			QString msg = QString(trUtf8("Le nom d'article saisi existe déjà dans la base!"));
			QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Ok);
		} else {
			QSqlQuery query;
			query.prepare(
				"INSERT INTO article(Name, Price, Comment) "
				"VALUES(:name, :price, :comment)"
			);
			query.bindValue(":name", aName->text());
			query.bindValue(":price", aPrice->text());
			query.bindValue(":comment", aCom->text());
			query.exec();
			QString Id;
			Id = query.lastInsertId().toString();
			query.finish();
			aId->setText(QString("new"));
			aName->setText(QString(""));
			aPrice->setText(QString("0.00"));
			aCom->setText(QString(""));
			aDel->setEnabled(false);
			statusbar->showMessage(trUtf8("Le nouvel article a été enregistré avec succès."), 3000);
		}
	} else {
		// Article existant (modification instance)
		QSqlQuery query;
		query.prepare("SELECT Name FROM article WHERE Name = :name");
		query.bindValue(":name", aName->text());
		query.exec();
		int Res = query.size();
		query.finish();
		if (Res > 0) {
			// Doublon
			QString msg = QString(trUtf8("Le nom d'article saisi existe déjà dans la base!"));
			QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Ok);
		} else {
			query.prepare(
				"UPDATE article "
				"SET Name = :name, Price = :price, Comment = :comment "
				"WHERE Id = :id "
			);
			query.bindValue(":id", aId->text());
			query.bindValue(":name", aName->text());
			query.bindValue(":price", aPrice->text());
			query.bindValue(":comment", aCom->text());
			query.exec();
			QString Test = query.executedQuery();
			query.finish();
			aSave->setEnabled(false);
			aDel->setEnabled(false);
			statusbar->showMessage(trUtf8("Les modifications ont été enregistrées avec succès."), 3000);
		}
	}
	aListRefresh();
	fArtListRefresh();
}

void QfactureImpl::on_aDel_clicked()
{
	// Supprimer un article
	
	QString msg = QString(trUtf8("Voulez-vous supprimer l'article sélectionné ?\n\n")) ;
	if (QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		QSqlQuery query;
		query.prepare("DELETE FROM article WHERE Id = :id");
		query.bindValue(":id", aId->text());
		query.exec();
		query.finish();
		aId->setText(QString("new"));
		aName->setText(QString(""));
		aPrice->setText(QString("0.00"));
		aCom->setText(QString(""));
		aDel->setEnabled(false);
		aNew->setEnabled(true);
		aSave->setEnabled(false);
		aListRefresh();
		fArtListRefresh();
		statusbar->showMessage(trUtf8("L'article a été supprimé avec succès."), 3000);
	}
}

bool QfactureImpl::aListRefresh()
{
	// Rafraichit la liste des articles
	
	aList->clear();
	QSqlQuery query;
	query.prepare(
		"SELECT Id, Name, Price, Comment "
		"FROM article "
		"ORDER BY Name"
	);
	query.exec();
	while (query.next()) {
		// Ajout d'une ligne à la liste pour chaque client!
		QString Item = query.value(1).toString() 
			+ QString(trUtf8(" | ")) + query.value(2).toString()
			+ QString(trUtf8("€ (")) + query.value(3).toString()
			+ QString(trUtf8(")"))
			;
		aList->addItem(Item);
	}
	query.finish();
	return true;
}

void QfactureImpl::on_aList_itemClicked(QListWidgetItem* item)
{
	// Modification d'un article de la liste
	
	QString Text = item->text();
	int NameLen = Text.indexOf(QString(" | "), 0);
	QString Name = Text.mid(0, NameLen);
	QSqlQuery query;
	query.prepare(
		"SELECT Id, Name, Price, Comment "
		"FROM article "
		"WHERE Name = :name"
	);
	query.bindValue(":name", Name);
	query.exec();
	while (query.next()) {
		artGroupBox->setEnabled(true);
		aSave->setEnabled(true);
		aDel->setEnabled(true);
		aId->setEnabled(false);
		aId->setText(query.value(0).toString());
		aName->setText(query.value(1).toString());
		aPrice->setText(query.value(2).toString());
		aCom->setText(query.value(3).toString());
	}
	query.finish();
}

/* Tab facture - liste des factures ******************************************/

bool QfactureImpl::fListRefresh()
{
	// Rafraichit la liste des factures
	
	fList->clear();
	QSqlQuery query;
	query.prepare(
		"SELECT "
		"    f.id, f.Amount, f.Comment, f.Payment, f.Reference, f.Type, f.Date, "
		"    c.Name "
		"FROM facture AS f "
		"LEFT JOIN client AS c "
		"    ON f.idClient = c.id "
		"ORDER BY Reference DESC"
	);
	query.exec();
	while (query.next()) {
		// Ajout d'une ligne à la liste pour chaque client!
		QString Item = query.value(0).toString() 
			+ QString(trUtf8(" | ")) + query.value(4).toString()
			+ QString(trUtf8(" | ")) + query.value(5).toString()
			+ QString(trUtf8(" | ")) + query.value(6).toString()
			+ QString(trUtf8(" | ")) + query.value(7).toString()
			+ QString(trUtf8(" | ")) + query.value(1).toString()
			+ QString(trUtf8("€ (")) + query.value(3).toString()
			+ QString(trUtf8(")"))
			;
		fList->addItem(Item);
	}
	query.finish();
	return true;
}

void QfactureImpl::on_fList_itemDoubleClicked(QListWidgetItem* item)
{
	// Ouvre la facture sélectionnée en édition
	
	QString Text = item->text();
	int IdLen = Text.indexOf(QString(" | "), 0);
	QString Id = Text.mid(0, IdLen);
	QSqlQuery query;
	query.prepare(
		"SELECT Id, IdClient, Amount, Comment, Payment, Reference, Type, Date "
		"FROM facture "
		"WHERE Id = :Id"
	);
	query.bindValue(":Id", Id);
	query.exec();
	QString Client;
	while (query.next()) {
		fNum->setText(query.value(0).toString());
		fDate->setDate(query.value(7).toDate());
		fMontant->setText(query.value(2).toString());
		fRegl->setEditText(query.value(4).toString());
		fType->setEditText(query.value(6).toString());
		Client = query.value(1).toString();
		fCalc->setEnabled(true);
		fSave->setEnabled(true);
		fPrint->setEnabled(true);
		fDel->setEnabled(true);
		fArticleGroup->setEnabled(true);
		fClientGroup->setEnabled(true);
		fDate->setEnabled(true);
		fType->setEnabled(true);
		fRegl->setEnabled(true);
	}
	query.finish();
	query.prepare(
		"SELECT Name "
		"FROM client "
		"WHERE Id = :Id"
	);
	query.bindValue(":Id", Client);
	query.exec();
	query.next();
	fClient->setText(query.value(0).toString());
	tabFacture->setCurrentIndex(0);
	fArtLinkRefresh();
	statusbar->showMessage(trUtf8("La facture est chargée avec succès."), 3000);
}

/* Tab facture - éditer une facture ******************************************/

bool QfactureImpl::fClientListRefresh()
{
	// Rafraichit la liste des clients de l'onglet facture
	
	fClientList->clear();
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
			+ QString(trUtf8(" | ")) + query.value(2).toString()
			+ QString(trUtf8(" ")) + query.value(3).toString()
			+ QString(trUtf8(" ")) + query.value(4).toString()
			+ QString(trUtf8(" ")) + query.value(5).toString()
			+ QString(trUtf8(" - ")) + query.value(6).toString()
			+ QString(trUtf8(" - ")) + query.value(7).toString()
			;
		fClientList->addItem(Item);
	}
	query.finish();
	return true;
}

void QfactureImpl::on_fClientList_itemDoubleClicked(QListWidgetItem* item)
{
	// Sélection du client de la facture (alimente le champ fClient)
	
	QString Text = item->text();
	int NameLen = Text.indexOf(QString(" | "), 0);
	QString Name = Text.mid(0, NameLen);
	QSqlQuery query;
	query.prepare(
		"SELECT Id, Name "
		"FROM client "
		"WHERE Name = :name "
	);
	query.bindValue(":name", Name);
	query.exec();
	while (query.next()) {
		fClient->setText(query.value(1).toString());
	}
	query.finish();
	statusbar->showMessage(trUtf8("Client ajouté sur la facture."), 3000);
}

bool QfactureImpl::fArtListRefresh()
{
	// Rafraichit la liste des articles de l'onglet facture
	
	fArtList->clear();
	QSqlQuery query;
	query.prepare(
		"SELECT Id, Name, Price, Comment "
		"FROM article "
		"ORDER BY Name"
	);
	query.exec();
	while (query.next()) {
		// Ajout d'une ligne à la liste pour chaque client!
		QString Item = query.value(1).toString() 
			+ QString(trUtf8(" | ")) + query.value(2).toString()
			+ QString(trUtf8("€ (")) + query.value(3).toString()
			+ QString(trUtf8(")"))
			;
		fArtList->addItem(Item);
	}
	query.finish();
	return true;
}

void QfactureImpl::on_fArtList_itemDoubleClicked(QListWidgetItem* item)
{
	// Ajout d'un article dans la facture
	
	QString Text = item->text();
	int NameLen = Text.indexOf(QString(" | "), 0);
	QString Name = Text.mid(0, NameLen);
	QSqlQuery query;
	query.prepare("SELECT Id, Price FROM article WHERE Name = :name");
	query.bindValue(":name", Name);
	query.exec();
	query.next();
	QString Art = QString(query.value(0).toString());
	QString Price = QString(query.value(1).toString());
	query.finish();
	query.prepare(
		"INSERT INTO link(IdFacture, IdArticle, Quantity, Off, Price, Amount) "
		"VALUES (:num, :art, 1, 0, :price, :amount)"
	);
	query.bindValue(":num", fNum->text());
	query.bindValue(":art", Art);
	query.bindValue(":price", Price);
	query.bindValue(":amount", Price);
	query.exec();
	query.finish();
	fArtLinkRefresh();
	statusbar->showMessage(trUtf8("Article ajouté sur la facture."), 3000);
}

bool QfactureImpl::fArtLinkRefresh()
{
	// Rafraichit la liste des liens de l'onglet facture
	
	fArtLink->clearContents();
	QSqlQuery query;
	query.prepare(
		"SELECT l.id, a.name, l.price, l.quantity, l.off, l.amount "
		"FROM link AS l "
		"LEFT JOIN article AS a "
		"ON a.id = l.idarticle "
		"WHERE l.idfacture = :idFacture "
		"ORDER BY l.id "
	);
	query.bindValue(":IdFacture", fNum->text());
	query.exec();
	fArtLink->setRowCount(query.size());
	fArtLink->setAlternatingRowColors(true);
	//fArtLink->verticalHeader()->setVisible(false);
	int i=0;
	while (query.next()) {
		// Ajout d'une ligne à la liste pour chaque lien
		QTableWidgetItem *Id = new QTableWidgetItem(query.value(0).toString());
		fArtLink->setItem(i, 0, Id);
		QTableWidgetItem *Name = new QTableWidgetItem(query.value(1).toString());
		fArtLink->setItem(i, 1, Name);
		QTableWidgetItem *Price = new QTableWidgetItem(query.value(2).toString());
		fArtLink->setItem(i, 2, Price);
		QTableWidgetItem *Nbr = new QTableWidgetItem(query.value(3).toString());
		fArtLink->setItem(i, 3, Nbr);
		QTableWidgetItem *Off = new QTableWidgetItem(query.value(4).toString());
		fArtLink->setItem(i, 4, Off);
		QTableWidgetItem *Mont = new QTableWidgetItem(query.value(5).toString());
		fArtLink->setItem(i, 5, Mont);
		i++;
	}
	query.finish();
	return true;
}



void QfactureImpl::on_fCalc_clicked()
{
	// Bouton calculer le montant total de la facture
	
	QSqlQuery query;
	query.prepare("SELECT SUM(Amount) FROM link WHERE IdFacture = :id");
	query.bindValue(":id", fNum->text());
	query.exec();
	query.next();
	fMontant->setText(query.value(0).toString());
	query.finish();
	fArtLinkRefresh();
}

void QfactureImpl::on_fSave_clicked()
{
	// Bouton sauver la facture
	
	/*
	 * 
	 *  IL FAUDRA RAJOUTER DES CONTROLES SUR LES CHAMPS OBLIGATOIRES
	 *  * Client,
	 *  * Type,
	 *  * Date
	 * 
 	 */
	
	if (fNum->text() == QString(trUtf8("0"))) {
		// Nouvelle facture
		QSqlQuery query;
		QString Client;
		query.prepare("SELECT COUNT(Id)+1 FROM facture WHERE Date = :date ");
		query.bindValue(":date", fDate->text().mid(6,4)+QString("-")+fDate->text().mid(3,2)+QString("-")+fDate->text().mid(0,2));
		query.exec();
		query.next();
		QString Count = query.value(0).toString();
		query.finish();
		query.prepare(
			"SELECT Id, Name "
			"FROM client "
			"WHERE Name = :name "
		);
		query.bindValue(":name", fClient->text());
		query.exec();
		while (query.next()) {
			Client = query.value(0).toString();
		}
		query.finish();
		query.prepare(
			"INSERT INTO facture ( "
			"    idClient, "
			"    Amount, "
			//"    Comment, " non implémenté
			"    Payment, "
			"    Reference, "
			"    Type, "
			"    Date  "
			") VALUES ("
			"    :client, "
			"    :amount, "
			//"    NULL, " non implémenté
			"    :pay, "
			"    :ref, "
			"    :type, "
			"    :date "
			")"
		);
		query.bindValue(":client", Client);
		query.bindValue(":amount", fMontant->text());
		query.bindValue(":pay", fRegl->currentText());
		query.bindValue(":ref",fDate->text().mid(6,4)+fDate->text().mid(3,2)+fDate->text().mid(0,2)+Count.rightJustified(3, '0'));
		query.bindValue(":type", fType->currentText());
		query.bindValue(":date", fDate->text().mid(6,4)+QString("-")+fDate->text().mid(3,2)+QString("-")+fDate->text().mid(0,2));
		query.exec();
		fNum->setText(query.lastInsertId().toString());
		query.finish();
		query.prepare("UPDATE link SET IdFacture = :id WHERE IdFacture = 0");
		query.bindValue(":id", fNum->text());
		query.exec();
		query.finish();
		statusbar->showMessage(trUtf8("La nouvelle facture est bien enregistrée."), 3000);
	} else {
		// Mise à jour facture existante
		QSqlQuery query;
		QString Client;
		query.finish();
		query.prepare(
			"SELECT Id, Name "
			"FROM client "
			"WHERE Name = :name "
		);
		query.bindValue(":name", fClient->text());
		query.exec();
		while (query.next()) {
			Client = query.value(1).toString();
		}
		query.finish();
		query.prepare(
			"UPDATE facture "
			"SET idClient = :client "
			"    Amount = :amount "
			//"    Comment = NULL " non implémenté
			"    Payment = :pay "
			//"    Reference = :ref "
			"    Type = :type "
			"    Date = :date "
			"WHERE IdFacture = :id"
		);
		query.bindValue(":id", fNum->text());
		query.bindValue(":client", Client);
		query.bindValue(":amount", fMontant->text());
		query.bindValue(":pay", fRegl->currentText());
		query.bindValue(":type", fType->currentText());
		query.bindValue(":date", fDate->text().mid(6,4)+QString("-")+fDate->text().mid(3,2)+QString("-")+fDate->text().mid(0,2));
		query.exec();
		query.finish();
		statusbar->showMessage(trUtf8("Les modifications sur la facture sont bien enregistrées."), 3000);
	}
	fArtLinkRefresh();
	fListRefresh();
}

void QfactureImpl::on_fPrint_clicked()
{
	// Bouton imprimer la facture
	
	// TO DO : générer fichier pdf et l'ouvrir dans le lecteur pdf (xdg-open ?)
}

void QfactureImpl::on_fDel_clicked()
{
	// Bouton supprimer la facture
	
	QString msg = QString(trUtf8("Voulez-vous supprimer la facture sélectionnée ?\n\n")) ;
	if (QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		QSqlQuery query;
		query.prepare("DELETE FROM link WHERE IdFacture = :id");
		query.bindValue(":id", fNum->text());
		query.exec();
		query.finish();
		query.prepare("DELETE FROM facture WHERE Id = :id");
		query.bindValue(":id", fNum->text());
		query.exec();
		query.finish();
		fCalc->setEnabled(false);
		fSave->setEnabled(false);
		fPrint->setEnabled(false);
		fDel->setEnabled(false);
		fArticleGroup->setEnabled(false);
		fClientGroup->setEnabled(false);
		fDate->setEnabled(false);
		fType->setEnabled(false);
		fRegl->setEnabled(false);
		fNum->setText(QString(trUtf8("0")));
		fDate->setDate(QDate::currentDate());
		fMontant->setText("");
		fClient->setText("");
		statusbar->showMessage(trUtf8("La facture a bien été supprimée."), 3000);
		fArtLinkRefresh();
		fListRefresh();
	}
}

void QfactureImpl::on_fNew_clicked()
{
	// Bouton nouvelle facture
	
	fNum->setText(QString(trUtf8("0")));
	fDate->setDate(QDate::currentDate());
	fMontant->setText("");
	fCalc->setEnabled(true);
	fSave->setEnabled(true);
	fPrint->setEnabled(true);
	fDel->setEnabled(true);
	fArticleGroup->setEnabled(true);
	fClientGroup->setEnabled(true);
	fDate->setEnabled(true);
	fType->setEnabled(true);
	fRegl->setEnabled(true);
	fArtLinkRefresh();
	statusbar->showMessage(trUtf8("Nouvelle facture créée."), 3000);
}

