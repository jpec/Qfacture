CREATE TABLE  `qfacture_db`.`template` (
  `Id` smallint(5) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Id template',
  `Type` varchar(10) NOT NULL COMMENT 'Type template',
  `Sel` varchar(1) NOT NULL COMMENT 'Template sélectionné (pour le type)',
  `Name` varchar(255) NOT NULL COMMENT 'Désignation template',
  `Content` text NOT NULL COMMENT 'Contenu du template',
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='Table des templates de facture';

INSERT INTO `qfacture_db`.`template` VALUES ( '', 'html', 'x', 'Thème par défaut', '<html>test</html>');
