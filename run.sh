#!/bin/bash -l

msg="=== $BASH_SOURCE :"
sdir=$(pwd)
name=$(basename $sdir) 

export CUDA_VISIBLE_DEVICES=0

which $name
$name $*


