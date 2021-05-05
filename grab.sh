#!/bin/bash -l 


from=P:/tmp/$USER/opticks/CSGOptiXGGeo/ 
to=/tmp/$USER/opticks/CSGOptiXGGeo/

rsync -zarv --progress --include="*/" --include="*.jpg" --exclude="*" "$from" "$to"



