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
#include <iostream>

//
QfactureImpl::QfactureImpl( QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
	setupUi(this);
	
	VERSION = QString(trUtf8("v0.1-alpha"));
	db = QSqlDatabase::addDatabase("QMYSQL");
	fFlag = true;
    
    createActions();
}
//

/* Fonctionnement interne de l'appli *****************************************/

/**
 * Définit quelles méthodes seront appelées lors de l'émission de certains
 * évènements
 * 
 * @return void
 */
void QfactureImpl::createActions()
{
    /** Actions effectuées à la connexion à la DB **/
    
    // affichage des infos sur l'auto-entrepreneur
    connect(this, SIGNAL(DBConnected()), this, SLOT(loadUserInfos()));
    
    // rechargement de la liste des clients à la connexion à la DB
    connect(this, SIGNAL(DBConnected()), this, SLOT(cListRefresh()));
    connect(this, SIGNAL(DBConnected()), this, SLOT(fClientListRefresh()));
    
    // rechargement de la liste des articles
	connect(this, SIGNAL(DBConnected()), this, SLOT(aListRefresh()));
	connect(this, SIGNAL(DBConnected()), this, SLOT(fArtListRefresh()));
	
	// rechargement de la liste des factures
	connect(this, SIGNAL(DBConnected()), this, SLOT(fListRefresh()));
    
    /** Actions effectuées lors de la sauvegarde d'un client (nouveau ou mise à jour d'un déjà existant) **/
    
    connect(this, SIGNAL(clientSaved()), this, SLOT(cListRefresh()));
	connect(this, SIGNAL(clientSaved()), this, SLOT(fClientListRefresh()));
    
    /** Actions effectuées lors de la suppression d'un client **/
    
    connect(this, SIGNAL(clientDeleted()), this, SLOT(cListRefresh()));
    connect(this, SIGNAL(clientDeleted()), this, SLOT(fClientListRefresh()));
    
    /** Actions effectuées lors de la sauvegarde d'une facture **/
    
    connect(this, SIGNAL(factureSaved()), this, SLOT(fArtLinkRefresh()));
    connect(this, SIGNAL(factureSaved()), this, SLOT(fListRefresh()));
    
    /** Actions effectuées lors de la suppression d'une facture **/
    
    connect(this, SIGNAL(factureDeleted()), this, SLOT(fArtLinkRefresh()));
    connect(this, SIGNAL(factureDeleted()), this, SLOT(fListRefresh()));
    
    /** Actions effectuées lors de l'ajout d'un article à une facture **/
    
    connect(this, SIGNAL(factureArticlesUpdated()), this, SLOT(fArtLinkRefresh()));
    connect(this, SIGNAL(factureArticlesUpdated()), this, SLOT(fUpdateAmount()));
    
    /** Actions effectuées lors de la sauvegarde d'un article **/
    
    connect(this, SIGNAL(articleSaved()), this, SLOT(aListRefresh()));
    connect(this, SIGNAL(articleSaved()), this, SLOT(fArtListRefresh()));
    
    /** Actions effectuées lors de la suppression d'un article **/
    
    connect(this, SIGNAL(articleDeleted()), this, SLOT(aListRefresh()));
    connect(this, SIGNAL(articleDeleted()), this, SLOT(fArtListRefresh()));
}

/**
 * Réalise la connexion à la base de données MySQL
 *
 * @return bool Témoin du succès de la connexion
 */
bool QfactureImpl::MySQL_connect()
{	
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
    
    // on signale à l'appli que la connexion à réussi
    emit DBConnected();
	
	return true;
}

/**
 * Génère une référence de facture.
 * 
 * @param number Numéro de la facture dans la journée
 * @param date Date d'édition de la facture
 * 
 * @return QString La référence
 */
QString QfactureImpl::makeFactureReference(QString number, QString date)
{
    return compactDate(date) + number.rightJustified(3, '0');
}

/**
 * Compacte une date pour ensuite être utilisée dans les références de
 * factures
 * 
 * @param date La date à compacter
 * 
 * @return QString La date compactée
 */
QString QfactureImpl::compactDate(QString date)
{
    return date.mid(6,4)
         + date.mid(3,2)
         + date.mid(0,2);
}

