#include <QMessageBox>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QListWidget>
#include <QTableWidget>
#include <QDate>
#include <QWebView>
#include <QPrinter>
#include <QPrintDialog>
#include <QRegExp>

#include "qfactureimpl.h"

//
QfactureImpl::QfactureImpl(QWidget * parent, Qt::WFlags f) : QMainWindow(parent, f)
{
    setupUi(this);

    VERSION = trUtf8("v0.1.2");

    db = QSqlDatabase::addDatabase("QMYSQL");

    settings = new QSettings("", "Qfacture");

    clients_model = new EditableSqlModel(this, db);
    products_model = new EditableSqlModel(this, db);

    createActions();
    readSettings();
}

QfactureImpl::~QfactureImpl()
{
    delete settings;
    delete clients_model;
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
    connect(this, SIGNAL(DBConnected()), this, SLOT(refreshCustomersList()));
    connect(this, SIGNAL(DBConnected()), this, SLOT(refreshInvoiceCustomersList()));

    // rechargement de la liste des articles
    connect(this, SIGNAL(DBConnected()), this, SLOT(refreshProductsList()));
    connect(this, SIGNAL(DBConnected()), this, SLOT(refreshInvoiceProductsList()));

    // rechargement de la liste des factures
    connect(this, SIGNAL(DBConnected()), this, SLOT(refreshInvoicesList()));

    // rechargement des statistiques
    connect(this, SIGNAL(DBConnected()), this, SLOT(sListCaRefresh()));

    /** Actions effectuées lors de la sauvegarde d'un client (nouveau ou mise à jour d'un déjà existant) **/

    connect(this, SIGNAL(clientSaved()), this, SLOT(refreshCustomersList()));
    connect(this, SIGNAL(clientSaved()), this, SLOT(refreshInvoiceCustomersList()));

    /** Lorsqu'un client est sélectionné, on active la possibilité de suppression et l'édition **/

    connect(cList, SIGNAL(clicked(QModelIndex)), this, SLOT(onCustomerSelected(QModelIndex)));

    /** Actions effectuées lors de la suppression d'un client **/

    connect(this, SIGNAL(clientDeleted()), this, SLOT(refreshCustomersList()));
    connect(this, SIGNAL(clientDeleted()), this, SLOT(refreshInvoiceCustomersList()));

    /** Actions effectuées lors de la sauvegarde d'une facture **/

    connect(this, SIGNAL(factureSaved()), this, SLOT(fArtLinkRefresh()));
    connect(this, SIGNAL(factureSaved()), this, SLOT(refreshInvoicesList()));
    connect(this, SIGNAL(factureSaved()), this, SLOT(sListCaRefresh()));

    /** Actions effectuées lors de la suppression d'une facture **/

    connect(this, SIGNAL(factureDeleted()), this, SLOT(fArtLinkRefresh()));
    connect(this, SIGNAL(factureDeleted()), this, SLOT(refreshInvoicesList()));
    connect(this, SIGNAL(factureDeleted()), this, SLOT(sListCaRefresh()));

    /** Actions effectuées lors de l'ajout d'un article à une facture **/

    connect(this, SIGNAL(factureArticlesUpdated()), this, SLOT(fArtLinkRefresh()));
    connect(this, SIGNAL(factureArticlesUpdated()), this, SLOT(updateInvoiceAmount()));

    /** Actions effectuées lors de la sauvegarde d'un article **/

    connect(this, SIGNAL(articleSaved()), this, SLOT(refreshProductsList()));
    connect(this, SIGNAL(articleSaved()), this, SLOT(refreshInvoiceProductsList()));

    /** Lorsqu'un article est sélectionné, on active la possibilité de suppression et d'édition **/

    connect(aList, SIGNAL(clicked(QModelIndex)), this, SLOT(onProductSelected(QModelIndex)));

    /** Actions effectuées lors de la suppression d'un article **/

    connect(this, SIGNAL(articleDeleted()), this, SLOT(refreshProductsList()));
    connect(this, SIGNAL(articleDeleted()), this, SLOT(refreshInvoiceProductsList()));
}

/**
 * Réalise la connexion à la base de données MySQL
 *
 * @return bool Témoin du succès de la connexion
 */
void QfactureImpl::MySQLConnect()
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
    aDb->setText(settings->value("db_name", "qfacture_db").toString());
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

