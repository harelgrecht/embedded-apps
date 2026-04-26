#!/bin/bash -x
cd /home/must/Installers
tar -xzvf cmake-3.30.8-linux-aarch64.tar.gz

cp -r /home/must/Installers/cmake-3.30.8-linux-aarch64/bin /usr
cp -r /home/must/Installers/cmake-3.30.8-linux-aarch64/share /usr

pushd /home/must/must_core/build

cmake -G "Unix Makefiles" ..
cmake --build . -j=4

popd

sudo cp StartMust.service /etc/systemd/system/
chmod +x StartMust.sh
sudo systemctl enable StartMust

sync