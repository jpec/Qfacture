/********************************************************************************
** Form generated from reading UI file 'Qfacture.ui'
**
** Created: Mon Oct 4 17:18:23 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef QFACTURE_H
#define QFACTURE_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qfacture
{
public:
    QAction *action_Quitter;
    QAction *action_propos;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *Param;
    QGridLayout *gridLayout_3;
    QGroupBox *uGroupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *uName;
    QLabel *label_2;
    QLineEdit *uSiret;
    QLabel *label_3;
    QLineEdit *uAdress;
    QLabel *label_4;
    QLineEdit *uAdress2;
    QLabel *label_5;
    QLineEdit *uZip;
    QLabel *label_6;
    QLineEdit *uCity;
    QLabel *label_7;
    QLineEdit *uPhone;
    QLabel *label_8;
    QLineEdit *uMail;
    QLabel *label_9;
    QLineEdit *uHome;
    QPushButton *uSave;
    QPushButton *uChangeLogo;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QGroupBox *aGroupBox;
    QFormLayout *formLayout_3;
    QLabel *label_14;
    QLineEdit *aServer;
    QLabel *label_15;
    QLineEdit *aPort;
    QLabel *label_16;
    QLineEdit *aUser;
    QLabel *label_17;
    QLineEdit *aPass;
    QLabel *label_18;
    QLineEdit *aDb;
    QPushButton *aConnect;
    QSpacerItem *verticalSpacer_2;
    QLabel *uLogo;
    QWidget *tClient;
    QGridLayout *gridLayout_4;
    QGroupBox *clGroupBox;
    QGridLayout *gridLayout_5;
    QTableView *cList;
    QGroupBox *cGroupBox;
    QFormLayout *formLayout_2;
    QLabel *label_13;
    QLabel *label_12;
    QLineEdit *cAdress2;
    QLabel *label_19;
    QLineEdit *cZip;
    QLabel *label_20;
    QLineEdit *cCity;
    QLabel *label_21;
    QLineEdit *cPhone;
    QLabel *label_22;
    QLineEdit *cMail;
    QLineEdit *cAdress;
    QLineEdit *cName;
    QLabel *label_11;
    QLineEdit *cId;
    QLabel *label_10;
    QGroupBox *actGroupBox;
    QFormLayout *formLayout_5;
    QPushButton *cNew;
    QPushButton *cSave;
    QPushButton *cDel;
    QSpacerItem *horizontalSpacer_3;
    QWidget *tArticle;
    QGridLayout *gridLayout;
    QGroupBox *artlGroupBox;
    QGridLayout *gridLayout_6;
    QTableView *aList;
    QGroupBox *artGroupBox;
    QFormLayout *formLayout_4;
    QLabel *label_25;
    QLineEdit *aId;
    QLabel *label_23;
    QLineEdit *aName;
    QLabel *label_26;
    QDoubleSpinBox *aPrice;
    QLabel *label_24;
    QLineEdit *aCom;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *artaGroupBox;
    QFormLayout *formLayout_6;
    QPushButton *aNew;
    QPushButton *aSave;
    QPushButton *aDel;
    QWidget *tFacture;
    QGridLayout *gridLayout_7;
    QTabWidget *tabFacture;
    QWidget *tabFact1;
    QGridLayout *gridLayout_9;
    QGroupBox *fArticleGroup;
    QGridLayout *gridLayout_11;
    QTableWidget *fArtLink;
    QTableWidget *fArtList;
    QGroupBox *fClientGroup;
    QGridLayout *gridLayout_10;
    QLineEdit *fClient;
    QListWidget *fClientList;
    QGroupBox *fDetail;
    QFormLayout *formLayout_7;
    QLabel *fNumLab;
    QLineEdit *fNum;
    QLabel *fDateLab;
    QDateEdit *fDate;
    QLabel *fTypeLab;
    QComboBox *fType;
    QLabel *fMontantLab;
    QLineEdit *fMontant;
    QLabel *fReglLab;
    QComboBox *fRegl;
    QFrame *frame;
    QGridLayout *gridLayout_12;
    QPushButton *fCalc;
    QPushButton *fSave;
    QPushButton *fPrint;
    QPushButton *fDel;
    QPushButton *fNew;
    QWidget *tabFact2;
    QGridLayout *gridLayout_8;
    QTableWidget *fList;
    QWidget *tStat;
    QGridLayout *gridLayout_13;
    QGroupBox *sGroupCa;
    QGridLayout *gridLayout_14;
    QLabel *label_27;
    QLineEdit *sYearCa;
    QListWidget *sListCa;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_3;
    QMenuBar *menubar;
    QMenu *menu_Menu;
    QMenu *menu_Aide;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Qfacture)
    {
        if (Qfacture->objectName().isEmpty())
            Qfacture->setObjectName(QString::fromUtf8("Qfacture"));
        Qfacture->setWindowModality(Qt::WindowModal);
        Qfacture->resize(800, 590);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Qfacture->sizePolicy().hasHeightForWidth());
        Qfacture->setSizePolicy(sizePolicy);
        Qfacture->setMinimumSize(QSize(800, 590));
        Qfacture->setSizeIncrement(QSize(1, 1));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../art/qfacture.png"), QSize(), QIcon::Normal, QIcon::Off);
        Qfacture->setWindowIcon(icon);
        Qfacture->setUnifiedTitleAndToolBarOnMac(false);
        action_Quitter = new QAction(Qfacture);
        action_Quitter->setObjectName(QString::fromUtf8("action_Quitter"));
        action_propos = new QAction(Qfacture);
        action_propos->setObjectName(QString::fromUtf8("action_propos"));
        centralwidget = new QWidget(Qfacture);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(0, 0));
        Param = new QWidget();
        Param->setObjectName(QString::fromUtf8("Param"));
        gridLayout_3 = new QGridLayout(Param);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        uGroupBox = new QGroupBox(Param);
        uGroupBox->setObjectName(QString::fromUtf8("uGroupBox"));
        uGroupBox->setEnabled(false);
        uGroupBox->setMinimumSize(QSize(0, 0));
        uGroupBox->setMaximumSize(QSize(800, 600));
        uGroupBox->setAutoFillBackground(false);
        formLayout = new QFormLayout(uGroupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(uGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        uName = new QLineEdit(uGroupBox);
        uName->setObjectName(QString::fromUtf8("uName"));
        uName->setMinimumSize(QSize(300, 22));
        uName->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(0, QFormLayout::FieldRole, uName);

        label_2 = new QLabel(uGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        uSiret = new QLineEdit(uGroupBox);
        uSiret->setObjectName(QString::fromUtf8("uSiret"));
        uSiret->setMinimumSize(QSize(300, 0));
        uSiret->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(1, QFormLayout::FieldRole, uSiret);

        label_3 = new QLabel(uGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        uAdress = new QLineEdit(uGroupBox);
        uAdress->setObjectName(QString::fromUtf8("uAdress"));
        uAdress->setMinimumSize(QSize(300, 0));
        uAdress->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(2, QFormLayout::FieldRole, uAdress);

        label_4 = new QLabel(uGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        uAdress2 = new QLineEdit(uGroupBox);
        uAdress2->setObjectName(QString::fromUtf8("uAdress2"));
        uAdress2->setMinimumSize(QSize(300, 0));
        uAdress2->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(3, QFormLayout::FieldRole, uAdress2);

        label_5 = new QLabel(uGroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_5);

        uZip = new QLineEdit(uGroupBox);
        uZip->setObjectName(QString::fromUtf8("uZip"));
        uZip->setMinimumSize(QSize(300, 0));
        uZip->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(4, QFormLayout::FieldRole, uZip);

        label_6 = new QLabel(uGroupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_6);

        uCity = new QLineEdit(uGroupBox);
        uCity->setObjectName(QString::fromUtf8("uCity"));
        uCity->setMinimumSize(QSize(300, 0));
        uCity->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(5, QFormLayout::FieldRole, uCity);

        label_7 = new QLabel(uGroupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_7);

        uPhone = new QLineEdit(uGroupBox);
        uPhone->setObjectName(QString::fromUtf8("uPhone"));
        uPhone->setMinimumSize(QSize(300, 0));
        uPhone->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(6, QFormLayout::FieldRole, uPhone);

        label_8 = new QLabel(uGroupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_8);

        uMail = new QLineEdit(uGroupBox);
        uMail->setObjectName(QString::fromUtf8("uMail"));
        uMail->setMinimumSize(QSize(300, 0));
        uMail->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(7, QFormLayout::FieldRole, uMail);

        label_9 = new QLabel(uGroupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout->setWidget(8, QFormLayout::LabelRole, label_9);

        uHome = new QLineEdit(uGroupBox);
        uHome->setObjectName(QString::fromUtf8("uHome"));
        uHome->setMinimumSize(QSize(300, 0));
        uHome->setMaximumSize(QSize(300, 16777215));

        formLayout->setWidget(8, QFormLayout::FieldRole, uHome);

        uSave = new QPushButton(uGroupBox);
        uSave->setObjectName(QString::fromUtf8("uSave"));
        uSave->setEnabled(false);
        uSave->setMinimumSize(QSize(0, 0));
        uSave->setMaximumSize(QSize(500, 16777215));
        uSave->setCheckable(false);
        uSave->setFlat(false);

        formLayout->setWidget(10, QFormLayout::FieldRole, uSave);

        uChangeLogo = new QPushButton(uGroupBox);
        uChangeLogo->setObjectName(QString::fromUtf8("uChangeLogo"));

        formLayout->setWidget(9, QFormLayout::FieldRole, uChangeLogo);


        gridLayout_3->addWidget(uGroupBox, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 4, 2, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);

        aGroupBox = new QGroupBox(Param);
        aGroupBox->setObjectName(QString::fromUtf8("aGroupBox"));
        sizePolicy1.setHeightForWidth(aGroupBox->sizePolicy().hasHeightForWidth());
        aGroupBox->setSizePolicy(sizePolicy1);
        aGroupBox->setMinimumSize(QSize(0, 0));
        aGroupBox->setMaximumSize(QSize(800, 800));
        formLayout_3 = new QFormLayout(aGroupBox);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_14 = new QLabel(aGroupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(0, 0));
        label_14->setMaximumSize(QSize(1555, 16777215));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_14);

        aServer = new QLineEdit(aGroupBox);
        aServer->setObjectName(QString::fromUtf8("aServer"));
        aServer->setMinimumSize(QSize(200, 0));
        aServer->setMaximumSize(QSize(200, 16777215));
        aServer->setInputMask(QString::fromUtf8(""));
        aServer->setText(QString::fromUtf8("localhost"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, aServer);

        label_15 = new QLabel(aGroupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(0, 0));
        label_15->setMaximumSize(QSize(1555, 16777215));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_15);

        aPort = new QLineEdit(aGroupBox);
        aPort->setObjectName(QString::fromUtf8("aPort"));
        aPort->setMinimumSize(QSize(200, 0));
        aPort->setMaximumSize(QSize(200, 16777215));
        aPort->setText(QString::fromUtf8("3306"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, aPort);

        label_16 = new QLabel(aGroupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(0, 0));
        label_16->setMaximumSize(QSize(1555, 16777215));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_16);

        aUser = new QLineEdit(aGroupBox);
        aUser->setObjectName(QString::fromUtf8("aUser"));
        aUser->setMinimumSize(QSize(200, 0));
        aUser->setMaximumSize(QSize(200, 16777215));
        aUser->setInputMask(QString::fromUtf8(""));
        aUser->setText(QString::fromUtf8("qfacture"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, aUser);

        label_17 = new QLabel(aGroupBox);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setMinimumSize(QSize(0, 0));
        label_17->setMaximumSize(QSize(1555, 16777215));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_17);

        aPass = new QLineEdit(aGroupBox);
        aPass->setObjectName(QString::fromUtf8("aPass"));
        aPass->setMinimumSize(QSize(200, 0));
        aPass->setMaximumSize(QSize(200, 16777215));
        aPass->setInputMask(QString::fromUtf8(""));
        aPass->setText(QString::fromUtf8(""));
        aPass->setEchoMode(QLineEdit::Password);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, aPass);

        label_18 = new QLabel(aGroupBox);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setMinimumSize(QSize(0, 0));
        label_18->setMaximumSize(QSize(1555, 16777215));

        formLayout_3->setWidget(4, QFormLayout::LabelRole, label_18);

        aDb = new QLineEdit(aGroupBox);
        aDb->setObjectName(QString::fromUtf8("aDb"));
        aDb->setMinimumSize(QSize(200, 0));
        aDb->setMaximumSize(QSize(200, 16777215));
        aDb->setInputMask(QString::fromUtf8(""));
        aDb->setText(QString::fromUtf8("qfacture"));

        formLayout_3->setWidget(4, QFormLayout::FieldRole, aDb);

        aConnect = new QPushButton(aGroupBox);
        aConnect->setObjectName(QString::fromUtf8("aConnect"));
        aConnect->setMaximumSize(QSize(16777214, 16777215));

        formLayout_3->setWidget(5, QFormLayout::SpanningRole, aConnect);


        gridLayout_3->addWidget(aGroupBox, 0, 3, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 1, 3, 1, 1);

        uLogo = new QLabel(Param);
        uLogo->setObjectName(QString::fromUtf8("uLogo"));
        uLogo->setMinimumSize(QSize(300, 150));
        uLogo->setMaximumSize(QSize(300, 150));
        uLogo->setAlignment(Qt::AlignCenter);
        uLogo->setIndent(-5);

        gridLayout_3->addWidget(uLogo, 1, 1, 1, 1);

        tabWidget->addTab(Param, QString());
        tClient = new QWidget();
        tClient->setObjectName(QString::fromUtf8("tClient"));
        gridLayout_4 = new QGridLayout(tClient);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        clGroupBox = new QGroupBox(tClient);
        clGroupBox->setObjectName(QString::fromUtf8("clGroupBox"));
        gridLayout_5 = new QGridLayout(clGroupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        cList = new QTableView(clGroupBox);
        cList->setObjectName(QString::fromUtf8("cList"));
        cList->setAutoScrollMargin(10);
        cList->setEditTriggers(QAbstractItemView::DoubleClicked);
        cList->setAlternatingRowColors(true);
        cList->setSelectionMode(QAbstractItemView::SingleSelection);
        cList->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        cList->setSortingEnabled(true);
        cList->horizontalHeader()->setCascadingSectionResizes(false);

        gridLayout_5->addWidget(cList, 0, 0, 1, 1);


        gridLayout_4->addWidget(clGroupBox, 0, 0, 1, 3);

        cGroupBox = new QGroupBox(tClient);
        cGroupBox->setObjectName(QString::fromUtf8("cGroupBox"));
        cGroupBox->setEnabled(false);
        cGroupBox->setMinimumSize(QSize(0, 0));
        cGroupBox->setMaximumSize(QSize(1000, 1000));
        formLayout_2 = new QFormLayout(cGroupBox);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_13 = new QLabel(cGroupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_13);

        label_12 = new QLabel(cGroupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_12);

        cAdress2 = new QLineEdit(cGroupBox);
        cAdress2->setObjectName(QString::fromUtf8("cAdress2"));
        cAdress2->setMinimumSize(QSize(250, 0));
        cAdress2->setMaximumSize(QSize(250, 16777215));

        formLayout_2->setWidget(4, QFormLayout::FieldRole, cAdress2);

        label_19 = new QLabel(cGroupBox);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_19);

        cZip = new QLineEdit(cGroupBox);
        cZip->setObjectName(QString::fromUtf8("cZip"));
        cZip->setMinimumSize(QSize(250, 0));
        cZip->setMaximumSize(QSize(250, 16777215));

        formLayout_2->setWidget(5, QFormLayout::FieldRole, cZip);

        label_20 = new QLabel(cGroupBox);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_20);

        cCity = new QLineEdit(cGroupBox);
        cCity->setObjectName(QString::fromUtf8("cCity"));
        cCity->setMinimumSize(QSize(250, 0));
        cCity->setMaximumSize(QSize(250, 16777215));

        formLayout_2->setWidget(6, QFormLayout::FieldRole, cCity);

        label_21 = new QLabel(cGroupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        formLayout_2->setWidget(7, QFormLayout::LabelRole, label_21);

        cPhone = new QLineEdit(cGroupBox);
        cPhone->setObjectName(QString::fromUtf8("cPhone"));
        cPhone->setMinimumSize(QSize(250, 0));
        cPhone->setMaximumSize(QSize(250, 16777215));

        formLayout_2->setWidget(7, QFormLayout::FieldRole, cPhone);

        label_22 = new QLabel(cGroupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        formLayout_2->setWidget(8, QFormLayout::LabelRole, label_22);

        cMail = new QLineEdit(cGroupBox);
        cMail->setObjectName(QString::fromUtf8("cMail"));
        cMail->setMinimumSize(QSize(250, 0));
        cMail->setMaximumSize(QSize(250, 16777215));

        formLayout_2->setWidget(8, QFormLayout::FieldRole, cMail);

        cAdress = new QLineEdit(cGroupBox);
        cAdress->setObjectName(QString::fromUtf8("cAdress"));
        cAdress->setMinimumSize(QSize(250, 0));
        cAdress->setMaximumSize(QSize(250, 16777215));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, cAdress);

        cName = new QLineEdit(cGroupBox);
        cName->setObjectName(QString::fromUtf8("cName"));
        cName->setMinimumSize(QSize(250, 0));
        cName->setMaximumSize(QSize(250, 16777215));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, cName);

        label_11 = new QLabel(cGroupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_11);

        cId = new QLineEdit(cGroupBox);
        cId->setObjectName(QString::fromUtf8("cId"));
        cId->setEnabled(false);
        cId->setMinimumSize(QSize(250, 0));
        cId->setMaximumSize(QSize(250, 16777215));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, cId);

        label_10 = new QLabel(cGroupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(100, 0));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_10);


        gridLayout_4->addWidget(cGroupBox, 1, 0, 1, 1);

        actGroupBox = new QGroupBox(tClient);
        actGroupBox->setObjectName(QString::fromUtf8("actGroupBox"));
        actGroupBox->setMinimumSize(QSize(0, 0));
        actGroupBox->setMaximumSize(QSize(150, 300));
        formLayout_5 = new QFormLayout(actGroupBox);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        cNew = new QPushButton(actGroupBox);
        cNew->setObjectName(QString::fromUtf8("cNew"));
        cNew->setMinimumSize(QSize(100, 0));
        cNew->setMaximumSize(QSize(100, 16777215));

        formLayout_5->setWidget(0, QFormLayout::SpanningRole, cNew);

        cSave = new QPushButton(actGroupBox);
        cSave->setObjectName(QString::fromUtf8("cSave"));
        cSave->setEnabled(false);
        cSave->setMinimumSize(QSize(100, 0));
        cSave->setMaximumSize(QSize(100, 16777215));

        formLayout_5->setWidget(1, QFormLayout::SpanningRole, cSave);

        cDel = new QPushButton(actGroupBox);
        cDel->setObjectName(QString::fromUtf8("cDel"));
        cDel->setEnabled(false);
        cDel->setMinimumSize(QSize(100, 0));
        cDel->setMaximumSize(QSize(100, 16777215));

        formLayout_5->setWidget(2, QFormLayout::SpanningRole, cDel);


        gridLayout_4->addWidget(actGroupBox, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 1, 2, 1, 1);

        tabWidget->addTab(tClient, QString());
        tArticle = new QWidget();
        tArticle->setObjectName(QString::fromUtf8("tArticle"));
        gridLayout = new QGridLayout(tArticle);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        artlGroupBox = new QGroupBox(tArticle);
        artlGroupBox->setObjectName(QString::fromUtf8("artlGroupBox"));
        sizePolicy.setHeightForWidth(artlGroupBox->sizePolicy().hasHeightForWidth());
        artlGroupBox->setSizePolicy(sizePolicy);
        gridLayout_6 = new QGridLayout(artlGroupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        aList = new QTableView(artlGroupBox);
        aList->setObjectName(QString::fromUtf8("aList"));
        sizePolicy.setHeightForWidth(aList->sizePolicy().hasHeightForWidth());
        aList->setSizePolicy(sizePolicy);
        aList->setMinimumSize(QSize(0, 280));
        aList->setEditTriggers(QAbstractItemView::DoubleClicked);
        aList->setAlternatingRowColors(true);
        aList->setSelectionMode(QAbstractItemView::SingleSelection);
        aList->setSortingEnabled(true);

        gridLayout_6->addWidget(aList, 0, 0, 1, 1);


        gridLayout->addWidget(artlGroupBox, 0, 0, 2, 4);

        artGroupBox = new QGroupBox(tArticle);
        artGroupBox->setObjectName(QString::fromUtf8("artGroupBox"));
        artGroupBox->setEnabled(false);
        artGroupBox->setMinimumSize(QSize(1, 1));
        artGroupBox->setMaximumSize(QSize(500, 300));
        formLayout_4 = new QFormLayout(artGroupBox);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_25 = new QLabel(artGroupBox);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_25);

        aId = new QLineEdit(artGroupBox);
        aId->setObjectName(QString::fromUtf8("aId"));
        aId->setEnabled(false);
        aId->setMinimumSize(QSize(100, 0));
        aId->setMaximumSize(QSize(100, 16777215));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, aId);

        label_23 = new QLabel(artGroupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_23);

        aName = new QLineEdit(artGroupBox);
        aName->setObjectName(QString::fromUtf8("aName"));
        aName->setMinimumSize(QSize(250, 0));
        aName->setMaximumSize(QSize(250, 16777215));

        formLayout_4->setWidget(1, QFormLayout::FieldRole, aName);

        label_26 = new QLabel(artGroupBox);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_26);

        aPrice = new QDoubleSpinBox(artGroupBox);
        aPrice->setObjectName(QString::fromUtf8("aPrice"));
        aPrice->setMinimumSize(QSize(100, 0));
        aPrice->setMaximumSize(QSize(100, 16777215));
        aPrice->setMinimum(-999999);
        aPrice->setMaximum(999999);
        aPrice->setSingleStep(1);
        aPrice->setValue(0);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, aPrice);

        label_24 = new QLabel(artGroupBox);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setMaximumSize(QSize(100, 16777215));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, label_24);

        aCom = new QLineEdit(artGroupBox);
        aCom->setObjectName(QString::fromUtf8("aCom"));
        aCom->setMinimumSize(QSize(250, 0));
        aCom->setMaximumSize(QSize(250, 16777215));

        formLayout_4->setWidget(3, QFormLayout::FieldRole, aCom);


        gridLayout->addWidget(artGroupBox, 2, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 2, 3, 1, 1);

        artaGroupBox = new QGroupBox(tArticle);
        artaGroupBox->setObjectName(QString::fromUtf8("artaGroupBox"));
        artaGroupBox->setMinimumSize(QSize(1, 1));
        artaGroupBox->setMaximumSize(QSize(150, 300));
        formLayout_6 = new QFormLayout(artaGroupBox);
        formLayout_6->setObjectName(QString::fromUtf8("formLayout_6"));
        aNew = new QPushButton(artaGroupBox);
        aNew->setObjectName(QString::fromUtf8("aNew"));

        formLayout_6->setWidget(0, QFormLayout::LabelRole, aNew);

        aSave = new QPushButton(artaGroupBox);
        aSave->setObjectName(QString::fromUtf8("aSave"));
        aSave->setEnabled(false);

        formLayout_6->setWidget(1, QFormLayout::LabelRole, aSave);

        aDel = new QPushButton(artaGroupBox);
        aDel->setObjectName(QString::fromUtf8("aDel"));
        aDel->setEnabled(false);

        formLayout_6->setWidget(2, QFormLayout::LabelRole, aDel);


        gridLayout->addWidget(artaGroupBox, 2, 2, 1, 1);

        tabWidget->addTab(tArticle, QString());
        tFacture = new QWidget();
        tFacture->setObjectName(QString::fromUtf8("tFacture"));
        gridLayout_7 = new QGridLayout(tFacture);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        tabFacture = new QTabWidget(tFacture);
        tabFacture->setObjectName(QString::fromUtf8("tabFacture"));
        tabFacture->setTabPosition(QTabWidget::West);
        tabFact1 = new QWidget();
        tabFact1->setObjectName(QString::fromUtf8("tabFact1"));
        gridLayout_9 = new QGridLayout(tabFact1);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        fArticleGroup = new QGroupBox(tabFact1);
        fArticleGroup->setObjectName(QString::fromUtf8("fArticleGroup"));
        fArticleGroup->setEnabled(false);
        fArticleGroup->setMinimumSize(QSize(0, 0));
        gridLayout_11 = new QGridLayout(fArticleGroup);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        fArtLink = new QTableWidget(fArticleGroup);
        if (fArtLink->columnCount() < 6)
            fArtLink->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        fArtLink->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        fArtLink->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        fArtLink->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        fArtLink->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        fArtLink->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        fArtLink->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        fArtLink->setObjectName(QString::fromUtf8("fArtLink"));
        fArtLink->setAlternatingRowColors(true);
        fArtLink->setSelectionMode(QAbstractItemView::SingleSelection);
        fArtLink->setSortingEnabled(true);
        fArtLink->horizontalHeader()->setCascadingSectionResizes(true);
        fArtLink->verticalHeader()->setCascadingSectionResizes(true);
        fArtLink->verticalHeader()->setDefaultSectionSize(20);
        fArtLink->verticalHeader()->setMinimumSectionSize(15);

        gridLayout_11->addWidget(fArtLink, 1, 0, 1, 1);

        fArtList = new QTableWidget(fArticleGroup);
        if (fArtList->columnCount() < 4)
            fArtList->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        fArtList->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        fArtList->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        fArtList->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        fArtList->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        fArtList->setObjectName(QString::fromUtf8("fArtList"));
        fArtList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        fArtList->setAlternatingRowColors(true);
        fArtList->setSelectionMode(QAbstractItemView::SingleSelection);
        fArtList->setSortingEnabled(true);

        gridLayout_11->addWidget(fArtList, 0, 0, 1, 1);


        gridLayout_9->addWidget(fArticleGroup, 2, 0, 1, 2);

        fClientGroup = new QGroupBox(tabFact1);
        fClientGroup->setObjectName(QString::fromUtf8("fClientGroup"));
        fClientGroup->setEnabled(false);
        gridLayout_10 = new QGridLayout(fClientGroup);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        fClient = new QLineEdit(fClientGroup);
        fClient->setObjectName(QString::fromUtf8("fClient"));
        fClient->setEnabled(false);

        gridLayout_10->addWidget(fClient, 1, 0, 1, 1);

        fClientList = new QListWidget(fClientGroup);
        fClientList->setObjectName(QString::fromUtf8("fClientList"));
        fClientList->setAlternatingRowColors(true);

        gridLayout_10->addWidget(fClientList, 0, 0, 1, 1);


        gridLayout_9->addWidget(fClientGroup, 0, 0, 1, 1);

        fDetail = new QGroupBox(tabFact1);
        fDetail->setObjectName(QString::fromUtf8("fDetail"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(fDetail->sizePolicy().hasHeightForWidth());
        fDetail->setSizePolicy(sizePolicy2);
        formLayout_7 = new QFormLayout(fDetail);
        formLayout_7->setObjectName(QString::fromUtf8("formLayout_7"));
        formLayout_7->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        fNumLab = new QLabel(fDetail);
        fNumLab->setObjectName(QString::fromUtf8("fNumLab"));

        formLayout_7->setWidget(0, QFormLayout::LabelRole, fNumLab);

        fNum = new QLineEdit(fDetail);
        fNum->setObjectName(QString::fromUtf8("fNum"));
        fNum->setEnabled(false);

        formLayout_7->setWidget(0, QFormLayout::FieldRole, fNum);

        fDateLab = new QLabel(fDetail);
        fDateLab->setObjectName(QString::fromUtf8("fDateLab"));

        formLayout_7->setWidget(1, QFormLayout::LabelRole, fDateLab);

        fDate = new QDateEdit(fDetail);
        fDate->setObjectName(QString::fromUtf8("fDate"));
        fDate->setEnabled(false);
        fDate->setDate(QDate(2000, 2, 1));

        formLayout_7->setWidget(1, QFormLayout::FieldRole, fDate);

        fTypeLab = new QLabel(fDetail);
        fTypeLab->setObjectName(QString::fromUtf8("fTypeLab"));

        formLayout_7->setWidget(2, QFormLayout::LabelRole, fTypeLab);

        fType = new QComboBox(fDetail);
        fType->setObjectName(QString::fromUtf8("fType"));
        fType->setEnabled(false);

        formLayout_7->setWidget(2, QFormLayout::FieldRole, fType);

        fMontantLab = new QLabel(fDetail);
        fMontantLab->setObjectName(QString::fromUtf8("fMontantLab"));

        formLayout_7->setWidget(3, QFormLayout::LabelRole, fMontantLab);

        fMontant = new QLineEdit(fDetail);
        fMontant->setObjectName(QString::fromUtf8("fMontant"));
        fMontant->setEnabled(false);

        formLayout_7->setWidget(3, QFormLayout::FieldRole, fMontant);

        fReglLab = new QLabel(fDetail);
        fReglLab->setObjectName(QString::fromUtf8("fReglLab"));

        formLayout_7->setWidget(4, QFormLayout::LabelRole, fReglLab);

        fRegl = new QComboBox(fDetail);
        fRegl->setObjectName(QString::fromUtf8("fRegl"));
        fRegl->setEnabled(false);

        formLayout_7->setWidget(4, QFormLayout::FieldRole, fRegl);

        frame = new QFrame(fDetail);
        frame->setObjectName(QString::fromUtf8("frame"));
        gridLayout_12 = new QGridLayout(frame);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        fCalc = new QPushButton(frame);
        fCalc->setObjectName(QString::fromUtf8("fCalc"));
        fCalc->setEnabled(false);

        gridLayout_12->addWidget(fCalc, 0, 1, 1, 1);

        fSave = new QPushButton(frame);
        fSave->setObjectName(QString::fromUtf8("fSave"));
        fSave->setEnabled(false);

        gridLayout_12->addWidget(fSave, 0, 2, 1, 1);

        fPrint = new QPushButton(frame);
        fPrint->setObjectName(QString::fromUtf8("fPrint"));
        fPrint->setEnabled(false);

        gridLayout_12->addWidget(fPrint, 0, 3, 1, 1);

        fDel = new QPushButton(frame);
        fDel->setObjectName(QString::fromUtf8("fDel"));
        fDel->setEnabled(false);

        gridLayout_12->addWidget(fDel, 0, 4, 1, 1);

        fNew = new QPushButton(frame);
        fNew->setObjectName(QString::fromUtf8("fNew"));

        gridLayout_12->addWidget(fNew, 0, 0, 1, 1);


        formLayout_7->setWidget(5, QFormLayout::SpanningRole, frame);


        gridLayout_9->addWidget(fDetail, 0, 1, 1, 1);

        tabFacture->addTab(tabFact1, QString());
        tabFact2 = new QWidget();
        tabFact2->setObjectName(QString::fromUtf8("tabFact2"));
        gridLayout_8 = new QGridLayout(tabFact2);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        fList = new QTableWidget(tabFact2);
        if (fList->columnCount() < 7)
            fList->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        fList->setHorizontalHeaderItem(0, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        fList->setHorizontalHeaderItem(1, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        fList->setHorizontalHeaderItem(2, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        fList->setHorizontalHeaderItem(3, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        fList->setHorizontalHeaderItem(4, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        fList->setHorizontalHeaderItem(5, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        fList->setHorizontalHeaderItem(6, __qtablewidgetitem16);
        fList->setObjectName(QString::fromUtf8("fList"));
        fList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        fList->setAlternatingRowColors(true);
        fList->setSortingEnabled(true);
        fList->setColumnCount(7);

        gridLayout_8->addWidget(fList, 0, 0, 1, 1);

        tabFacture->addTab(tabFact2, QString());

        gridLayout_7->addWidget(tabFacture, 1, 0, 1, 1);

        tabWidget->addTab(tFacture, QString());
        tStat = new QWidget();
        tStat->setObjectName(QString::fromUtf8("tStat"));
        gridLayout_13 = new QGridLayout(tStat);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        sGroupCa = new QGroupBox(tStat);
        sGroupCa->setObjectName(QString::fromUtf8("sGroupCa"));
        sizePolicy1.setHeightForWidth(sGroupCa->sizePolicy().hasHeightForWidth());
        sGroupCa->setSizePolicy(sizePolicy1);
        gridLayout_14 = new QGridLayout(sGroupCa);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        label_27 = new QLabel(sGroupCa);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_27->sizePolicy().hasHeightForWidth());
        label_27->setSizePolicy(sizePolicy3);

        gridLayout_14->addWidget(label_27, 0, 0, 1, 1);

        sYearCa = new QLineEdit(sGroupCa);
        sYearCa->setObjectName(QString::fromUtf8("sYearCa"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(sYearCa->sizePolicy().hasHeightForWidth());
        sYearCa->setSizePolicy(sizePolicy4);
        sYearCa->setMaxLength(4);

        gridLayout_14->addWidget(sYearCa, 0, 1, 1, 1);

        sListCa = new QListWidget(sGroupCa);
        sListCa->setObjectName(QString::fromUtf8("sListCa"));
        sizePolicy1.setHeightForWidth(sListCa->sizePolicy().hasHeightForWidth());
        sListCa->setSizePolicy(sizePolicy1);
        sListCa->setMinimumSize(QSize(300, 220));
        sListCa->setMaximumSize(QSize(500, 220));
        QFont font;
        font.setFamily(QString::fromUtf8("Monospace"));
        font.setPointSize(10);
        sListCa->setFont(font);
        sListCa->setAlternatingRowColors(true);

        gridLayout_14->addWidget(sListCa, 1, 0, 1, 2);


        gridLayout_13->addWidget(sGroupCa, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_13->addItem(horizontalSpacer_5, 0, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_13->addItem(verticalSpacer_3, 1, 0, 1, 1);

        tabWidget->addTab(tStat, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 1);

        Qfacture->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Qfacture);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menu_Menu = new QMenu(menubar);
        menu_Menu->setObjectName(QString::fromUtf8("menu_Menu"));
        menu_Aide = new QMenu(menubar);
        menu_Aide->setObjectName(QString::fromUtf8("menu_Aide"));
        Qfacture->setMenuBar(menubar);
        statusbar = new QStatusBar(Qfacture);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Qfacture->setStatusBar(statusbar);
#ifndef QT_NO_SHORTCUT
        label->setBuddy(uName);
        label_2->setBuddy(uSiret);
        label_3->setBuddy(uAdress);
        label_4->setBuddy(uAdress2);
        label_5->setBuddy(uZip);
        label_6->setBuddy(uCity);
        label_7->setBuddy(uPhone);
        label_8->setBuddy(uMail);
        label_9->setBuddy(uHome);
        label_14->setBuddy(aServer);
        label_15->setBuddy(aPort);
        label_16->setBuddy(aUser);
        label_17->setBuddy(aPass);
        label_18->setBuddy(aDb);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(aServer, aPort);
        QWidget::setTabOrder(aPort, aUser);
        QWidget::setTabOrder(aUser, aDb);
        QWidget::setTabOrder(aDb, aPass);
        QWidget::setTabOrder(aPass, aConnect);
        QWidget::setTabOrder(aConnect, uName);
        QWidget::setTabOrder(uName, uSiret);
        QWidget::setTabOrder(uSiret, uAdress);
        QWidget::setTabOrder(uAdress, uAdress2);
        QWidget::setTabOrder(uAdress2, uZip);
        QWidget::setTabOrder(uZip, uCity);
        QWidget::setTabOrder(uCity, uPhone);
        QWidget::setTabOrder(uPhone, uMail);
        QWidget::setTabOrder(uMail, uHome);
        QWidget::setTabOrder(uHome, cList);
        QWidget::setTabOrder(cList, cId);
        QWidget::setTabOrder(cId, cName);
        QWidget::setTabOrder(cName, cAdress);
        QWidget::setTabOrder(cAdress, cAdress2);
        QWidget::setTabOrder(cAdress2, cZip);
        QWidget::setTabOrder(cZip, cCity);
        QWidget::setTabOrder(cCity, cPhone);
        QWidget::setTabOrder(cPhone, cMail);
        QWidget::setTabOrder(cMail, cNew);
        QWidget::setTabOrder(cNew, cSave);
        QWidget::setTabOrder(cSave, cDel);
        QWidget::setTabOrder(cDel, aId);
        QWidget::setTabOrder(aId, aName);
        QWidget::setTabOrder(aName, aCom);
        QWidget::setTabOrder(aCom, aNew);
        QWidget::setTabOrder(aNew, aSave);
        QWidget::setTabOrder(aSave, aDel);

        menubar->addAction(menu_Menu->menuAction());
        menubar->addAction(menu_Aide->menuAction());
        menu_Menu->addAction(action_Quitter);
        menu_Aide->addAction(action_propos);

        retranslateUi(Qfacture);
        QObject::connect(aConnect, SIGNAL(clicked()), Qfacture, SLOT(update()));
        QObject::connect(aConnect, SIGNAL(clicked()), statusbar, SLOT(update()));
        QObject::connect(aConnect, SIGNAL(clicked()), aGroupBox, SLOT(update()));

        tabWidget->setCurrentIndex(0);
        tabFacture->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Qfacture);
    } // setupUi

    void retranslateUi(QMainWindow *Qfacture)
    {
        Qfacture->setWindowTitle(QApplication::translate("Qfacture", "Qfacture", 0, QApplication::UnicodeUTF8));
        action_Quitter->setText(QApplication::translate("Qfacture", "&Quitter", 0, QApplication::UnicodeUTF8));
        action_propos->setText(QApplication::translate("Qfacture", "&\303\200 propos", 0, QApplication::UnicodeUTF8));
        uGroupBox->setTitle(QApplication::translate("Qfacture", "Utilisateur", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Qfacture", "Raison sociale", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Qfacture", "SIRET", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Qfacture", "Adresse", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Qfacture", "Compl\303\251ment", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Qfacture", "Code postal", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Qfacture", "Ville", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Qfacture", "T\303\251l\303\251phone", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Qfacture", "Email", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("Qfacture", "Site internet", 0, QApplication::UnicodeUTF8));
        uSave->setText(QApplication::translate("Qfacture", "Enregistrer", 0, QApplication::UnicodeUTF8));
        uChangeLogo->setText(QApplication::translate("Qfacture", "Changer le logo", 0, QApplication::UnicodeUTF8));
        aGroupBox->setTitle(QApplication::translate("Qfacture", "Application", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Qfacture", "H\303\264te", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Qfacture", "Port", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("Qfacture", "Utilisateur", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Qfacture", "Mot de passe", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Qfacture", "Base", 0, QApplication::UnicodeUTF8));
        aConnect->setText(QApplication::translate("Qfacture", "Connexion", 0, QApplication::UnicodeUTF8));
        uLogo->setText(QApplication::translate("Qfacture", "S\303\251lectionnez une image de 300x150 pixels.", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Param), QApplication::translate("Qfacture", "Param\303\250tres", 0, QApplication::UnicodeUTF8));
        clGroupBox->setTitle(QApplication::translate("Qfacture", "Liste des clients", 0, QApplication::UnicodeUTF8));
        cGroupBox->setTitle(QApplication::translate("Qfacture", "Client", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Qfacture", "Adresse", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Qfacture", "Compl\303\251ment", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("Qfacture", "Code postal", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("Qfacture", "Ville", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("Qfacture", "T\303\251l\303\251phone", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("Qfacture", "Email", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Qfacture", "Nom", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Qfacture", "Num\303\251ro client", 0, QApplication::UnicodeUTF8));
        actGroupBox->setTitle(QApplication::translate("Qfacture", "Actions", 0, QApplication::UnicodeUTF8));
        cNew->setText(QApplication::translate("Qfacture", "Nouveau", 0, QApplication::UnicodeUTF8));
        cSave->setText(QApplication::translate("Qfacture", "Enregistrer", 0, QApplication::UnicodeUTF8));
        cDel->setText(QApplication::translate("Qfacture", "Supprimer", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tClient), QApplication::translate("Qfacture", "Clients", 0, QApplication::UnicodeUTF8));
        artlGroupBox->setTitle(QApplication::translate("Qfacture", "Liste des prestations", 0, QApplication::UnicodeUTF8));
        artGroupBox->setTitle(QApplication::translate("Qfacture", "Prestations", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("Qfacture", "Identifiant", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("Qfacture", "D\303\251signation", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("Qfacture", "Prix unitaire", 0, QApplication::UnicodeUTF8));
        aPrice->setSuffix(QApplication::translate("Qfacture", " \342\202\254", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("Qfacture", "Commentaire", 0, QApplication::UnicodeUTF8));
        artaGroupBox->setTitle(QApplication::translate("Qfacture", "Actions", 0, QApplication::UnicodeUTF8));
        aNew->setText(QApplication::translate("Qfacture", "Nouveau", 0, QApplication::UnicodeUTF8));
        aSave->setText(QApplication::translate("Qfacture", "Enregistrer", 0, QApplication::UnicodeUTF8));
        aDel->setText(QApplication::translate("Qfacture", "Supprimer", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tArticle), QApplication::translate("Qfacture", "Prestations", 0, QApplication::UnicodeUTF8));
        fArticleGroup->setTitle(QApplication::translate("Qfacture", "Liste des prestations", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = fArtLink->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Qfacture", "Id", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = fArtLink->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Qfacture", "D\303\251signation", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = fArtLink->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Qfacture", "Prix unitaire", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = fArtLink->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("Qfacture", "Quantit\303\251", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = fArtLink->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("Qfacture", "Remise", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = fArtLink->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("Qfacture", "Total", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = fArtList->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("Qfacture", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = fArtList->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("Qfacture", "Nom", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = fArtList->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("Qfacture", "Prix", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = fArtList->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QApplication::translate("Qfacture", "Commentaire", 0, QApplication::UnicodeUTF8));
        fClientGroup->setTitle(QApplication::translate("Qfacture", "Client", 0, QApplication::UnicodeUTF8));
        fDetail->setTitle(QApplication::translate("Qfacture", "D\303\251tails de la facture", 0, QApplication::UnicodeUTF8));
        fNumLab->setText(QApplication::translate("Qfacture", "Num\303\251ro de la facture", 0, QApplication::UnicodeUTF8));
        fDateLab->setText(QApplication::translate("Qfacture", "Date de facturation", 0, QApplication::UnicodeUTF8));
        fDate->setDisplayFormat(QApplication::translate("Qfacture", "dd/MM/yyyy", 0, QApplication::UnicodeUTF8));
        fTypeLab->setText(QApplication::translate("Qfacture", "Type de document", 0, QApplication::UnicodeUTF8));
        fType->clear();
        fType->insertItems(0, QStringList()
         << QApplication::translate("Qfacture", "FACTU", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Qfacture", "DEVIS", 0, QApplication::UnicodeUTF8)
        );
        fMontantLab->setText(QApplication::translate("Qfacture", "Montant total", 0, QApplication::UnicodeUTF8));
        fReglLab->setText(QApplication::translate("Qfacture", "R\303\250glement", 0, QApplication::UnicodeUTF8));
        fRegl->clear();
        fRegl->insertItems(0, QStringList()
         << QApplication::translate("Qfacture", "Aucun r\303\251glement", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Qfacture", "Ch\303\250que", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Qfacture", "Esp\303\250ces", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Qfacture", "Paypal", 0, QApplication::UnicodeUTF8)
        );
        fCalc->setText(QApplication::translate("Qfacture", "Recalculer", 0, QApplication::UnicodeUTF8));
        fSave->setText(QApplication::translate("Qfacture", "Sauver", 0, QApplication::UnicodeUTF8));
        fPrint->setText(QApplication::translate("Qfacture", "Imprimer", 0, QApplication::UnicodeUTF8));
        fDel->setText(QApplication::translate("Qfacture", "Supprimer", 0, QApplication::UnicodeUTF8));
        fNew->setText(QApplication::translate("Qfacture", "Cr\303\251er", 0, QApplication::UnicodeUTF8));
        tabFacture->setTabText(tabFacture->indexOf(tabFact1), QApplication::translate("Qfacture", "Editer une facture", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = fList->horizontalHeaderItem(0);
        ___qtablewidgetitem10->setText(QApplication::translate("Qfacture", "R\303\251f\303\251rence", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = fList->horizontalHeaderItem(1);
        ___qtablewidgetitem11->setText(QApplication::translate("Qfacture", "Date", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = fList->horizontalHeaderItem(2);
        ___qtablewidgetitem12->setText(QApplication::translate("Qfacture", "Client", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem13 = fList->horizontalHeaderItem(3);
        ___qtablewidgetitem13->setText(QApplication::translate("Qfacture", "Montant", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem14 = fList->horizontalHeaderItem(4);
        ___qtablewidgetitem14->setText(QApplication::translate("Qfacture", "Paiement", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem15 = fList->horizontalHeaderItem(5);
        ___qtablewidgetitem15->setText(QApplication::translate("Qfacture", "Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem16 = fList->horizontalHeaderItem(6);
        ___qtablewidgetitem16->setText(QApplication::translate("Qfacture", "Commentaire", 0, QApplication::UnicodeUTF8));
        tabFacture->setTabText(tabFacture->indexOf(tabFact2), QApplication::translate("Qfacture", "Liste des factures", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tFacture), QApplication::translate("Qfacture", "Factures", 0, QApplication::UnicodeUTF8));
        sGroupCa->setTitle(QApplication::translate("Qfacture", "Chiffre d'affaires", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("Qfacture", "Ann\303\251e de calcul", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tStat), QApplication::translate("Qfacture", "Statistiques", 0, QApplication::UnicodeUTF8));
        menu_Menu->setTitle(QApplication::translate("Qfacture", "&Menu", 0, QApplication::UnicodeUTF8));
        menu_Aide->setTitle(QApplication::translate("Qfacture", "&Aide", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Qfacture: public Ui_Qfacture {};
} // namespace Ui

QT_END_NAMESPACE

#endif // QFACTURE_H
