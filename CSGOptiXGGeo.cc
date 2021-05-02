#include "OPTICKS_LOG.hh"
#include "Opticks.hh"
#include "GGeo.hh"

#include "sutil_vec_math.h"
#include "CSGFoundry.h"
#include "CSGOptiX.h"
#include "Converter.h"

int main(int argc, char** argv)
{
    int repeatIdx = argc > 1 ? atoi(argv[1]) : -1 ; 
    int primIdx  = argc > 2 ? atoi(argv[2]) : -1 ; 
    int partIdxRel  = argc > 3 ? atoi(argv[3]) : -1 ; 

    OPTICKS_LOG(argc, argv);
    Opticks ok(argc, argv);

    GGeo* ggeo = GGeo::Load(&ok); 
    ggeo->dumpParts("CSGOptiXGGeo.main", repeatIdx, primIdx, partIdxRel ) ;

    bool dump = true ; 
    CSGFoundry foundry ; 
    Converter conv(&foundry, ggeo, dump ) ; 
    conv.convert(repeatIdx, primIdx, partIdxRel); 

    CSGOptiX cx(&foundry); 

    foundry.dump(); 
    foundry.upload();   // uploads nodes, planes, transforms

    const float4 gce = make_float4( 0.f, 0.f, 0.f, 300.f );   
    glm::vec4 ce(gce.x,gce.y,gce.z, gce.w*1.4f );   // defines the center-extent of the region to view

    float tmin = 0.1 ; 
    float tmax = 10000.f ; 
    const char* tspec = "g0" ; 

    cx.setCE(ce, tmin, tmax); 
    cx.render( tspec );  

    return 0 ; 
}
