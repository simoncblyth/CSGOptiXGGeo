#!/bin/bash -l 

usage(){ cat << EOU

With solid 0 (global remainder) it looks like transforms are not being applied, 
getting pileup at origin::

   EYE=-0.05,0.0,0.0,1.0 ./CSGOptiXGGeo.sh 

EOU
}

source ./env.sh $*

bin=CSGOptiXGGeoRender
ridx=$1

case $ridx in 
  0) TMIN=0.4 EYE=-0.4,0.0,0.0,1.0  CAMERATYPE=1 $bin $ridx  ;;
  1) TMIN=0.5 EYE=-0.8,0.0,0.0,1.0  CAMERATYPE=1 $bin $ridx  ;;
  2) TMIN=0.5 EYE=-0.8,0.0,0.0,1.0  CAMERATYPE=1 $bin $ridx  ;;
  3) TMIN=0.5 EYE=-0.8,0.0,0.0,1.0  CAMERATYPE=1 $bin $ridx  ;;
  4) TMIN=0.5 EYE=-1.0,0.0,0.0,1.0  CAMERATYPE=1 $bin $ridx  ;;
  5) TMIN=0.5 EYE=-1.0,0.0,-0.2,1.0 CAMERATYPE=1 $bin $ridx  ;;
  6) TMIN=0.5 EYE=-1.0,0.0,-0.2,1.0 CAMERATYPE=1 $bin $ridx  ;;    ## Greek Temple : dias too small for columns
  7) TMIN=0.5 EYE=-1.0,0.0,0.0,1.0  CAMERATYPE=1 $bin $ridx  ;;
  8) TMIN=0.5 EYE=-1.0,0.0,0.0,1.0  CAMERATYPE=1 $bin $ridx  ;;     ## giant crazy sphere prim, blank render 
  9) TMIN=0.7 EYE=-0.8,0.0,0.5,1.0  CAMERATYPE=1 $bin $ridx  ;;
  *) TMIN=0.5 EYE=-0.8,0.0,0.0,1.0  $bin $ridx  ;;
esac

jpg=$OUTDIR/pixels.jpg
echo $jpg
ls -l $jpg

if [ "$(uname)" == "Darwin" ]; then
   open $jpg
fi

exit 0 
