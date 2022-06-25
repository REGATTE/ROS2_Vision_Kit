!#/usr/bin/env bash

echo "Locale Check"
locale  # check for UTF-8

sudo apt update && sudo apt install locales
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8

locale  # verify settings

# enbaling Universe Repository
sudo apt install software-properties-common
sudo add-apt-repository universe
sudo apt-get install -y curl gnupg2 lsb-release build-essential

# Setup Sources
echo "setting up Sources"
sudo apt update && sudo apt install curl gnupg lsb-release
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
    
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(source /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# install ROS2 Packages
echo "installing ros2 packages"
sudo apt update
sudo apt upgrade
sudo apt install ros-humble-desktop

# source repo's
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
sudo apt install python3-colcon-common-extensions
sudo apt install ros-humble-eigen3-cmake-module
sudo pip3 install -U empy pyros-genmsg setuptools
sudo apt install ros-humble-gazebo-ros-pkgs

sudo rosdep init
    rosdep update

echo "Done installing ROS2 - HUMBLE"

mkdir -p ros2_ws/src 
cd ros2_ws
colcon build
cd
    
echo "source ~/ros2_ws/install/setup.sh" >> ~/.bashrc
source ~/.bashrc