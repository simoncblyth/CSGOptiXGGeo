#include <iostream>
#include <iomanip>
#include <vector>

#include "NGLMExt.hpp"
#include "GLMFormat.hpp"

#include "GGeo.hh"
#include "GGeoLib.hh"
#include "GParts.hh"

#include "sutil_vec_math.h"
#include "CSGFoundry.h"
#include "CSGSolid.h" 
#include "CSGPrim.h" 
#include "CSGNode.h" 

#include "PLOG.hh"
#include "Converter.h"


Converter::Converter(CSGFoundry* foundry_, const GGeo* ggeo_, bool dump_ ) 
    : 
    foundry(foundry_),
    ggeo(ggeo_),
    dump(dump_)
{
}

void Converter::convert(int repeatIdx,  int primIdx, int partIdxRel )
{
    if( repeatIdx > -1 && primIdx > -1 && partIdxRel > -1 )
    {   
        convert_(repeatIdx, primIdx, partIdxRel);
    }
    else if( repeatIdx > -1 && primIdx > -1  )
    {   
        convert_(repeatIdx, primIdx);
    }
    else if( repeatIdx > -1 )
    {   
        convert_(repeatIdx);
    }
    else
    {
        convert_();
    }
} 

void Converter::convert_()
{
    unsigned numRepeat = ggeo->getNumMergedMesh(); 
    for(unsigned repeatIdx=0 ; repeatIdx < numRepeat ; repeatIdx++)
    {
        convert_(repeatIdx); 
    }
}

void Converter::convert_( unsigned repeatIdx )
{
    const GParts* comp = ggeo->getCompositeParts(repeatIdx) ; 
    unsigned numPrim = comp->getNumPrim();

    std::stringstream ss ; 
    ss << "r" << std::setfill('0') << std::setw(3) << repeatIdx ; 
    std::string s = ss.str();  
    const char* label = s.c_str() ;

    LOG(info)
        << " repeatIdx " << repeatIdx 
        << " numPrim " << numPrim
        << " label " << label 
        ;   

    CSGSolid* so = foundry->addSolid(numPrim, label );
    assert(so); 

    for(unsigned primIdx=0 ; primIdx < numPrim ; primIdx++)
    {   
        convert_(repeatIdx, primIdx); 
    }   
}


void Converter::convert_( unsigned repeatIdx, unsigned primIdx )
{
    const GParts* comp = ggeo->getCompositeParts(repeatIdx); 
    unsigned numPrim = comp->getNumPrim();
    assert( primIdx < numPrim ); 
    unsigned numParts = comp->getNumParts(primIdx) ;

    unsigned sbtIdx = primIdx ; // should be absolute ?

    LOG(info)
        << " sbtIdx " << sbtIdx
        << " repeatIdx " << repeatIdx 
        << " primIdx " << primIdx 
        << " numPrim " << numPrim
        << " numParts " << numParts 
        ;   

    CSGPrim* pr = foundry->addPrim(numParts); 
    assert(pr) ; 
    pr->setSbtIndexOffset(sbtIdx) ;

    for(unsigned partIdxRel=0 ; partIdxRel < numParts ; partIdxRel++ )
    {
        convert_(repeatIdx, primIdx, partIdxRel);
    }

    // need to combine AABB ?
    // pr->setAABB( n->AABB() );
}


void Converter::convert_(unsigned repeatIdx, unsigned primIdx, unsigned partIdxRel )
{
    const GParts* comp = ggeo->getCompositeParts(repeatIdx);
    unsigned partOffset = comp->getPartOffset(primIdx) ;
    //unsigned tranOffset = comp->getTranOffset(primIdx) ;
    unsigned numParts = comp->getNumParts(primIdx) ;
    assert( partIdxRel < numParts );
    unsigned partIdx = partOffset + partIdxRel ;
    
    convertPart(comp, primIdx, partIdx); 
}


void Converter::convertPart(const GParts* comp, unsigned primIdx, unsigned partIdx )
{
    unsigned tc = comp->getTypeCode(partIdx);
    unsigned idx = comp->getIndex(partIdx);
    assert( idx == partIdx ); 

    unsigned gt = comp->getGTransform(partIdx);
    //unsigned bnd = comp->getBoundary(partIdx);
    //std::string  bn = comp->getBoundaryName(partIdx);
    //const char*  tn = comp->getTypeName(partIdx);
    std::string tag = comp->getTag(partIdx); 

    unsigned tranIdx = 0 ; 
    if( gt > 0 )
    {
        glm::mat4 t = comp->getTran(gt-1,0) ;
        glm::mat4 v = comp->getTran(gt-1, 1); 
        Tran<float> tv(t, v); 
        tranIdx = 1 + foundry->addTran(tv);   
        //std::cout << gpresent( "t", t ) << std::endl ;
    }

    const float* param = comp->getPartValues(partIdx, 0, 0 );  
    if(dump)
    {
        std::cout 
            << tag 
            << " primIdx " << std::setw(3) << primIdx 
            << " partIdx " << std::setw(3) << partIdx 
            << " tranIdx " << std::setw(4) << tranIdx 
            << " param: " 
            ;
        for(int p=0 ; p < 6 ; p++) std::cout << *(param+p) << " " ;  
        std::cout << std::endl ; 
    }

    const float* aabb = nullptr ;  
    CSGNode* n = foundry->addNode(CSGNode::Make(tc, param, aabb ));
    n->setIndex(partIdx); 
    n->setAABBLocal(); 
    n->setTransform(tranIdx); 
}


