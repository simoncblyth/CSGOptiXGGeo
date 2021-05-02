#include <iostream>
#include <iomanip>

#include "OPTICKS_LOG.hh"
#include "Opticks.hh"

#include "GGeo.hh"
#include "GGeoLib.hh"
#include "GParts.hh"

#include "sutil_vec_math.h"
#include "CSGFoundry.h"


struct CSGOptiXGGeo
{
    GGeo*      ggeo ; 
    GGeoLib*   geolib ; 
    CSGFoundry foundry ; 

    CSGOptiXGGeo(GGeo* gg) ; 
}; 

CSGOptiXGGeo::CSGOptiXGGeo(GGeo* ggeo_)
    :
    ggeo(ggeo_),
    geolib(ggeo->getGeoLib())
{
}



int main(int argc, char** argv)
{
    int repeatIdx = argc > 1 ? atoi(argv[1]) : -1 ; 
    int primIdx  = argc > 2 ? atoi(argv[2]) : -1 ; 
    int partIdxRel  = argc > 3 ? atoi(argv[3]) : -1 ; 

    OPTICKS_LOG(argc, argv);

    Opticks ok(argc, argv);

    GGeo* gg = GGeo::Load(&ok); 

    gg->dumpParts("CSGOptiXGGeo.main", repeatIdx, primIdx, partIdxRel ) ;

    CSGOptiXGGeo cog(gg); 

    return 0 ; 
}

