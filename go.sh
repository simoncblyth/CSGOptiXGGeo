#!/bin/bash -l

msg="=== $BASH_SOURCE :"

opticks-
om-

sdir=$(pwd)
name=$(basename $sdir) 
bdir=/tmp/$USER/opticks/$name/build 
rm   -rf $bdir && mkdir -p $bdir && cd $bdir && pwd 



cmake $sdir \
    -DCMAKE_BUILD_TYPE=Debug \
    -DOPTICKS_PREFIX=$(om-prefix) \
    -DCMAKE_INSTALL_PREFIX=$(om-prefix) \
    -DCMAKE_MODULE_PATH=$(om-home)/cmake/Modules
[ $? -ne 0 ] && echo $msg : config error && exit 1 
     
make
[ $? -ne 0 ] && echo $msg : make error && exit 2 

make install   
[ $? -ne 0 ] && echo $msg : install error && exit 3

exe=$(opticks-prefix)/lib/$name
echo "running exe $exe"
eval $exe
[ $? -ne 0 ] && echo $msg : run error && exit 3

exit 0 

