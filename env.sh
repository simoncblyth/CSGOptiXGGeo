#!/bin/bash -l 

ridx=$1
export OUTDIR=/tmp/$USER/opticks/CSGOptiXGGeo/$ridx
mkdir -p $OUTDIR

export CUDA_VISIBLE_DEVICES=1 


