#!/bin/sh
# install.sh - build the program & install it

# chemin absolu vers le script
SCRIPT=$(readlink -f $0)
# chemin absolu vers le répertoire contenant le script
SCRIPTPATH="`dirname $SCRIPT`/"

echo "\n*** \033[32mCleaning the project...\033[0m\n"
make clean

echo "\n*** \033[32mGenerating makefile...\033[0m\n"
if [ -f /usr/bin/qmake-qt4 ];
then 
    echo "    Using qmake-qt4."
    qmake-qt4 Qfacture.pro
else
    echo "    Using qmake."
    qmake Qfacture.pro
fi

echo "\n*** \033[32mBuilding the program...\033[0m\n"
make

echo "\n*** \033[32mInstalling the program...\033[0m\n"
sudo cp bin/Qfacture /usr/local/bin/qfacture
sudo cp dist/qfacture.png /usr/share/pixmaps/qfacture.png
sudo cp dist/qfacture.desktop /usr/share/applications/qfacture.desktop

echo "\n*** \033[32mCreating the database...\033[0m\n"

echo -n "User: "
read LOGIN

mysql -h localhost -u $LOGIN -p < "$SCRIPTPATH"qfacture_create_database.sql

echo "*** \033[32mEND OF INSTALLATION\033[0m ***\n"
echo "If /usr/local/bin is not in your PATH, you need to create "
echo "a symbolic link in /usr/bin to /usr/local/bin/qfacture :"
echo "cd /usr/bin && sudo ln -s /usr/local/bin/qfacture\n"

echo "For more informations : http://github.com/jpec/Qfacture/wiki"