/**
 * Renvoie la valeur d'un QDateEdit de telle sorte qu'elle puisse être 
 * insérée en DB
 * 
 * @param date Pointeur vers le QDateEdit
 * 
 * @return QString
 */
QString QfactureImpl::dateToDB(QDateEdit *date)
{
    return date->text().mid(6,4)
         + QString("-")
         + date->text().mid(3,2)
         + QString("-")
         + date->text().mid(0,2);
}


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
	"--\n"
	"Contributeur(s) :\n"
	" * Module génération PDF : Kévin GOMEZ <contact@kevingomez.fr>\n"
	" * Logo : Kevin MACPHAIL <http://kmacphail.blogspot.com>\n"
	"\n"));
	msg.replace(":VERSION", VERSION);
	QMessageBox::about(this, "Qfacture", msg);
}


/* Tab Paramètres ************************************************************/

/**
 * Charge les informations sur l'auto-entrepreneur à afficher dans l'onglet
 * paramètres
 * 
 * @return void
 */
void QfactureImpl::loadUserInfos()
{
    QSqlQuery query;
	QPixmap pic;
    
    // Recupération des infos utilisateur
	query.exec("SELECT Name, Siret, Adress, Adress2, Zip, City, Phone, Mail, Home, Logo FROM user WHERE id = 1 LIMIT 1;");
	query.next();
	
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

/**
 * Méthode de callback appelée lors du clic sur le bouton de (dé)connexion
 * à la DB dans l'onglet "Paramètres".
 * On s'occupe ici d'appeler la méthode gérant la connexion à la DB et
 * de (dés)activer certains champs
 * 
 * @return void
 */
void QfactureImpl::on_aConnect_clicked()
{
	bool connexion_state = db.isOpen();
	
	QString Connected = QString(trUtf8("Connecté"));
	QString Disconnected = QString(trUtf8("Déconnecté"));
	QString Connect = QString(trUtf8("Connexion"));
	QString Disconnect = QString(trUtf8("Déconnexion"));
	
	if(!connexion_state) {
		// Se connecter à la base
		connexion_state = MySQL_connect();
		
		if(connexion_state) {
			aFlag->setText(Connected);
			aConnect->setText(Disconnect);
		}
	} else {
		// Se déconnecter de la base
		QString msg = QString(trUtf8("Voulez-vous réellement vous déconnecter du serveur?"));
		
		if(QMessageBox::warning(this, "Qfacture", msg, QMessageBox::Yes, QMessageBox::Cancel, QMessageBox::No) == QMessageBox::Yes) {
			aFlag->setText(Disconnected);
			aConnect->setText(Connect);
			
			db.close();
		}
	}
	
	connexion_state = db.isOpen();
	
	uSave->setEnabled(connexion_state);
	uGroupBox->setEnabled(connexion_state);
	aServer->setEnabled(!connexion_state);
	aPort->setEnabled(!connexion_state);
	aUser->setEnabled(!connexion_state);
	aPass->setEnabled(!connexion_state);
	aDb->setEnabled(!connexion_state);
}

void QfactureImpl::on_aPass_returnPressed()
{
	// Password saisi
	
	on_aConnect_clicked();
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
		"	Zip = :zip, City = :city, Phone = :phone, Mail = :mail, Home = :home "
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

/**
 * Méthode appelée lors du clic sur le bouton d'ajout d'un nouveau
 * client.
 * On ne fait qu'activer les champs permettant de saisir les informations
 * sur le client
 *
 * @return void
 */
void QfactureImpl::on_cNew_clicked()
{
	cGroupBox->setEnabled(true);
	cSave->setEnabled(true);
	cDel->setEnabled(false);
	cId->setEnabled(false);
    
	cId->setText(QString(""));
	cName->setText(QString(""));
	cAdress->setText(QString(""));
	cAdress2->setText(QString(""));
	cZip->setText(QString(""));
	cCity->setText(QString(""));
	cPhone->setText(QString(""));
	cMail->setText(QString(""));
}

/**
 * Méthode appelée lors du clic sur le bouton de sauvegarde d'un client.
 * On réalise l'enregistrement d'un nouveau client si le champ de l'ID
 * est vide, ou la mise à jour des informations d'un client déjà existant
 * dans le cas contraire.
 * 
 * \todo Documenter les vérifications effectuées par la méthode
 * 
 * @return void
 */
void QfactureImpl::on_cSave_clicked()
{
    QSqlQuery query;
    
    // Nouveau client
	if (cId->text().isEmpty())
        query.prepare(
            "INSERT INTO client(Name, Adress, Adress2, Zip, City, Phone, Mail) "
            "VALUES(:name, :adress, :adress2, :zip, :city, :phone, :mail)"
        );
    else { 
        // Client existant (modification instance)
        
        query.prepare(
            "UPDATE client "
            "SET Name = :name, Adress = :adress, Adress2 = :adress2, Zip = :zip, City = :city, Phone = :phone, Mail = :mail "
            "WHERE Id = :id "
        );
        
        query.bindValue(":id", cId->text());
	}
    
    query.bindValue(":name", cName->text());
    query.bindValue(":adress", cAdress->text());
    query.bindValue(":adress2", cAdress2->text());
    query.bindValue(":zip", cZip->text());
    query.bindValue(":city", cCity->text());
    query.bindValue(":phone", cPhone->text());
    query.bindValue(":mail", cMail->text());
    
    query.exec();
    
    if(!cId->text().isEmpty())
        statusbar->showMessage(trUtf8("Les modifications ont été enregistrées avec succès."), 3000);
    else {
        // dans le cas d'un nouveau client, on récupère aussi son ID
        cId->setText(query.lastInsertId().toString());
        
        statusbar->showMessage(trUtf8("Le nouveau client a été enregistré avec succès."), 3000);
    }
    
    cSave->setEnabled(true);
    cDel->setEnabled(true);
    
    emit clientSaved();
}

/**
 * Recharge la liste des clients.
 * 
 * @return void
 */
void QfactureImpl::cListRefresh()
{
    QSqlQuery query;
    QString item;
	
	cList->clear();
	
	query.prepare(
		"SELECT Id, Name, Adress, Adress2, Zip, City, Phone, Mail "
		"FROM client "
		"ORDER BY Name"
	);
    
	query.exec();
    
	while (query.next()) {
		item = query.value(1).toString() 
			+ QString(trUtf8(" | ")) + query.value(2).toString()
			+ QString(trUtf8(" ")) + query.value(3).toString()
			+ QString(trUtf8(" ")) + query.value(4).toString()
			+ QString(trUtf8(" ")) + query.value(5).toString()
			+ QString(trUtf8(" - ")) + query.value(6).toString()
			+ QString(trUtf8(" - ")) + query.value(7).toString()
			;
        
		cList->addItem(item);
        
        // chaque élément de la liste connait ainsi l'ID du client qu'il représente
        cList->item(cList->count()-1)->setData(Qt::UserRole, query.value(0).toInt());
	}
    
	query.finish();
}

/**
 * Affiche les infos d'un client dans le formulaire d'édition au clic
 * sur ce dernier dans la liste
 * 
 * @param item Pointeur vers la ligne représentant le client
 * 
 * @return void
 */
void QfactureImpl::on_cList_itemClicked(QListWidgetItem* item)
{
	QSqlQuery query;
    
	query.prepare(
		"SELECT Id, Name, Adress, Adress2, Zip, City, Phone, Mail "
		"FROM client "
		"WHERE Id = :id"
	);
	
    query.bindValue(":id", item->data(Qt::UserRole).toInt());
	query.exec();
	
    query.next();
    
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
    
	query.finish();
}

/**
 * Supprime le client actuellement sélectionné dans la liste
 * 
 * @return void
 */
void QfactureImpl::on_cDel_clicked()
{
    QSqlQuery query;
	
	QString msg = QString(trUtf8("Voulez-vous supprimer le client sélectionné ?\n\n"));
	if(QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) 
        return;
    
    query.prepare("DELETE FROM client WHERE Id = :id");
    query.bindValue(":id", cId->text());
    query.exec();
    query.finish();
    
    cId->setText(QString(""));
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
    
    emit clientDeleted();
    
    statusbar->showMessage(trUtf8("Le client a été supprimé avec succès."), 3000);
}

/* Tab Articles **************************************************************/

/**
 * Méthode callback appelée lors d'un clic sur le bouton d'ajout d'un
 * article
 * 
 * @return void
 */
void QfactureImpl::on_aNew_clicked()
{
    artGroupBox->setEnabled(true);
	aSave->setEnabled(true);
	aDel->setEnabled(false);
	aId->setEnabled(false);
    
	aId->setText(QString(""));
	aName->setText(QString(""));
	aPrice->setText(QString("0.00"));
	aCom->setText(QString(""));
}

/**
 * Méthode appelée lors du clic sur le bouton de sauvegarde d'un article.
 * On réalise l'enregistrement d'un nouvel article si le champ de l'ID
 * est vide, ou la mise à jour des informations d'un article déjà existant
 * dans le cas contraire.
 * 
 * \todo Documenter les vérifications effectuées par la méthode
 * 
 * @return void
 */
void QfactureImpl::on_aSave_clicked()
{
    QSqlQuery query;
    QString msg = QString(trUtf8("Le nom d'article saisi existe déjà dans la base !"));
    int nb;
    
	// Enregistrer nouvel article
	if(aId->text().isEmpty()) {
        // on vérifie qu'il n'existe pas déjà un article ayant la même désignation
		query.prepare("SELECT 1 FROM article WHERE Name = :name");
		query.bindValue(":name", aName->text());
        
		query.exec();
        
		nb = query.size();
        
		query.finish();
		
        if (nb > 0) {
			QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Ok);
            
            return;
		}
        
        query.prepare(
            "INSERT INTO article(Name, Price, Comment) "
            "VALUES(:name, :price, :comment)"
        );
	} else {
		// on vérifie qu'il n'existe pas déjà un article ayant la même désignation
		query.prepare("SELECT 1 FROM article WHERE Name = :name AND Id != :id");
		query.bindValue(":id", aId->text());
        query.bindValue(":name", aName->text());
        
		query.exec();
        
		nb = query.size();
        
		query.finish();
		
        if (nb > 0) {
			QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Ok);
            
            return;
		}
		
        query.prepare(
            "UPDATE article "
            "SET Name = :name, Price = :price, Comment = :comment "
            "WHERE Id = :id "
        );
        
        query.bindValue(":id", aId->text());
	}
    
    query.bindValue(":name", aName->text());
    query.bindValue(":price", aPrice->text());
    query.bindValue(":comment", aCom->text());
    
    query.exec();
    
    if(!aId->text().isEmpty())
        statusbar->showMessage(trUtf8("Les modifications ont été enregistrées avec succès."), 3000);
    else {
        aId->setText(query.lastInsertId().toString());
        
        statusbar->showMessage(trUtf8("Le nouvel article a été enregistré avec succès."), 3000);
    }
    
    query.finish();
    
    aSave->setEnabled(true);
    aDel->setEnabled(true);
    
    emit articleSaved();
}

