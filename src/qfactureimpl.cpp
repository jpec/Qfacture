#include <QMessageBox>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileDialog>
#include <QListWidget>
#include <QTableWidget>
#include <QDate>
#include "qfactureimpl.h"

//
QfactureImpl::QfactureImpl(QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
    setupUi(this);
	
    VERSION = trUtf8("v0.1-alpha");
    
    db = QSqlDatabase::addDatabase("QMYSQL");
    
    settings = new QSettings("", "Qfacture");
    
    createActions();
    readSettings();
}

QfactureImpl::~QfactureImpl()
{
    delete settings;
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
void QfactureImpl::MySQL_connect()
{
    db.setHostName(aServer->text());
    db.setPort(aPort->text().toInt());
    db.setUserName(aUser->text());
    db.setPassword(aPass->text());
    db.setDatabaseName(aDb->text());
	
    if(db.open()) {
        emit DBConnected();
        
        return;
    }
    
    QMessageBox::warning(this, "Qfacture", trUtf8("Une erreur est survenue lors de la connexion : %1").arg(db.lastError().databaseText()),
                         QMessageBox::Ok);
}

/**
 * Affiche une boite de dialogue permettant de confirmer ou non la
 * réalisation d'une action. La boite de dialogue est composée d'un
 * message (passé en paramètre) et de deux boutons : Oui ou Non.
 * 
 * @param msg Message à afficher
 * 
 * @return bool
 */
bool QfactureImpl::confirm(const char *msg)
{
    return (QMessageBox::question(this, "Qfacture", trUtf8(msg),
            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes);
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

/**
 * Sauvegardes les paramètres de l'appli
 */
void QfactureImpl::writeSettings()
{
    // infos sur la fenêtre (taille et position)
    settings->beginGroup("Window");
    settings->setValue("size", size());
    settings->setValue("pos", pos());
    settings->endGroup();
    
    // infos de connexion à la DB
    settings->beginGroup("DB");
    settings->setValue("host", aServer->text());
    settings->setValue("port", aPort->text().toInt());
    settings->setValue("user", aUser->text());
    settings->setValue("passwd", aPass->text());
    settings->setValue("db_name", aDb->text());
    settings->endGroup();
}

/**
 * Lit les paramètres de l'appli
 */
void QfactureImpl::readSettings()
{
    // infos sur la fenêtre (taille et position)
    settings->beginGroup("Window");
    resize(settings->value("size", QSize(400, 400)).toSize());
    move(settings->value("pos", QPoint(200, 200)).toPoint());
    settings->endGroup();
    
    // infos de connexion à la DB
    settings->beginGroup("DB");
    aServer->setText(settings->value("host", "localhost").toString());
    aPort->setText(settings->value("port", 3306).toString());
    aUser->setText(settings->value("user", "qfacture").toString());
    aPass->setText(settings->value("passwd", "").toString());
    aDb->setText(settings->value("db_name", "qfacture").toString());
    settings->endGroup();
}

/**
 * Réalise les actions nécessaires à la fermeture de l'appli (clore la
 * connexion à la DB, stocker les préférences, etc.) et ferme l'appli.
 */
void QfactureImpl::doQuit()
{
    writeSettings();
    
    db.close();
    qApp->quit();
}


/* Menu **********************************************************************/

/**
 * Méthode appelée lorsque l'utilisateur demande à quitter l'application
 * (via le menu)
 */
void QfactureImpl::on_action_Quitter_triggered()
{
    if(!confirm("Voulez vous quitter l'application ?"))
        return;
    
    doQuit();
}

void QfactureImpl::on_action_propos_activated()
{
    /** À propos **/

    QString msg = trUtf8("Qfacture :VERSION\n"
                    "Le logiciel libre de facturation pour les Auto-Entrepreneurs!\n"
			        "--\n"
			        "Copyright 2010 : Julien PECQUEUR\n"
			        "Licence : GPL\n"
			        "Auteur : Julien PECQUEUR <jpec@julienpecqueur.com>\n"
			        "--\n"
			        "Contributeur(s) :\n"
			        " * Module génération PDF : Kévin GOMEZ <contact@kevingomez.fr>\n"
			        " * Logo : Kevin MACPHAIL <http://kmacphail.blogspot.com>\n"
			        "\n").replace(":VERSION", VERSION);
    
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
    
    query.exec(
        "SELECT Name, Siret, Adress, Adress2, Zip, City, Phone, Mail, Home, Logo "
        "FROM user WHERE id = 1 LIMIT 1;"
    );
    query.next();

    /* Alimentation des widgets */
    uName->setText(query.value(0).toString());
    uSiret->setText(query.value(1).toString());
    uAdress->setText(query.value(2).toString());
    uAdress2->setText(query.value(3).toString());
    uZip->setText(query.value(4).toString());
    uCity->setText(query.value(5).toString());
    uPhone->setText(query.value(6).toString());
    uMail->setText(query.value(7).toString());
    uHome->setText(query.value(8).toString());
	
    /* Alimentation du widget logo */
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
	
    /* Connexion */
    if(!connexion_state)
        MySQL_connect();
    else {
        /* Déconnexion */
        
        if(!confirm("Voulez-vous réellement vous déconnecter du serveur?"))
            return;
        
        db.close();
    }

    connexion_state = db.isOpen();
    
    // mise à jour des widgets en fonction de l'état de la connexion
    aConnect->setText(connexion_state ? trUtf8("Déconnexion") : trUtf8("Connexion"));
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
    /** Mot de passe saisi */
	
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
    
    query.prepare(
		"UPDATE user "
		"SET Name = :name, Siret = :siret, Adress = :adress, Adress2 = :adress2, "
		"    Zip = :zip, City = :city, Phone = :phone, Mail = :mail, Home = :home "
		"WHERE id = 1"
    );
    
    query.bindValue(":name", uName->text());
    query.bindValue(":siret", uSiret->text());
    query.bindValue(":adress", uAdress->text());
    query.bindValue(":adress2", uAdress2->text());
    query.bindValue(":zip", uZip->text());
    query.bindValue(":city", uCity->text());
    query.bindValue(":phone", uPhone->text());
    query.bindValue(":mail", uMail->text());
    query.bindValue(":home", uHome->text());
    
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
	
    /* Sélection du logo */
    image = QFileDialog::getOpenFileName(this, trUtf8("Qfacture - Importer un logo..."),
                        "", trUtf8("Image Files (*.png *.jpg *.bmp)"));
    
    if(image.isNull())
        return;
    
    uLogo->text().clear();
    img_file.setFileName(image);
    
    if(!img_file.open(QIODevice::ReadOnly)) {
        uLogo->setText("Impossible d'ouvrir le fichier contenant le logo !");
        return;
    }
	
    /* Enregistrement de l'image dans la base SQL */
    query.prepare("UPDATE user SET Logo = :logo WHERE id = 1;");
    query.bindValue(":logo", img_file.readAll());
    query.exec();
	
    /* Affichage de l'image */
    img_pixmap.load(image);
    uLogo->setPixmap(img_pixmap);
	
    /* Fermeture du fichier */
    img_file.close();
    
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
    cId->clear();
    cName->clear();
    cAdress->clear();
    cAdress2->clear();
    cZip->clear();
    cCity->clear();
    cPhone->clear();
    cMail->clear();
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
    
    /* Nouveau client */
    if (cId->text().isEmpty())
        query.prepare(
		    "INSERT INTO client(Name, Adress, Adress2, Zip, City, Phone, Mail) "
            "VALUES(:name, :adress, :adress2, :zip, :city, :phone, :mail)"
        );
    else { /* Mise à jour d'un client */
        query.prepare(
            "UPDATE client "
            "SET"
            " Name = :name, Adress = :adress, Adress2 = :adress2,"
            " Zip = :zip, City = :city, Phone = :phone, Mail = :mail "
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
        /* Dans le cas d'un nouveau client, on récupère aussi son ID */
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
    
    while(query.next()) {
        item = query.value(1).toString() 
             + trUtf8(" - ") + query.value(2).toString()
             + trUtf8(" ") + query.value(3).toString()
             + trUtf8(" ") + query.value(4).toString()
             + trUtf8(" ") + query.value(5).toString()
             + trUtf8(" - ") + query.value(6).toString()
             + trUtf8(" - ") + query.value(7).toString();
    
        cList->addItem(item); 
        
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
    
    if(!confirm("Voulez-vous supprimer le client sélectionné ?"))
        return;
    
    query.prepare("DELETE FROM client WHERE Id = :id");
    query.bindValue(":id", cId->text());
    query.exec();
    query.finish();
    
    cId->clear();
    cName->clear();
    cAdress->clear();
    cAdress2->clear();
    cZip->clear();
    cCity->clear();
    cPhone->clear();
    cMail->clear();
    
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
    
    aId->clear();
    aName->clear();
    aPrice->setText(QString("0.00"));
    aCom->clear();
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
    
    aId->clear();
    aName->clear();
    aPrice->setText(QString("0.00"));
    aCom->clear();
    
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
    
    while(query.next()) {
        item = query.value(1).toString() 
            + trUtf8(" - ") + query.value(2).toString()
            + QString(trUtf8("€ (")) + query.value(3).toString()
            + QString(trUtf8(")"));
    
        aList->addItem(item);
    
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
    QString item;
    
    fList->clear();
    
    query.prepare(
		"SELECT f.id, f.Amount, f.Comment, f.Payment, f.Reference, "
		"         f.Type, f.Date, c.Name "
		"FROM facture AS f "
		"LEFT JOIN client AS c "
		"	ON f.idClient = c.id "
		"ORDER BY Reference DESC"
    );
    query.exec();
    
    while(query.next()) {
        item = query.value(0).toString() 
             + QString(trUtf8(" | ")) + query.value(4).toString()
             + QString(trUtf8(" | ")) + query.value(5).toString()
             + QString(trUtf8(" | ")) + query.value(6).toString()
             + QString(trUtf8(" | ")) + query.value(7).toString()
             + QString(trUtf8(" | ")) + query.value(1).toString()
             + QString(trUtf8("€ (")) + query.value(3).toString()
             + QString(trUtf8(")"));
    
        fList->addItem(item);
        
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
    fRegl->setCurrentIndex(fRegl->findText(query.value(5).toString(), Qt::MatchExactly));
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

/**
 * Recharge la liste des clients de l'onglet facture
 * 
 * @return void
 */
void QfactureImpl::fClientListRefresh()
{
    QSqlQuery query;
    QString item;
    
	fClientList->clear();
    
	query.prepare(
		"SELECT Id, Name, Adress, Adress2, Zip, City, Phone, Mail "
		"FROM client "
		"ORDER BY Name"
	);
	query.exec();
	
    while(query.next()) {
		item = query.value(1).toString() 
			+ QString(trUtf8(" | ")) + query.value(2).toString()
			+ trUtf8(" ") + query.value(3).toString()
			+ trUtf8(" ") + query.value(4).toString()
			+ trUtf8(" ") + query.value(5).toString()
			+ trUtf8(" - ") + query.value(6).toString()
			+ trUtf8(" - ") + query.value(7).toString();
		
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
 *         pour ne travailler qu'avec l'ID (unique) par la suite et non
 *         le nom (pas forcément unique :-°)
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
             + trUtf8(" - ") + query.value(2).toString()
             + trUtf8("€ (") + query.value(3).toString()
             + trUtf8(")");
    
        fArtList->addItem(item);
    
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

/**
 * Recharge la liste des liens de l'onglet facture
 */
void QfactureImpl::fArtLinkRefresh()
{
    QSqlQuery query;
    
	fArtLink->clearContents();
	
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
	while(query.next()) {
		// id
		fArtLink->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
		
        // name
		fArtLink->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
		
        // price
		fArtLink->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
		
        // nbr
		fArtLink->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
		
        // off
		fArtLink->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
		
        // mont 
		fArtLink->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
		
        i++;
	}
    
	query.finish();
}

void QfactureImpl::on_fArtLink_itemChanged(QTableWidgetItem* Item)
{
    /** Une cellule du tableau lien article est modifiée **/
	
    int Row = Item->row();
    int Col = Item->column();
    QString id = fArtLink->item(Row, 0)->text();
    QSqlQuery query;
    int quantity;
    float price, off, amount, amount_bis;
    switch (Col)
    {
        case 3: 
	/* Quantité mise à jour */
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
	    /* Quantité = 0 => suppression du lien */
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
	/* Remise mise à jour */
	off = Item->text().toInt();
	price = fArtLink->item(Row, 2)->text().toFloat();
	quantity = fArtLink->item(Row, 3)->text().toInt();
	amount = fArtLink->item(Row, 5)->text().toFloat();
	amount_bis = quantity * price * (1 - off / 100);
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
	statusbar->showMessage(trUtf8("La remise a été modifiée avec succés."), 3000);
	break;
        default:
	statusbar->showMessage(trUtf8("Cette cellule n'est pas modifiable."), 3000);
	break;
    }
    
    emit factureArticlesUpdated();
}

/**
 * Méthode callback appelée lors du clic sur le bouton de calcul du
 * montant de la facture.
 * 
 * \note Ne devrait plus exister (en principe le montant est automatiquement
 *         mis à jour)
 * 
 * @return void
 */
void QfactureImpl::on_fCalc_clicked()
{
    /** Calcul le montant total de la facture **/
	
    fUpdateAmount();
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
    
    /* Id client - A REIMPLEMENTER AVEC ID CLIENT AU LIEU DU NOM */
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
    
    /* Nouvelle facture */
    if(fNum->text().isEmpty()) {
        /* Récupération du numéro de la facture */
        query.prepare("SELECT COUNT(1)+1 FROM facture WHERE Date = :date ");
        query.bindValue(":date", dateToDB(fDate));
        
        query.exec();
        query.next();
        
        count = query.value(0).toString();
        
        query.finish();
        
        query.prepare(
            "INSERT INTO facture"
            " (idclient, Amount, Payment, Reference, Type, Date) "
            "VALUES"
            " (:client, :amount, :pay, :ref, :type, :date)"
        );
        
        query.bindValue(":ref", makeFactureReference(count, fDate->text()));
    } else {
        /* Mise à jour d'une facture */
        query.prepare(
            "UPDATE facture "
            "SET"
            " idclient = :client, Amount = :amount, Payment = :pay, "
            " Type = :type, Date = :date "
            "WHERE Id = :id"
        );
        
        query.bindValue(":id", fNum->text());
    }
    
    query.bindValue(":client", client_id);
    query.bindValue(":amount", fMontant->text());
    query.bindValue(":pay", fRegl->currentText());
    query.bindValue(":type", fType->currentText());
    query.bindValue(":date", dateToDB(fDate)); 
    
    query.exec();
    
    /* Modification d'une facture */
    if(!fNum->text().isEmpty())
        statusbar->showMessage(trUtf8("Les modifications sur la facture sont bien enregistrées."), 3000);
    else {
        /* Nouvelle facture */
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
    /** Imprimer la facture **/
	
    /** TO DO : 
     * générer fichier pdf et l'ouvrir dans le lecteur pdf (xdg-open ?)
     * -> pas d'ouverture auto du pdf (puis xdg-open c'est pas portable =p)
     */
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
    
    // on demande confirmation
    if(!confirm("Voulez-vous supprimer la facture sélectionnée ?"))
        return;
    
    /* Suppression des liens */
    query.prepare("DELETE FROM link WHERE IdFacture = :id");
    query.bindValue(":id", fNum->text());
    query.exec();
    query.finish();
    
    /* Suppression de la facture */
    query.prepare("DELETE FROM facture WHERE Id = :id");
    query.bindValue(":id", fNum->text());
    query.exec();
    query.finish();
    
    /* Mise à jour des widgets */
    fCalc->setEnabled(false);
    fSave->setEnabled(false);
    fPrint->setEnabled(false);
    fDel->setEnabled(false);
    fArticleGroup->setEnabled(false);
    fClientGroup->setEnabled(false);
    fDate->setEnabled(false);
    fType->setEnabled(false);
    fRegl->setEnabled(false);
    
    fNum->clear();
    fDate->setDate(QDate::currentDate());
    fMontant->clear();
    fClient->clear();
    
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
    fNum->clear();
    fDate->setDate(QDate::currentDate());	
    fMontant->clear();
    
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

bool QfactureImpl::sListCaRefresh()
{
    QSqlQuery query;
    QString text;
    QString Year;
    
    Year = sYearCa->text();
    
    if(Year.isEmpty() or Year.length() != 4 or !(9999 >= Year.toInt())){
        Year = QString("2010");
        sYearCa->setText(Year);
    }
    sListCa->clear();
    query.prepare(
		"SELECT LEFT(date, 7) AS month, SUM(amount) AS sum "
		"FROM facture WHERE date LIKE :year GROUP BY month"
		);
    query.bindValue(":year", Year+QString("%"));
    statusbar->showMessage(query.executedQuery(), 5000);
    query.exec();

    while(query.next()){
        text = query.value(0).toString()
             + trUtf8(" : ")
             + query.value(1).toString()
             + trUtf8("€");
        sListCa->addItem(text);
    }

    query.finish();

    return true;
}

void QfactureImpl::on_sYearCa_lostFocus()
{
    /** Année changée **/
	
    sListCaRefresh();
}
