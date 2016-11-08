#!/bin/bash
# installation script for tara-sdk 

#Finding the OS, Arch and LTS version
. /etc/lsb-release
OS=$DISTRIB_ID
ARCH=$(uname -m | sed 's/x86_//;s/i[3-6]86/32/')
VER=$DISTRIB_RELEASE


#Setting the directories
prefix="/usr/local/tara-sdk"
bindir="${prefix}/bin"
libdir="${prefix}/lib"

#commands 
MKDIR_P="/bin/mkdir -p"
PWD=`pwd`


#Printing the found OS, Arch and LTS version
echo ""
echo "Installing Tara-SDK"
echo ""
echo "OS : "$OS
echo "Architecure : "$ARCH
echo "LTS version : "$VER
echo ""


#Checking if the OS is Ubuntu
if [ "$OS" != "Ubuntu" ]; then
	echo "Not a ubuntu version"

else
	#Checking if there is a prebuilt directory for the LTS version
	if [ -d $PWD/$OS-$VER ]; then
		echo "Directory for the current LTS version is found"

		if [ $ARCH -eq 32 ]; then
	                cd $PWD/$OS-$VER/binary && chmod +x *
        	elif [ $ARCH -eq 64 ]; then
	                cd $PWD/$OS-$VER/binary_x64 && chmod +x *
        	fi
		
		currentdir=$PWD
			
		#1. Remove binary from the prefix
		echo "Removing the already installed binaries"
		if test -d "$bindir"; then
		    cd "$bindir" && rm -rf *;
		fi

		#Remove library from the prefix
                echo "Removing the already installed libraries"
                echo ""
                if test -d "$libdir"; then
                    cd "$libdir" && rm -rf *;
                fi


		#2. Install the binary from the current path to the prefix
		echo "Installing binaries"
		echo "Creating a bin directory"		
		test -z "$bindir" || $MKDIR_P "$bindir";
		echo "Installing the binaries to the bin directory created"
		cp -rpf  $currentdir/* $bindir;
		
		#Install the libraries to the prefix
	        echo "Installing libraries"
                echo "Creating a lib directory"         
                test -z "$libdir" || $MKDIR_P "$libdir";
                echo "Installing the libraries to the lib directory created"
		if [ $ARCH -eq 32 ]; then      
			cp -rpf  $currentdir/../lib/* $libdir;
		elif [ $ARCH -eq 64 ]; then
			cp -rpf  $currentdir/../lib_x64/* $libdir;
		fi


		#3. Creating a .conf file for library loading
		if [ -f /etc/ld.so.conf.d/00-tara.conf ];then
			rm  /etc/ld.so.conf.d/00-tara.conf
		fi
		
		echo ""	
		echo "Creating a conf file (/etc/ld.so.conf.d/00-tara.conf) to link the shared libraries required for Tara-SDK applications"
		echo "#Tara dependency Lib directory" >> /etc/ld.so.conf.d/00-tara.conf
		echo "$libdir" >> /etc/ld.so.conf.d/00-tara.conf
		ldconfig
					
		
		#4. Adding the path to use sudo	
		sed -i '/tara/d' /etc/profile
		echo "Adding the path variable in /etc/profile"
		echo "export PATH=\$PATH:$bindir" >> /etc/profile


		#5. Adding the path to bashrc for autocomplete
		echo "Adding the path variable in ~/.bashrc for auto complete"
		if [ -f $HOME'/'.bashrc ];then
			sed -i '/tara/d'  $HOME/.bashrc			
			echo "PATH=\$PATH:$bindir" >> $HOME'/'.bashrc
		else
			echo "bashrc not found"
		fi
	
		echo ""
		echo "Installation success"	
		echo "Tara-SDK installed in the directory $prefix"	
		echo ""
		echo "Source the bashrc to reflect the environment changes"
		echo ". ~/.bashrc"
		echo ""
	else
		echo "Directory for the current LTS version is not found"
	fi
fi
