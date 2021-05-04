#!/bin/bash -l 

ridx=$1

optix_version(){ perl -ne 'm,^#define OPTIX_VERSION (\d*),&& print $1' ${OPTICKS_OPTIX_PREFIX}/include/optix.h ; }
export OPTIX_VERSION=$(optix_version)

export OUTDIR=/tmp/$USER/opticks/CSGOptiXGGeo/${OPTIX_VERSION}/$ridx
mkdir -p $OUTDIR

export CUDA_VISIBLE_DEVICES=0


