# PVD-Equipment

You must install Qt Creator 4.5.0 on UBUNTU 16.04


You must install MSQL server
sudo apt-get update
sudo apt-get install mysql-server
sudo mysql_secure_installation
sudo mysql_install_db

you must confifure the MSQL pugin 
cd $HOME/Qt/5.4/Src/qtbase/src/plugins/sqldrivers/mysql
$HOME/Qt/5.4/gcc_64/bin/qmake INCLUDEPATH+=/usr/include/mysql LIBS+=-L/usr/lib/x86_64-linux-gnu mysql.pro
make
make install

in the consoleplc folder you can find the the server PLC source coude
