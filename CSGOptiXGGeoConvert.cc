#include "OPTICKS_LOG.hh"
#include "Opticks.hh"
#include "GGeo.hh"

#include "sutil_vec_math.h"
#include "CSGFoundry.h"

#include "Converter.h"

int main(int argc, char** argv)
{
    int repeatIdx = argc > 1 ? atoi(argv[1]) : -1 ; 
    int primIdx = -1 ; 
    int partIdxRel = -1 ; 

    std::cout << "repeatIdx " << repeatIdx << std::endl; 

    OPTICKS_LOG(argc, argv);
    Opticks ok(argc, argv);
    ok.configure(); 

    bool dump = false ; 
    GGeo* ggeo = GGeo::Load(&ok); 
    //ggeo->dumpParts("CSGOptiXGGeo.main", repeatIdx, primIdx, partIdxRel ) ;

    CSGFoundry foundry ; 
    Converter conv(&foundry, ggeo, dump) ; 
    conv.convert(repeatIdx, primIdx, partIdxRel); 

    const char* dir = "/tmp" ; 
    const char* rel = "CSGOptiXGGeo" ; 
    foundry.write(dir, rel); 

    CSGFoundry* fd = CSGFoundry::Load(dir, rel); 
    int cmp = CSGFoundry::Compare(&foundry, fd );  
    assert( cmp == 0 ); 

    return 0 ; 
}