/**
 * Renvoie le template HTML à utiliser pour générer une facture
 *
 * @return QString Le TPL en vigueur
 */
QString QfactureImpl::getInvoiceHTMLTpl()
{
    QSqlQuery query;

    query.exec(
            "SELECT Content FROM template WHERE sel = \"x\" AND type = \"html\" LIMIT 1;"
            );
    query.next();

    return query.value(0).toString();
}


/**
 * Reemplace le template HTML à utiliser pour générer une facture
 *
 * @return QString Le nouveau TPL enregistré
 */
QString QfactureImpl::setInvoiceHTMLTpl(QString Tpl)
{
    QSqlQuery query;

    query.prepare(
            "UPDATE template SET Content = :t "
            "WHERE sel = \"x\" AND type = \"html\";"
            );
    query.bindValue(":tpl", Tpl);
    query.exec();
    return getInvoiceHTMLTpl();
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

    QString msg = trUtf8("Qfacture %1\n"
                         "Le logiciel libre de facturation pour les Auto-Entrepreneurs!\n"
                         "--\n"
                         "Copyright 2010 : Julien PECQUEUR\n"
                         "Licence : GPL\n"
                         "Auteur : Julien PECQUEUR <jpec@julienpecqueur.com>\n"
                         "--\n"
                         "Contributeur(s) :\n"
                         " * Kévin Gomez <contact@kevingomez.fr>\n"
                         "\n").arg(VERSION);

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
        MySQLConnect();
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
    fFlag = false;
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
            "	Zip = :zip, City = :city, Phone = :phone, Mail = :mail, Home = :home "
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
    cName->setFocus();
    cName->setEnabled(true);
    cAdress->clear();
    cAdress->setEnabled(true);
    cAdress2->clear();
    cAdress2->setEnabled(true);
    cZip->clear();
    cZip->setEnabled(true);
    cCity->clear();
    cCity->setEnabled(true);
    cPhone->clear();
    cPhone->setEnabled(true);
    cMail->clear();
    cMail->setEnabled(true);
}

/**
 * Affiche les infos d'un client dans le formulaire lors du clic sur ce
 * dernier
 *
 * @param item Pointeur vers la ligne représentant le client
 *
 * @return void
 */
void QfactureImpl::onCustomerSelected(const QModelIndex &item)
{
    QString id;
    QSqlQuery query;

    // récupération de l'id du client sélectionné
    id = item.sibling(item.row(), 0).data().toString();

    // récupération des infos du client
    query.prepare(
            "SELECT Name, Adress, Adress2, Zip, City, Phone, Mail FROM client "
            "WHERE Id = :id "
            );

    query.bindValue(":id", id);
    query.exec();
    query.next();

    cGroupBox->setEnabled(true);
    cSave->setEnabled(true);
    cDel->setEnabled(true);
    cId->setEnabled(false);

    cId->setText(id);
    cName->setText(query.value(0).toString());
    cName->setFocus();
    cName->setEnabled(true);
    cAdress->setText(query.value(1).toString());
    cAdress->setEnabled(true);
    cAdress2->setText(query.value(2).toString());
    cAdress2->setEnabled(true);
    cZip->setText(query.value(3).toString());
    cZip->setEnabled(true);
    cCity->setText(query.value(4).toString());
    cCity->setEnabled(true);
    cPhone->setText(query.value(5).toString());
    cPhone->setEnabled(true);
    cMail->setText(query.value(6).toString());
    cMail->setEnabled(true);
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

    cId->clear();
    cName->clear();
    cName->setEnabled(false);
    cAdress->clear();
    cAdress->setEnabled(false);
    cAdress2->clear();
    cAdress2->setEnabled(false);
    cZip->clear();
    cZip->setEnabled(false);
    cCity->clear();
    cCity->setEnabled(false);
    cPhone->clear();
    cPhone->setEnabled(false);
    cMail->clear();
    cMail->setEnabled(false);

    cDel->setEnabled(false);
    cNew->setEnabled(true);
    cSave->setEnabled(false);

    emit clientSaved();
}

/**
 * Recharge la liste des clients.
 *
 * @return void
 */
