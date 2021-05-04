#include "OPTICKS_LOG.hh"
#include "Opticks.hh"
#include "GGeo.hh"

#include "sutil_vec_math.h"
#include "CSGFoundry.h"
#include "CSGOptiX.h"
#include "CXUtil.h"
#include "Converter.h"

int main(int argc, char** argv)
{
    int repeatIdx = argc > 1 ? atoi(argv[1]) : 0 ; 
    int primIdx = -1 ; 
    int partIdxRel = -1 ; 

    std::cout << "repeatIdx " << repeatIdx << std::endl; 

    OPTICKS_LOG(argc, argv);
    Opticks ok(argc, argv);
    ok.configure(); 

    bool dump = CXUtil::GetEValue<bool>("DUMP", false ); 
    GGeo* ggeo = GGeo::Load(&ok); 
    if(dump) ggeo->dumpParts("CSGOptiXGGeo.main", repeatIdx, primIdx, partIdxRel ) ;

    CSGFoundry foundry ; 
    Converter conv(&foundry, ggeo, dump) ; 
    conv.convert(repeatIdx, primIdx, partIdxRel); 

    CSGOptiX cx(&foundry); 

    foundry.dump(); 
    foundry.upload();   // uploads nodes, planes, transforms

    unsigned target = ok.getTarget(); 
    const CSGSolid* so = foundry.getSolid(target); 
    LOG(info) 
        << " --target " << target 
        << " so " << so->center_extent 
        ; 

    const float4 gce = so->center_extent ; 
    glm::vec4 ce(gce.x,gce.y,gce.z, gce.w*1.4f );   // defines the center-extent of the region to view

    float tmin = CXUtil::GetEValue<float>("TMIN", 1.0) ;
    float tmax = CXUtil::GetEValue<float>("TMAX", 10000.0) ;

    std::string top = CXUtil::GetEValue<std::string>("TOP", "g0"); 
    const char* tspec = top.c_str(); 

    cx.setCE(ce, tmin, tmax); 
    cx.render( tspec );  

    return 0 ; 
}
