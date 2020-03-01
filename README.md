# PVD-Equipment

You have install Qt Creator 4.5 on UBUNTU 16.04
https://download.qt.io/official_releases/qtcreator/4.5/4.5.0/
this is important because is Qt Creator 4.5 GPL licenze

You have install MSQL 
sudo apt-get update
sudo apt-get install mysql-server
sudo mysql_secure_installation
sudo mysql_install_db

you have confifure the  QT4.5 MSQL pugin 
cd $HOME/Qt/5.4/Src/qtbase/src/plugins/sqldrivers/mysql
$HOME/Qt/5.4/gcc_64/bin/qmake INCLUDEPATH+=/usr/include/mysql LIBS+=-L/usr/lib/x86_64-linux-gnu mysql.pro
make
make install

you have to install phpmyadmin 
you have to create the account (login)  and (password)

in the consoleplc folder you can find the the server PLC source coude
