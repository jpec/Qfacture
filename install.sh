#!/bin/sh
# install.sh - build the program & install it

clear
echo "*** Cleaning the project..."
make clean

echo "*** Generating makefile..."
if [ -f /usr/bin/qmake-qt4 ];
then 
    echo "    Using qmake-qt4."
    qmake-qt4 Qfacture.pro
else
    echo "    Using qmake."
    qmake Qfacture.pro
fi

echo "*** Building the program..."
make

echo "*** Installing the program..."
sudo cp bin/Qfacture /usr/local/bin/qfacture
sudo cp art/qfacture.png /usr/share/pixmaps/qfacture.png
sudo cp qfacture.desktop /usr/share/applications/qfacture.desktop

echo "*** END OF INSTALLATION ***"
echo " "
echo "Now you have to configure you SQL database."
echo " "
echo "If /usr/local/bin is not in your PATH, you need to create "
echo "a symbolic link in /usr/bin to /usr/local/bin/qfacture :"
echo "cd /usr/bin && sudo ln -s /usr/local/bin/qfacture"
echo " "
echo "For more informations : http://github.com/jpec/Qfacture/wiki"