!#/usr/bin/env bash

cd

sudo apt update && sudo apt upgrade

# Installs curl if not already installed
sudo apt install curl

# Install pip if not already installed
sudo apt-get install python3-pip

# Install ntfy, a desktop notification service to notify updates and error messages
sudo pip3 install ntfy

sudo pip3 install opencv-python
sudo pip3 install opencv-contrib-python

ntfy send "Installing ROS2...."
ntfy send "Checking Ubuntu Version"

if [[ $(lsb_release -sc) == "focal" ]]; then
    ntfy send "Installing ROS2 FOXY"
    sudo bash Installation/ROS2/Foxy.sh

elif [[ $(lsb_release -sc) == "jammy" ]]; then
    ntfy send 'Installing ROS2 Humble'
    sudo bash Installation/ROS2/Humble.sh
else
    ntfy send "Ubuntu Version not compatible."
fi