void QfactureImpl::refreshCustomersList()
{
    clients_model->clear();

    clients_model->setTable("client");

    clients_model->setHeaderData(0, Qt::Horizontal, trUtf8("ID"));
    clients_model->setHeaderData(1, Qt::Horizontal, trUtf8("Nom"));
    clients_model->setHeaderData(2, Qt::Horizontal, trUtf8("Adresse"));
    clients_model->setHeaderData(3, Qt::Horizontal, trUtf8("Complément"));
    clients_model->setHeaderData(4, Qt::Horizontal, trUtf8("Code postal"));
    clients_model->setHeaderData(5, Qt::Horizontal, trUtf8("Ville"));
    clients_model->setHeaderData(6, Qt::Horizontal, trUtf8("Téléphone"));
    clients_model->setHeaderData(7, Qt::Horizontal, trUtf8("Mail"));

    clients_model->select();

    cList->setModel(clients_model);

    // pour que la largeur des colonnes soit automatiquement mise à jour
    // pour s'accorder au contenu
    cList->resizeColumnsToContents();

    // pour que la largeur des colonnes soit automatiquement mise à jour
    // pour s'accorder à la taille de la fenêtre
    // cList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
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

    // temporaire
    if(!clients_model->removeRows(cList->currentIndex().row(), 1))
        QMessageBox::warning(this, "Qfacture", clients_model->lastError().databaseText());

    cId->clear();
    cName->clear();
    cName->setEnabled(false);
    cAdress->clear();
    cAdress->setEnabled(false);
    cAdress2->clear();
    cAdress2->setEnabled(false);
    cZip->clear();
    cZip->setEnabled(false);
    cCity->clear();
    cCity->setEnabled(false);
    cPhone->clear();
    cPhone->setEnabled(false);
    cMail->clear();
    cMail->setEnabled(false);

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
    aName->setFocus();
    aName->setEnabled(true);
    aPrice->setValue(0);
    aPrice->setEnabled(true);
    aCom->clear();
    aCom->setEnabled(true);
}

/**
 * Affiche les infos d'un article dans le formulaire lors du clic sur ce
 * dernier
 *
 * @param item Pointeur vers la ligne représentant l'article
 *
 * @return void
 */
void QfactureImpl::onProductSelected(const QModelIndex &item)
{
    QString id;
    QSqlQuery query;

    // récupération de l'id du produit sélectionné
    id = item.sibling(item.row(), 0).data().toString();

    // récupération des infos du produit
    query.prepare(
            "SELECT id, Name, Price, Comment FROM article "
            "WHERE id = :id "
            );

    query.bindValue(":id", id);
    query.exec();
    query.next();

    artGroupBox->setEnabled(true);
    aSave->setEnabled(true);
    aDel->setEnabled(true);
    aId->setEnabled(false);

    aId->setText(id);
    aName->setText(query.value(1).toString());
    aName->setFocus();
    aName->setEnabled(true);
    aPrice->setValue(query.value(2).toFloat());
    aPrice->setEnabled(true);
    aCom->setText(query.value(3).toString());
    aCom->setEnabled(true);
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
    query.bindValue(":price", aPrice->value());
    query.bindValue(":comment", aCom->text());

    query.exec();

    if(!aId->text().isEmpty())
        statusbar->showMessage(trUtf8("Les modifications ont été enregistrées avec succès."), 3000);
    else {
        aId->setText(query.lastInsertId().toString());

        statusbar->showMessage(trUtf8("Le nouvel article a été enregistré avec succès."), 3000);
    }

    query.finish();

    aId->clear();
    aName->clear();
    aName->setEnabled(false);
    aPrice->setValue(0);
    aPrice->setEnabled(false);
    aCom->clear();
    aCom->setEnabled(false);

    aDel->setEnabled(false);
    aNew->setEnabled(true);
    aSave->setEnabled(false);

    emit articleSaved();
}

/**
 * Supprime l'article actuellement sélectionné dans la liste
 *
 * @return void
 */
