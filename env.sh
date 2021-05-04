#!/bin/bash -l 

ridx=$1

export OUTDIR=/tmp/$USER/opticks/CSGOptiXGGeo/$(CSGOptiXVersion)/$ridx    # see CSGOptiX/tests/CSGOptiXVersion.cc
mkdir -p $OUTDIR

export CUDA_VISIBLE_DEVICES=0


