#!/bin/sh


sudo apt-get update -q

# install Qt
sudo apt-get install libqt4-dev qt4-qmake 

# install boost 
sudo apt-get install libboost-thread-dev libboost-system-dev libboost-filesystem-dev

# install Doxygen
sudo apt-get install doxygen

#sudo apt-get install libroot-gui-ged-dev libroot-hist5.34 libroot-hist-dev libroot-math-physics5.34 libroot-math-physics-dev libroot-core5.34 libroot-net-dev libroot-core-dev libroot-geom5.34 libroot-html-dev libroot-geom-dev libroot-io5.34 libroot-graf2d-gpad5.34 libroot-io-dev libroot-graf2d-gpad-dev libroot-graf2d-graf5.34 libroot-graf2d-graf-dev libroot-math-unuran-dev libroot-graf2d-postscript5.34 libroot-graf2d-postscript-dev libroot-graf3d-eve5.34 libroot-graf3d-eve-dev libroot-math-mathcore5.34 libroot-tmva-dev libroot-graf3d-g3d5.34 libroot-math-mathcore-dev libroot-tree5.34 libroot-graf3d-g3d-dev libroot-math-mathmore5.34 libroot-tree-dev libroot-graf3d-gl5.34 libroot-math-mathmore-dev libroot-graf3d-gl-dev libroot-math-matrix5.34 libroot-gui5.34 libroot-math-matrix-dev libroot-gui-dev libroot-gui-ged5.34 libroot-net5.34 root-plugin-io-xml root-plugin-geom-geompainter root-plugin-gui-qt root-plugin-graf2d-asimage root-plugin-gui-sessionviewer root-plugin-graf2d-qt root-plugin-hist-hbook root-plugin-graf2d-x11 root-plugin-hist-histpainter root-plugin-graf3d-x3d root-system root-system-bin root-system-common root-system-rootd 
# install log4cxx
sudo apt-get install liblog4cxx10 liblog4cxx10-dev

# install root with Qt backend
#cd && wget https://root.cern.ch/download/root_v5.34.36.Linux-ubuntu14-x86_64-gcc4.8.tar.gz
#tar -xf root_v5.34.36.Linux-ubuntu14-x86_64-gcc4.8.tar.gz
#source ~/root/bin/thisroot.sh
# cd root
# ./configure --enable-qt && make -j6 --quiet && . ./bin/thisroot.sh
# cd ..
