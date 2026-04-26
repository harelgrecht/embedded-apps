#!/bin/bash

# transfer all requierd files into MUST
hostname=must
target_ip=192.168.3.10
target_folder=/home/must
password=root

curr_dir=`pwd`   
echo $curr_dir  
echo "TEST"
sshpass -p "$password" scp -r $curr_dir/* $hostname@$target_ip:$target_folder


sshpass -p "$password" ssh -o StrictHostkeyChecking=no  "$hostname@$target_ip" "echo "$password" | sudo -S bash /home/must/Installers/InstallInMust.sh"

