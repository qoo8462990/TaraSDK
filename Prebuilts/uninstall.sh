#!/bin/bash
#Uninstallation script for tara-sdk


#Setting the directories
prefix="/usr/local/tara-sdk"

if [ -d $prefix ];then
	echo "Uninstalling tara-sdk"
	rm -rf $prefix
	echo "Removed : tara-sdk folder in /usr/local"
else
	echo "tara-sdk not found"
fi

#Removing a .conf file for library loading
if [ -f /etc/ld.so.conf.d/00-tara.conf ];then
	rm  /etc/ld.so.conf.d/00-tara.conf
	echo "Removed : conf file created to link the shared libraries"
fi
ldconfig

#Removing the path to use sudo    
sed -i '/tara/d' /etc/profile
echo "Removed : path exported to /etc/profile"


#Removing the path to bashrc for autocomplete
if [ -f $HOME'/'.bashrc ];then
	sed -i '/tara/d'  $HOME/.bashrc
	echo "Removed : path exported to bashrc"
fi
echo "Uninstalled"

