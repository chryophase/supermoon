#!/bin/bash

set -e

date

#################################################################
# Update Ubuntu and install prerequisites for running Supermoon   #
#################################################################
sudo apt-get update
#################################################################
# Build Supermoon from source                                     #
#################################################################
NPROC=$(nproc)
echo "nproc: $NPROC"
#################################################################
# Install all necessary packages for building Supermoon           #
#################################################################
sudo apt-get install -y qt4-qmake libqt4-dev libminiupnpc-dev libdb++-dev libdb-dev libcrypto++-dev libqrencode-dev libboost-all-dev build-essential libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libssl-dev ufw git
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev

# By default, assume running within repo
repo=$(pwd)
file=$repo/src/supermoond
if [ ! -e "$file" ]; then
	# Now assume running outside and repo has been downloaded and named supermoon
	if [ ! -e "$repo/supermoon/build.sh" ]; then
		# if not, download the repo and name it supermoon
		git clone https://github.com/supermoond/source supermoon
	fi
	repo=$repo/supermoon
	file=$repo/src/supermoond
	cd $repo/src/
fi
make -j$NPROC -f makefile.unix

cp $repo/src/supermoond /usr/bin/supermoond

################################################################
# Configure to auto start at boot                                      #
################################################################
file=$HOME/.supermoon
if [ ! -e "$file" ]
then
        mkdir $HOME/.supermoon
fi
printf '%s\n%s\n%s\n%s\n' 'daemon=1' 'server=1' 'rpcuser=u' 'rpcpassword=p' | tee $HOME/.supermoon/supermoon.conf
file=/etc/init.d/supermoon
if [ ! -e "$file" ]
then
        printf '%s\n%s\n' '#!/bin/sh' 'sudo supermoond' | sudo tee /etc/init.d/supermoon
        sudo chmod +x /etc/init.d/supermoon
        sudo update-rc.d supermoon defaults
fi

/usr/bin/supermoond
echo "Supermoon has been setup successfully and is running..."

