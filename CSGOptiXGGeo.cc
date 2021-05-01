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
    void dump(); 
    void dump_(unsigned solidIdx); 
    void dump(unsigned solidIdx); 
    void dump(unsigned solidIdx, unsigned primIdx); 
    void dump(unsigned solidIdx, unsigned primIdx, unsigned partIdxRel );
}; 

CSGOptiXGGeo::CSGOptiXGGeo(GGeo* ggeo_)
    :
    ggeo(ggeo_),
    geolib(ggeo->getGeoLib())
{
}

void CSGOptiXGGeo::dump()
{
    unsigned nmm = geolib->getNumMergedMesh();
    LOG(info) << " nmm " << nmm ; 
    for(unsigned i=0 ; i < nmm ; i++) dump(i); 
}

void CSGOptiXGGeo::dump_(unsigned solid_idx)
{
    LOG(info) << " solid_idx " << solid_idx ; 
    GParts* comp = geolib->getCompositeParts(solid_idx); 

    unsigned num_subs = comp->getNumSubs();
    unsigned num_prim = comp->getNumPrim();
    unsigned num_parts = comp->getNumParts();
    std::cout 
        << " comp.NumSubs  " << std::setw(5) << num_subs 
        << " comp.NumPrim " << std::setw(5) << num_prim
        << " comp.NumParts " << std::setw(5) << num_parts
        << " PrimNumParts (" ; 
        ;

    //comp->reconstructPartsPerPrim();   NOPE THIS IS FOR THE OLD PARTLIST 

    for(unsigned j=0 ; j < std::min(num_prim, 10u) ; j++) std::cout << comp->getPrimNumParts(j) << " " ; 
    std::cout << ") " << std::endl ;  

    comp->dump();  

    for(unsigned primIdx=0 ; primIdx < num_prim ; primIdx++)
    {
        unsigned partOffset = comp->getPartOffset(primIdx) ; 
        unsigned numParts = comp->getNumParts(primIdx) ;
        std::cout 
            << " primIdx "    << primIdx
            << " partOffset " << std::setw(4) << partOffset 
            << " numParts "   << std::setw(4) << numParts
            << " tranOffset " << std::setw(4) << comp->getTranOffset(primIdx)
            << " planOffset " << std::setw(4) << comp->getPlanOffset(primIdx)
            << std::endl
            ;
    }
}


void CSGOptiXGGeo::dump(unsigned solidIdx)
{
    GParts* comp = geolib->getCompositeParts(solidIdx); 
    unsigned numPrim = comp->getNumPrim();
    LOG(info)
        << " solidIdx " << solidIdx 
        << " numPrim " << numPrim
        ;

    for(unsigned primIdx=0 ; primIdx < numPrim ; primIdx++)
    {
        dump(solidIdx, primIdx); 
    }
}


void CSGOptiXGGeo::dump(unsigned solidIdx, unsigned primIdx )
{
    GParts* comp = geolib->getCompositeParts(solidIdx); 
    unsigned numPrim = comp->getNumPrim();
    unsigned numParts = comp->getNumParts(primIdx) ;
    unsigned tranOffset = comp->getTranOffset(primIdx) ;
    unsigned partOffset = comp->getPartOffset(primIdx) ;
    LOG(info) 
        << " solidIdx " << solidIdx 
        << " primIdx/numPrim " << primIdx << "/" << numPrim 
        << " numParts " << numParts
        << " tranOffset " << tranOffset
        << " partOffset " << partOffset
        ; 
    for(unsigned partIdxRel=0 ; partIdxRel < numParts ; partIdxRel++ )
    {
        dump(solidIdx, primIdx, partIdxRel); 
    } 
}
void CSGOptiXGGeo::dump(unsigned solidIdx, unsigned primIdx, unsigned partIdxRel )
{
    GParts* comp = geolib->getCompositeParts(solidIdx); 
    unsigned partOffset = comp->getPartOffset(primIdx) ; 
    unsigned numParts = comp->getNumParts(primIdx) ;
    assert( partIdxRel < numParts );  
    unsigned partIdx = partOffset + partIdxRel ; 
    comp->dumpPart(partIdx); 
}



int main(int argc, char** argv)
{
    int solidIdx = argc > 1 ? atoi(argv[1]) : -1 ; 
    int primIdx  = argc > 2 ? atoi(argv[2]) : -1 ; 
    int partIdxRel  = argc > 3 ? atoi(argv[3]) : -1 ; 

    OPTICKS_LOG(argc, argv);

    Opticks ok(argc, argv);

    GGeo* gg = GGeo::Load(&ok); 


    CSGOptiXGGeo cog(gg); 

    if( solidIdx > -1 && primIdx > -1 && partIdxRel > -1 )
    {
        cog.dump(solidIdx, primIdx, partIdxRel); 
    }
    else if( solidIdx > -1 && primIdx > -1  )
    {
        cog.dump(solidIdx, primIdx); 
    }
    else if( solidIdx > -1 )
    {
        cog.dump(solidIdx); 
    }
    else
    {
        LOG(info) << " gg " << gg ;  
        gg->dumpParts(); 
    }

    return 0 ; 
}

