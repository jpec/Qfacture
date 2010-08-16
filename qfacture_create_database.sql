-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.1.47


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema qfacture_db
--

CREATE DATABASE IF NOT EXISTS qfacture_db;
USE qfacture_db;

--
-- Definition of table `qfacture_db`.`article`
--
CREATE TABLE  `qfacture_db`.`article` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Id article',
  `Name` text COMMENT 'Désignation article',
  `Price` decimal(10,0) DEFAULT NULL COMMENT 'Prix unitaire',
  `Comment` text COMMENT 'Commentaire',
  `Disabled` tinyint(1) DEFAULT '0' COMMENT '1 -> désactivé',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Table des articles';

--
-- Dumping data for table `qfacture_db`.`article`
--
INSERT INTO `qfacture_db`.`article` VALUES  (1,'Article 1','120','Commentaire article 1',NULL);

--
-- Definition of table `qfacture_db`.`client`
--
CREATE TABLE  `qfacture_db`.`client` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Id du client',
  `Name` text NOT NULL COMMENT 'Nom du client',
  `Adress` text COMMENT 'Adresse ligne 1',
  `Adress2` text COMMENT 'Adresse ligne 2',
  `Zip` text COMMENT 'Code postal',
  `City` text NOT NULL COMMENT 'Ville',
  `Phone` text COMMENT 'Téléphone',
  `Mail` text COMMENT 'Email',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='Table des clients';

--
-- Dumping data for table `qfacture_db`.`client`
--
INSERT INTO `qfacture_db`.`client` VALUES  (1,'Client 1','Adresse client 1','Complément adresse client 1','80000','AMIENS','00.00.00.00.00','user@domain.org');

--
-- Definition of table `qfacture_db`.`facture`
--
CREATE TABLE  `qfacture_db`.`facture` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Id de la facture',
  `idClient` int(11) NOT NULL COMMENT 'Id du client',
  `Amount` decimal(10,0) DEFAULT NULL COMMENT 'Montant total de la facture',
  `Comment` text COMMENT 'Commentaire',
  `Payment` text COMMENT 'Mode de paiement',
  `Reference` text COMMENT 'Référence de la facture AAAAMMJJNNN',
  `Type` text COMMENT 'FACTU ou DEVIS',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Table des factures';

--
-- Dumping data for table `qfacture_db`.`facture`
--

--
-- Definition of table `qfacture_db`.`link`
--
CREATE TABLE  `qfacture_db`.`link` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Id lien',
  `idFacture` int(11) NOT NULL COMMENT 'Id facture',
  `idArticle` int(11) NOT NULL COMMENT 'Id article',
  `quantity` int(11) DEFAULT NULL COMMENT 'Quantité',
  `off` decimal(10,0) DEFAULT NULL COMMENT 'Remise (%)',
  `price` decimal(10,0) DEFAULT NULL COMMENT 'Prix unitaire',
  `amount` decimal(10,0) DEFAULT NULL COMMENT 'Montant total',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Liens facture - article';

--
-- Dumping data for table `qfacture_db`.`link`
--

--
-- Definition of table `qfacture_db`.`user`
--
CREATE TABLE  `qfacture_db`.`user` (
  `Name` text COMMENT 'Nom de l''utilisateur',
  `Adress` text COMMENT 'Adresse ligne 1',
  `Adress2` text COMMENT 'Adresse ligne2',
  `Zip` text COMMENT 'Code postal',
  `City` text COMMENT 'Ville',
  `Mail` text COMMENT 'Email',
  `Phone` text COMMENT 'Téléphone',
  `Siret` text COMMENT 'Siret',
  `Home` text COMMENT 'Site web',
  `Logo` blob COMMENT 'Logo',
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Id utilisateur',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='Table utilisateur de Kfacture';

--
-- Dumping data for table `qfacture_db`.`user`
--
INSERT INTO `qfacture_db`.`user` VALUES  ('Nom utilisateur','Adresse de l\'utilisateur','Complément adresse utilisateur\n','80000','Amiens','user@domain.com','00.00.00.00.00','SIRET NUMBER','http://julienpecqueur,com',0x31,1);



/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