void QfactureImpl::on_aDel_clicked()
{
    QString msg = trUtf8("Voulez-vous supprimer l'article sélectionné ?");
    if(QMessageBox::warning(this, "Qfacture", msg , QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;

    // temporaire
    if(!products_model->removeRows(aList->currentIndex().row(), 1))
        QMessageBox::warning(this, "Qfacture", products_model->lastError().databaseText());

    aId->clear();
    aName->clear();
    aName->setEnabled(false);
    aPrice->setValue(0);
    aPrice->setEnabled(false);
    aCom->clear();
    aCom->setEnabled(false);

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
void QfactureImpl::refreshProductsList()
{
    products_model->clear();

    products_model->setTable("article");

    products_model->setHeaderData(0, Qt::Horizontal, trUtf8("ID"));
    products_model->setHeaderData(1, Qt::Horizontal, trUtf8("Nom"));
    products_model->setHeaderData(2, Qt::Horizontal, trUtf8("Prix"));
    products_model->setHeaderData(3, Qt::Horizontal, trUtf8("Commentaire"));

    products_model->select();

    aList->setModel(products_model);

    // pour que la largeur des colonnes soit automatiquement mise à jour
    // pour s'accorder au contenu
    aList->resizeColumnsToContents();

    // pour que la largeur des colonnes soit automatiquement mise à jour
    // pour s'accorder à la taille de la fenêtre
    // aList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

/* Tab facture - liste des factures ******************************************/

/**
 * Recharge la liste des factures.
 *
 * @return void
 */
void QfactureImpl::refreshInvoicesList()
{
    QSqlQuery query;

    fList->clearContents();

    query.prepare(
            "SELECT f.id, f.Amount, f.Comment, f.Payment, f.Reference, "
            "		 f.Type, f.Date, c.Name "
            "FROM facture AS f "
            "LEFT JOIN client AS c "
            "   ON f.idClient = c.id "
            "ORDER BY Reference DESC"
            );
    query.exec();

    fList->setRowCount(query.size());

    int i=0;
    while(query.next()) {
        fList->setItem(i, 0, new QTableWidgetItem(query.value(4).toString())); // ref
        fList->setItem(i, 1, new QTableWidgetItem(query.value(6).toString())); // date
        fList->setItem(i, 2, new QTableWidgetItem(query.value(7).toString())); // client
        fList->setItem(i, 3, new QTableWidgetItem(query.value(1).toString())); // montant
        fList->setItem(i, 4, new QTableWidgetItem(query.value(3).toString())); // paiement
        fList->setItem(i, 5, new QTableWidgetItem(query.value(5).toString())); // type
        fList->setItem(i, 6, new QTableWidgetItem(query.value(2).toString())); // commentaire

        fList->item(i, 0)->setData(Qt::UserRole, query.value(0).toInt());

        i++;
    }

    // pour que la largeur des colonnes soit automatiquement mise à jour
    // pour s'accorder au contenu
    fList->resizeColumnsToContents();

    // pour que la largeur des colonnes soit automatiquement mise à jour
    // pour s'accorder à la taille de la fenêtre
    // fList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

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
void QfactureImpl::on_fList_itemDoubleClicked(QTableWidgetItem* item)
{
    QSqlQuery query;
    int id;

    query.prepare(
            "SELECT f.Id, IdClient, Name, Amount, Comment, Payment, Reference, Type, Date "
            "FROM facture f "
            "LEFT JOIN client c "
            "ON c.Id = f.IdClient "
            "WHERE f.Id = :Id"
            );

    id = fList->item(item->row(), 0)->data(Qt::UserRole).toInt();

    query.bindValue(":Id", id);
    query.exec();
    query.next();

    fNum->setText(query.value(0).toString());
    fDate->setDate(query.value(8).toDate());
    fMontant->setText(query.value(3).toString());
    fClient->setText(query.value(2).toString());
    fRegl->setCurrentIndex(fRegl->findText(query.value(5).toString(), Qt::MatchExactly));
    fType->setCurrentIndex(fType->findText(query.value(7).toString(), Qt::MatchExactly));

    query.finish();

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
void QfactureImpl::refreshInvoiceCustomersList()
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
 *		 pour ne travailler qu'avec l'ID (unique) par la suite et non
 *		 le nom (pas forcément unique :-°)
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
void QfactureImpl::refreshInvoiceProductsList()
{
    QString item;
    QSqlQuery query;

    fArtList->clearContents();

    query.prepare(
            "SELECT Id, Name, Price, Comment "
            "FROM article "
            "ORDER BY Name"
            );

    query.exec();

    fArtList->setRowCount(query.size());

    int i=0;
    while(query.next()) {
        fArtList->setItem(i, 0, new QTableWidgetItem(query.value(0).toString())); // id
        fArtList->setItem(i, 1, new QTableWidgetItem(query.value(1).toString())); // name
        fArtList->setItem(i, 2, new QTableWidgetItem(query.value(2).toString())); // price
        fArtList->setItem(i, 3, new QTableWidgetItem(query.value(3).toString())); // comment

        i++;
    }

    // pour que la largeur des colonnes soit automatiquement mise à jour
    // pour s'accorder au contenu
    fArtList->resizeColumnsToContents();

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
void QfactureImpl::on_fArtList_itemDoubleClicked(QTableWidgetItem* item)
{
    QSqlQuery query;
    QString art_id, art_price;

    art_id = fArtList->item(item->row(), 0)->text();

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

    fFlag = false;
    int i=0;
    while(query.next()) {
        fArtLink->setItem(i, 0, new QTableWidgetItem(query.value(0).toString())); // id
        fArtLink->setItem(i, 1, new QTableWidgetItem(query.value(1).toString())); // name
        fArtLink->setItem(i, 2, new QTableWidgetItem(query.value(2).toString())); // price
        fArtLink->setItem(i, 3, new QTableWidgetItem(query.value(3).toString())); // nbr
        fArtLink->setItem(i, 4, new QTableWidgetItem(query.value(4).toString())); // off
        fArtLink->setItem(i, 5, new QTableWidgetItem(query.value(5).toString())); // mont

        i++;
    }
    fFlag = true;
    query.finish();

    /* Ajustement automatique largeur des colonnes */
    fArtLink->resizeColumnsToContents();
}

void QfactureImpl::on_fArtLink_itemChanged(QTableWidgetItem* Item)
{
    /** Une cellule du tableau lien article est modifiée **/
    if (fFlag) {
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
                updateInvoiceAmount();
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
}

/**
 * Met à jour le montant total de la facture en cours
 *
 * @return void
 */
void QfactureImpl::updateInvoiceAmount()
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

/**
 * Edite la facture actuellement sélectionnée dans la liste (ou plutôt
 * en cours d'édition)
 *
 * @return void
 */
void QfactureImpl::on_fPrint_clicked()
{
    if(fNum->text().isEmpty()) {
        /* On ne peut imprimer qu'une facture sauvegardée */
        QMessageBox::warning(this, "Qfacture",
                             trUtf8("La facture n'est pas sauvegardée!"),
                             QMessageBox::Ok);
        return;
    }

    QWebView view;
    QPrinter printer;
    QString invoice_tpl = getInvoiceHTMLTpl();
    QSqlQuery query;
    QString logo_file = QDir::tempPath()+"/logo.png";
    QPixmap logo;

    // configuration du printer
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(makeFactureReference(fNum->text(), fDate->text()) + " - " + fType->currentText() + ".pdf");

    /** récupération des infos pour la facture  **/

    // infos sur l'AE
    query.exec(
            "SELECT Name, Siret, Adress, Adress2, Zip, City, Phone, Mail, Home, Logo "
            "FROM user WHERE id = 1"
            );
    query.next();

    invoice_tpl.replace("{% siren %}", query.value(1).toString())
            .replace("{% ae_name %}", query.value(0).toString())
            .replace("{% ae_address %}", query.value(2).toString())
            .replace("{% ae_address2 %}", query.value(3).toString())
            .replace("{% ae_zip %}", query.value(4).toString())
            .replace("{% ae_city %}", query.value(5).toString())
            .replace("{% ae_tel %}", query.value(6).toString())
            .replace("{% ae_mail %}", query.value(7).toString())
            .replace("{% ae_web %}", query.value(8).toString());

    // affichage du logo
    logo.loadFromData(query.value(9).toByteArray());
    logo.save(logo_file);
    invoice_tpl.replace("{% logo_url %}", "file://"+logo_file);

    query.finish();

    // infos sur la facture et le client
    query.prepare(
            "SELECT f.Id, f.Amount, f.Comment, f.Payment, f.Reference, "
            "f.Type, f.Date, c.Name, c.Adress, c.Adress2, c.Zip, "
            "c.City "
            "FROM facture f "
            "LEFT JOIN client c ON f.idClient = c.id "
            "WHERE f.Id = ?"
            );
    query.bindValue(0, fNum->text());
    query.exec();
    query.next();

    invoice_tpl.replace("{% ref %}", makeFactureReference(fNum->text(), fDate->text()))
            .replace("{% invoice_date %}", query.value(6).toString())
            .replace("{% invoice_comment %}", query.value(2).toString())
            .replace("{% invoice_amount %}", query.value(1).toString())
            .replace("{% invoice_type %}", query.value(5).toString())
            .replace("{% customer_name %}", query.value(7).toString())
            .replace("{% customer_address %}", query.value(8).toString())
            .replace("{% customer_address2 %}", query.value(9).toString())
            .replace("{% customer_zip %}", query.value(10).toString())
            .replace("{% customer_city %}", query.value(11).toString());

    // on parse la boucle des produits
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

    // traitement de la boucle via une regex
    QRegExp regex(QRegExp::escape("{% product_line %}") + "(.+)" + QRegExp::escape("{% product_line %}"));
    QString products;
    QString product_line;
    int pos=0;

    while ((pos = regex.indexIn(invoice_tpl, pos)) != -1)
    {
        product_line = regex.capturedTexts().at(0);
        product_line.replace("{% product_line %}", " "); // dirty hack ... un de plus.

        pos += regex.matchedLength();
    }

    while(query.next()) {
        QString product_line_tmp = product_line;
        products += product_line_tmp.replace("{% designation %}", query.value(1).toString())
                    .replace("{% prix_unitaire %}", query.value(2).toString())
                    .replace("{% quantite %}", query.value(3).toString())
                    .replace("{% montant %}", query.value(5).toString());
    }

    invoice_tpl.replace(regex, products);

    view.setHtml(invoice_tpl);

    // affichage d'une boite de dialogue avec quelques options d'impression
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted) {
        view.print(&printer);

        statusbar->showMessage(trUtf8("Facture imprimée."), 3000);
    }
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

/**
 * Méthode rafraichissant la liste des chiffres d'affaires cumulés par mois.
 *
 * @return void
 */
void QfactureImpl::sListCaRefresh()
{
    QSqlQuery query;
    QString year;
    QString text;

    year = sYearCa->text();

    if(year.length() != 4 and !year.isEmpty()) {
        /* Date invalide */
        QMessageBox::warning(this, "Qfacture",
                             QString(trUtf8("La date saisie n'est pas valide!")),
                             QMessageBox::Ok);
        year = QDate::currentDate().toString(QString(trUtf8("yyyy")));
        sYearCa->setText(year);
    }
    if (year.isEmpty()) {
        /* Date non renseignée */
        year = QDate::currentDate().toString(QString(trUtf8("yyyy")));
        sYearCa->setText(year);
    }
    sListCa->clear();

    query.prepare(
            "SELECT LEFT(date, 7) AS month, SUM(amount) AS sum "
            "FROM facture "
            "WHERE date LIKE :year "
            "  AND type = 'FACTU'"
            "GROUP BY month"
            );
    query.bindValue(":year", year+QString("%"));

    query.exec();

    while(query.next()){
        text = query.value(0).toString()
               + trUtf8(" : ")
               + query.value(1).toString()
               + trUtf8("€");

        sListCa->addItem(text);
    }

    query.finish();
}

/**
 * Méthode de callback appellée lorsque l'année est modifiée.
 *
 * @return void
 */
void QfactureImpl::on_sYearCa_lostFocus()
{
    /** Année changée **/

    sListCaRefresh();
}


/* Tab template - Modèle de facture*******************************************/

/**
 * Méthode de callback appelée lorsque l'on clique sur le bouton Ouvrir
 *
 * @return void
 */
void QfactureImpl::on_tOpen_clicked()
{
    /** Charger le template **/

    QString Modele = getInvoiceHTMLTpl();

    tText->setPlainText(Modele);
    tText->setDisabled(false);
    tText->setFocus();
    statusbar->showMessage(trUtf8("Le modèle de facture a été chargé avec succés!"));
}

/**
 * Méthode de callback appelée lorsque l'on clique sur le bouton Sauver
 *
 * @return void
 */
void QfactureImpl::on_tSave_clicked()
{
    /** Sauver le template **/

    if (!tText->toPlainText().isNull()) {
        QString New = setInvoiceHTMLTpl(tText->toPlainText());
        tText->setPlainText(New);
        statusbar->showMessage(trUtf8("Le modèle de facture a été sauvé avec succés!"));
    }
}

/**
 * Méthode de callback appelée lorsque l'on clique sur le bouton Annuler
 *
 * @return void
 */
void QfactureImpl::on_tUndo_clicked()
{
    /** Annuler les modifications en cours **/

    tText->setPlainText(QString(trUtf8("Modèle de facture non chargé...")));
    tText->setDisabled(true);
    statusbar->showMessage(trUtf8("Modifications annulées!"));
}