/**
 * Supprime l'article actuellement sélectionné dans la liste
 * 
 * @return void
 */
void QfactureImpl::on_aDel_clicked()
{
    QSqlQuery query;
	
	QString msg = QString(trUtf8("Voulez-vous supprimer l'article sélectionné ?\n\n"));
	if(QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;
    
    query.prepare("DELETE FROM article WHERE Id = :id");
    query.bindValue(":id", aId->text());
    query.exec();
    query.finish();
    
    aId->setText(QString(""));
    aName->setText(QString(""));
    aPrice->setText(QString("0.00"));
    aCom->setText(QString(""));
    aDel->setEnabled(false);
    aNew->setEnabled(true);
    aSave->setEnabled(false);
    
    statusbar->showMessage(trUtf8("L'article a été supprimé avec succès."), 3000);
    
    emit articleDeleted();
}

/**
 * Recharge la liste des articles.
 * 
 * @return void
 */
void QfactureImpl::aListRefresh()
{
    QSqlQuery query;
    QString item;
    
	aList->clear();
    
	query.prepare(
		"SELECT Id, Name, Price, Comment "
		"FROM article "
		"ORDER BY Name"
	);
    
	query.exec();
    
    while (query.next()) {
		item = query.value(1).toString() 
			+ QString(trUtf8(" | ")) + query.value(2).toString()
			+ QString(trUtf8("€ (")) + query.value(3).toString()
			+ QString(trUtf8(")"));
        
		aList->addItem(item);
        
        // chaque élément de la liste connait ainsi l'ID de l'article qu'il représente
        aList->item(aList->count()-1)->setData(Qt::UserRole, query.value(0).toInt());
	}
	
    query.finish();
}

/**
 * Affiche les infos d'un article dans le formulaire d'édition au clic
 * sur ce dernier dans la liste
 * 
 * @param item Pointeur vers la ligne représentant l'article
 * 
 * @return void
 */
void QfactureImpl::on_aList_itemClicked(QListWidgetItem* item)
{
	QSqlQuery query;
    
	query.prepare(
		"SELECT Id, Name, Price, Comment "
		"FROM article "
		"WHERE Id = :id"
	);
	
    query.bindValue(":id", item->data(Qt::UserRole).toInt());
	query.exec();
    
	query.next();
    
    artGroupBox->setEnabled(true);
    aSave->setEnabled(true);
    aDel->setEnabled(true);
    aId->setEnabled(false);
	
    aId->setText(query.value(0).toString());
    aName->setText(query.value(1).toString());
    aPrice->setText(query.value(2).toString());
    aCom->setText(query.value(3).toString());
	
	query.finish();
}

/* Tab facture - liste des factures ******************************************/

/**
 * Recharge la liste des factures.
 * 
 * @return void
 */
void QfactureImpl::fListRefresh()
{
    QSqlQuery query;
    
	fList->clear();
    
	query.prepare(
		"SELECT "
		"	f.id, f.Amount, f.Comment, f.Payment, f.Reference, f.Type, f.Date, "
		"	c.Name "
		"FROM facture AS f "
		"LEFT JOIN client AS c "
		"	ON f.idClient = c.id "
		"ORDER BY Reference DESC"
	);
	query.exec();
	
    while (query.next()) {
		QString Item = query.value(0).toString() 
			+ QString(trUtf8(" | ")) + query.value(4).toString()
			+ QString(trUtf8(" | ")) + query.value(5).toString()
			+ QString(trUtf8(" | ")) + query.value(6).toString()
			+ QString(trUtf8(" | ")) + query.value(7).toString()
			+ QString(trUtf8(" | ")) + query.value(1).toString()
			+ QString(trUtf8("€ (")) + query.value(3).toString()
			+ QString(trUtf8(")"));
        
		fList->addItem(Item);
        
        // chaque élément de la liste connait ainsi l'ID de l'article qu'il représente
        fList->item(fList->count()-1)->setData(Qt::UserRole, query.value(0).toInt());
	}
    
	query.finish();
}

/**
 * Affiche les infos d'une facture dans le formulaire d'édition au clic
 * sur cette dernière dans la liste.
 * 
 * @param item Pointeur vers la ligne représentant la facture
 * 
 * @return void
 */
void QfactureImpl::on_fList_itemDoubleClicked(QListWidgetItem* item)
{
	QSqlQuery query;
    QString id_client;
    
	query.prepare(
		"SELECT f.Id, IdClient, Name, Amount, Comment, Payment, Reference, Type, Date "
		"FROM facture f "
        "LEFT JOIN client c "
        "ON c.Id = f.IdClient "
		"WHERE f.Id = :Id"
	);
    
	query.bindValue(":id", item->data(Qt::UserRole).toInt());
	query.exec();
    
    query.next();
	
    fNum->setText(query.value(0).toString());
    fDate->setDate(query.value(8).toDate());
    fMontant->setText(query.value(3).toString());
    fClient->setText(query.value(2).toString());
	
    // on sélectionne le mode de paiement
    fRegl->setCurrentIndex(fRegl->findText(query.value(5).toString(), Qt::MatchExactly));
    
    // on sélectionne le type de document
    fType->setCurrentIndex(fType->findText(query.value(7).toString(), Qt::MatchExactly));
    
    query.finish();
    
    fCalc->setEnabled(true);
    fSave->setEnabled(true);
    fPrint->setEnabled(true);
    fDel->setEnabled(true);
    fArticleGroup->setEnabled(true);
    fClientGroup->setEnabled(true);
    fDate->setEnabled(true);
    fType->setEnabled(true);
    fRegl->setEnabled(true);
	
	tabFacture->setCurrentIndex(0);
	
    fArtLinkRefresh();
	
    statusbar->showMessage(trUtf8("La facture a été chargée avec succès."), 3000);
}

/* Tab facture - éditer une facture ******************************************/

void QfactureImpl::fClientListRefresh()
{
    QSqlQuery query;
    QString item;
    
	// Rafraichit la liste des clients de l'onglet facture
	
	fClientList->clear();
    
	query.prepare(
		"SELECT Id, Name, Adress, Adress2, Zip, City, Phone, Mail "
		"FROM client "
		"ORDER BY Name"
	);
	query.exec();
	
    while (query.next()) {
		item = query.value(1).toString() 
			+ QString(trUtf8(" | ")) + query.value(2).toString()
			+ QString(trUtf8(" ")) + query.value(3).toString()
			+ QString(trUtf8(" ")) + query.value(4).toString()
			+ QString(trUtf8(" ")) + query.value(5).toString()
			+ QString(trUtf8(" - ")) + query.value(6).toString()
			+ QString(trUtf8(" - ")) + query.value(7).toString()
			;
		fClientList->addItem(item);
        
        // chaque élément de la liste connait ainsi l'ID du client qu'il représente
        fClientList->item(fClientList->count()-1)->setData(Qt::UserRole, query.value(0).toInt());
	}
    
	query.finish();
}

/**
 * Appelée lors du clic sur un client. Le choix du client pour la facture
 * est alors réalisé et le champ fClient est alimenté.
 * 
 * \todo se débrouiller pour stocker l'ID du membre et afficher le nom
 *       pour ne travailler qu'avec l'ID (unique) par la suite et non
 *       le nom (pas forcément unique :-°)
 * 
 * @param item Pointeur vers la ligne choisie.
 * 
 * @return void
 */
void QfactureImpl::on_fClientList_itemDoubleClicked(QListWidgetItem* item)
{
	QSqlQuery query;
    
	query.prepare(
		"SELECT Id, Name "
		"FROM client "
		"WHERE Id = :id "
	);
    
	query.bindValue(":id", item->data(Qt::UserRole).toInt());
    
	query.exec();
    
	query.next();
    
    fClient->setText(query.value(1).toString());
	
	query.finish();
	
    statusbar->showMessage(trUtf8("Client ajouté sur la facture."), 3000);
}

/**
 * Recharge la liste des articles de l'onglet facture.
 * 
 * @return void
 */
void QfactureImpl::fArtListRefresh()
{
    QString item;
	QSqlQuery query;
    
    fArtList->clear();
	
	query.prepare(
		"SELECT Id, Name, Price, Comment "
		"FROM article "
		"ORDER BY Name"
	);
    
	query.exec();
    
	while (query.next()) {
		item = query.value(1).toString() 
             + QString(trUtf8(" | ")) + query.value(2).toString()
			 + QString(trUtf8("€ (")) + query.value(3).toString()
			 + QString(trUtf8(")"));
        
		fArtList->addItem(item);
        
        // chaque élément de la liste connait ainsi l'ID de l'article qu'il représente
        fArtList->item(fArtList->count()-1)->setData(Qt::UserRole, query.value(0).toInt());
	}
    
	query.finish();
}

/**
 * Ajoute l'article sur lequel on vient de cliquer dans la facture en
 * cours d'édition
 * 
 * @param item Pointeur vers la ligne représentant l'article
 * 
 * @return void
 */
void QfactureImpl::on_fArtList_itemDoubleClicked(QListWidgetItem* item)
{
	QSqlQuery query;
    QString art_id, art_price;
    
    art_id = item->data(Qt::UserRole).toString();
	
    query.prepare("SELECT Id, Price FROM article WHERE Id = :id");
	query.bindValue(":id", art_id);
	query.exec();
	
    query.next();
	
	art_price = query.value(1).toString();
    
	query.finish();
    
	query.prepare(
		"INSERT INTO link(IdFacture, IdArticle, Quantity, Off, Price, Amount) "
		"VALUES (:num, :art, 1, 0, :price, :amount)"
	);
    
	query.bindValue(":num", fNum->text());
	query.bindValue(":art", art_id);
	query.bindValue(":price", art_price);
	query.bindValue(":amount", art_price);
	
    query.exec();
	
    query.finish();
    
    emit factureArticlesUpdated();
    
	statusbar->showMessage(trUtf8("Article ajouté sur la facture."), 3000);
}

void QfactureImpl::fArtLinkRefresh()
{
	// Rafraichit la liste des liens de l'onglet facture
	
	fFlag = false; // verrou refresh facture activé
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
	fFlag = true; // verrou refresh facture non activé
}

void QfactureImpl::on_fArtLink_itemChanged(QTableWidgetItem* Item)
{
	// Une cellule du tableau lien article est modifiée
	
	if (fFlag) {
		// verrou refresh facture non activé
		int Row = Item->row();
		int Col = Item->column();
		QString id = fArtLink->item(Row, 0)->text();
		QSqlQuery query;
		int quantity;
		float price, off, amount, amount_bis;
		switch (Col)
		{
			case 3: 
			// Quantité mise à jour
			quantity = Item->text().toInt();
			if (quantity != 0) {
				price = fArtLink->item(Row, 2)->text().toFloat();
				off = fArtLink->item(Row, 4)->text().toFloat();
				amount = fArtLink->item(Row, 5)->text().toFloat();
				amount_bis = quantity * price * (1 - off / 100);
				if (amount != amount_bis) {
					query.prepare(
						"UPDATE link "
						"SET amount = :Amount, quantity = :Quantity "
						"WHERE id = :Id"
					);
					query.bindValue(":Amount", amount_bis); 
					query.bindValue(":Quantity", quantity); 
					query.bindValue(":Id", id);
					query.exec();
					query.finish();
				}
                
				statusbar->showMessage(trUtf8("La quantité a été modifiée avec succés."), 3000);
			} else {
				// Quantité = 0 => suppression de la ligne
				query.prepare("DELETE FROM link WHERE id = :Id");
				query.bindValue(":Id", id);
				query.exec();
				query.finish();
				fArtLinkRefresh();
				on_fCalc_clicked();
				statusbar->showMessage(trUtf8("La ligne de la facture a été supprimée avec succés."), 3000);
			}
			break;
			case 4: 
			// Remise mise à jour
			off = Item->text().toInt();
			price = fArtLink->item(Row, 2)->text().toFloat();
			quantity = fArtLink->item(Row, 3)->text().toInt();
			amount = fArtLink->item(Row, 5)->text().toFloat();
			amount_bis = quantity * price * (1 - off / 100);
			fFlag = false;
			if (amount != amount_bis) {
				query.prepare(
					"UPDATE link "
					"SET amount = :Amount, off = :Off "
					"WHERE id = :Id"
				);
				query.bindValue(":Amount", amount_bis); 
				query.bindValue(":Off", off); 
				query.bindValue(":Id", id);
				query.exec();
				query.finish();
			}
			fFlag = true;
            
			statusbar->showMessage(trUtf8("La remise a été modifiée avec succés."), 3000);
			break;
			default:
			statusbar->showMessage(trUtf8("Cette cellule n'est pas modifiable."), 3000);
			break;
		}
        
        emit factureArticlesUpdated();
	}
}

/**
 * Méthode callback appelée lors du clic sur le bouton de calcul du
 * montant de la facture.
 * 
 * \note Ne devrait plus exister (en principe le montant est automatiquement
 *       mis à jour)
 * 
 * @return void
 */
void QfactureImpl::on_fCalc_clicked()
{
	// Bouton calculer le montant total de la facture
	
    fUpdateAmount();
	
	//fArtLinkRefresh();
}

/**
 * Met à jour le montant total de la facture en cours
 * 
 * @return void
 */
void QfactureImpl::fUpdateAmount()
{
    QSqlQuery query;
    
	query.prepare("SELECT SUM(Amount) FROM link WHERE IdFacture = :id");
	query.bindValue(":id", fNum->text());
	query.exec();
	
    query.next();
	
    fMontant->setText(query.value(0).toString());
	
    query.finish();
}

/**
 * Méthode appelée lors du clic sur le bouton de sauvegarde d'une facture.
 * On réalise l'enregistrement d'une nouvelle facture si le champ du numéro
 * est vide, ou la mise à jour des informations d'une facture déjà éditée
 * dans le cas contraire.
 * 
 * \todo Documenter les vérifications effectuées par la méthode
 * 
 * @return void
 */
void QfactureImpl::on_fSave_clicked()
{
	QSqlQuery query;
    QString client_id, count;
    
	if(fClient->text().isEmpty() or fDate->text().isEmpty()) {
		QMessageBox::warning(this, "Qfacture",
                             QString(trUtf8("Tous les champs ne sont pas renseignés!")),
                             QMessageBox::Ok);
        
        return;
    }
    
    // récupération de l'id du client
    query.prepare(
        "SELECT Id "
        "FROM client "
        "WHERE Name = :name "
    );
    
    query.bindValue(":name", fClient->text());
    
    query.exec();
    query.next();
    
    client_id = query.value(0).toString();
    
    query.finish();
    
    // Nouvelle facture
    if(fNum->text().isEmpty()) {
        
        // récupération du numéro de la facture
        query.prepare("SELECT COUNT(1)+1 FROM facture WHERE Date = :date ");
        query.bindValue(":date", dateToDB(fDate));
        query.exec();
        query.next();
        
        count = query.value(0).toString();
        
        query.finish();
        
        query.prepare(
            "INSERT INTO facture ( "
            "	idclient, "
            "	Amount, "
            //"	Comment, " non implémenté
            "	Payment, "
            "	Reference, "
            "	Type, "
            "	Date  "
            ") VALUES ("
            "	:client, "
            "	:amount, "
            //"	NULL, " non implémenté
            "	:pay, "
            "	:ref, "
            "	:type, "
            "	:date "
            ")"
        );
        
        query.bindValue(":ref", makeFactureReference(count, fDate->text()));
    } else {
        // Mise à jour facture existante
        
        query.prepare(
            "UPDATE facture "
            "SET idclient = :client, "
            "	Amount = :amount, "
            //"	Comment = NULL, " non implémenté
            "	Payment = :pay, "
            //"	Reference = :ref, "
            "	Type = :type, "
            "	Date = :date "
            "WHERE Id = :id"
        );
        
        query.bindValue(":id", fNum->text());
    }
    
    // on bind les valeurs pour la requête de la facture
    query.bindValue(":client", client_id);
    query.bindValue(":amount", fMontant->text());
    query.bindValue(":pay", fRegl->currentText());
    query.bindValue(":type", fType->currentText());
    query.bindValue(":date", dateToDB(fDate));
    
    query.exec();
    
    // la facture est enregistrée
    if(!fNum->text().isEmpty())
        statusbar->showMessage(trUtf8("Les modifications sur la facture sont bien enregistrées."), 3000);
    else {
        // pour les nouvelles factures
        
        fNum->setText(query.lastInsertId().toString());
        
        query.finish();
        
        statusbar->showMessage(trUtf8("La nouvelle facture est bien enregistrée."), 3000);
        
        query.prepare("UPDATE link SET IdFacture = :id WHERE IdFacture = 0");
        query.bindValue(":id", fNum->text());
        
        query.exec();
    }
    
    query.finish();
    
    emit factureSaved();
}

void QfactureImpl::on_fPrint_clicked()
{
	// Bouton imprimer la facture
	
	// TO DO : générer fichier pdf et l'ouvrir dans le lecteur pdf (xdg-open ?)
    // -> pas d'ouverture auto du pdf (puis xdg-open c'est pas portable =p)
}

/**
 * Supprime la facture actuellement sélectionnée dans la liste (ou plutôt
 * en cours d'édition)
 * 
 * @return void
 */
void QfactureImpl::on_fDel_clicked()
{
    QSqlQuery query;
	
	QString msg = QString(trUtf8("Voulez-vous supprimer la facture sélectionnée ?\n\n"));
	if(QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;
    
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
    
    fNum->setText(QString(trUtf8("")));
    fDate->setDate(QDate::currentDate());
    fMontant->setText("");
    fClient->setText("");
    
    statusbar->showMessage(trUtf8("La facture a bien été supprimée."), 3000);
    
    emit factureDeleted();
}

/**
 * Méthode de callback appelée lors du clic sur le bouton de création
 * d'une nouvelle facture.
 * 
 * @return void
 */
void QfactureImpl::on_fNew_clicked()
{
    fNum->setText(QString(""));
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

/* Tab stats - Statistiques **************************************************/

void QfactureImpl::on_sListCa_itemDoubleClicked(QListWidgetItem* item)
{
	// TODO
}

bool QfactureImpl::sListCaRefresh()
{
	// TODO
	
	return true;
}

void QfactureImpl::on_sYearCa_textChanged(QString )
{
	// TODO
	
	sListCaRefresh();
}

void QfactureImpl::on_sYearCa_editingFinished()
{
	// TODO
	
	sListCaRefresh();
}

void QfactureImpl::on_sYearCa_lostFocus()
{
	// TODO
	
	sListCaRefresh();
}
