#!/bin/bash -l 

usage(){ cat << EOU

With solid 0 (global remainder) it looks like transforms are not being applied, 
getting pileup at origin::

   EYE=-0.05,0.0,0.0,1.0 ./CSGOptiXGGeo.sh 

EOU
}

source ./env.sh $*

[ -z "$OPTIX_VERSION" ] && echo $BASH_SOURCE FAILED TO IDENTIFY OPTIX_VERSION && exit 1


ridx=$1

case $ridx in 
  0) EYE=-0.05,0.0,0.0,1.0 CSGOptiXGGeo $ridx  ;;
  1) TMIN=40.0 EYE=-0.8,0.0,0.0,1.0  CAMERATYPE=1 CSGOptiXGGeo $ridx  ;;
  2) TMIN=150.0 EYE=-0.8,0.0,0.0,1.0  CAMERATYPE=1 CSGOptiXGGeo $ridx  ;;
  3) TMIN=100.0 EYE=-1.0,-1.0,0.8,1.0  CSGOptiXGGeo $ridx  ;;
  4) TMIN=100.0 EYE=-1.0,0.0,0.0,1.0 CAMERATYPE=1 CSGOptiXGGeo $ridx  ;;
  5) TMIN=100.0 EYE=-1.0,0.0,-0.2,1.0  CSGOptiXGGeo $ridx  ;;
  6) TMIN=100.0 EYE=-1.0,0.0,-0.2,1.0  CSGOptiXGGeo $ridx  ;;    ## Greek Temple : dias too small for columns
  7) TMIN=100.0 EYE=-1.0,0.0,0.0,1.0  CSGOptiXGGeo $ridx  ;;
  8) TMIN=100.0 EYE=-1.0,0.0,0.0,1.0  CSGOptiXGGeo $ridx  ;;     ## giant crazy sphere prim, blank render 
  9) TMIN=100.0 EYE=-1.0,0.0,0.0,1.0  CSGOptiXGGeo $ridx  ;;
  *) TMIN=100.0 EYE=-0.8,0.0,0.0,1.0  CSGOptiXGGeo $ridx  ;;
esac


jpg=$OUTDIR/pixels.jpg
echo $jpg
ls -l $jpg

if [ "$(uname)" == "Darwin" ]; then
   open $jpg
fi


exit 0 
