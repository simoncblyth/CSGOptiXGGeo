#include <iostream>
#include <iomanip>
#include <vector>

#include "NGLMExt.hpp"
#include "GLMFormat.hpp"

#include "GGeo.hh"
#include "GGeoLib.hh"
#include "GParts.hh"
#include "GMergedMesh.hh"

#include "sutil_vec_math.h"
#include "CSGFoundry.h"
#include "CSGSolid.h" 
#include "CSGPrim.h" 
#include "CSGNode.h" 
#include "qat4.h"
#include "AABB.h"

#include "CXUtil.h"
#include "PLOG.hh"
#include "Converter.h"


Converter::Converter(CSGFoundry* foundry_, const GGeo* ggeo_, bool dump_ ) 
    : 
    foundry(foundry_),
    ggeo(ggeo_),
    dump(dump_),
    splay(CXUtil::GetEValue<float>("SPLAY", 0.f ))
{
}


void Converter::convert(int repeatIdx,  int primIdx, int partIdxRel )
{
    if( repeatIdx > -1 && primIdx > -1 && partIdxRel > -1 )
    {   
        const GParts* comp = ggeo->getCompositeParts(repeatIdx) ; 
        convert_(comp, primIdx, partIdxRel);
    }
    else if( repeatIdx > -1 && primIdx > -1  )
    {   
        const GParts* comp = ggeo->getCompositeParts(repeatIdx) ; 
        convert_(comp, primIdx);
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

void Converter::addInstances(unsigned repeatIdx )
{
    unsigned nmm = ggeo->getNumMergedMesh(); 
    assert( repeatIdx < nmm ); 
    const GMergedMesh* mm = ggeo->getMergedMesh(repeatIdx); 
    unsigned num_inst = mm->getNumITransforms() ;

    LOG(info)
        << " nmm " << nmm
        << " repeatIdx " << repeatIdx
        << " num_inst " << num_inst 
        ;


    for(unsigned i=0 ; i < num_inst ; i++)
    {
        glm::mat4 it = mm->getITransform_(i); 
        qat4 instance(glm::value_ptr(it)) ;   
        unsigned ins_idx = foundry->inst.size() ;
        unsigned gas_idx = repeatIdx ; 
        unsigned ias_idx = 0 ; 
        instance.setIdentity( ins_idx, gas_idx, ias_idx );
        foundry->inst.push_back( instance );
    }
}


CSGSolid* Converter::convert_( unsigned repeatIdx )
{
    unsigned nmm = ggeo->getNumMergedMesh(); 
    assert( repeatIdx < nmm ); 
    const GMergedMesh* mm = ggeo->getMergedMesh(repeatIdx); 
    unsigned num_inst = mm->getNumITransforms() ;


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
        << " num_inst " << num_inst 
        ;   

    CSGSolid* so = foundry->addSolid(numPrim, label );
    assert(so); 

    AABB bb = {} ;

    for(unsigned primIdx=0 ; primIdx < numPrim ; primIdx++)
    {   
        CSGPrim* p = convert_(comp, primIdx); 
        bb.include_aabb( p->AABB() );
    }   
    so->center_extent = bb.center_extent() ;  

    //addInstances(repeatIdx); 

    LOG(info) << " solid.bb " <<  bb ;
    LOG(info) << " solid.ce " << so->center_extent ;
    LOG(info) << " solid.desc " << so->desc() ;

    return so ; 
}


CSGPrim* Converter::convert_(const GParts* comp, unsigned primIdx )
{
    unsigned numPrim = comp->getNumPrim();
    assert( primIdx < numPrim ); 
    unsigned numParts = comp->getNumParts(primIdx) ;
    unsigned partOffset = comp->getPartOffset(primIdx) ;
    unsigned sbtIdx = primIdx ; // should be absolute ?

    LOG(info)
        << " sbtIdx " << sbtIdx
        << " primIdx " << primIdx 
        << " numPrim " << numPrim
        << " numParts " << numParts 
        << " partOffset " << partOffset
        ;   

    CSGPrim* pr = foundry->addPrim(numParts); 
    assert(pr) ; 
    pr->setSbtIndexOffset(sbtIdx) ;

    AABB bb = {} ;

    for(unsigned partIdxRel=0 ; partIdxRel < numParts ; partIdxRel++ )
    {
        CSGNode* n = convert_(comp, primIdx, partIdxRel); 
        bb.include_aabb( n->AABB() );
    }

    pr->setAABB( bb.data() ); 

    return pr ; 
}



CSGNode* Converter::convert_(const GParts* comp, unsigned primIdx, unsigned partIdxRel )
{
    unsigned partOffset = comp->getPartOffset(primIdx) ;
    unsigned partIdx = partOffset + partIdxRel ;
    unsigned idx = comp->getIndex(partIdx);
    assert( idx == partIdx ); 

    //const char*  tn = comp->getTypeName(partIdx);
    std::string tag = comp->getTag(partIdx); 
    unsigned tc = comp->getTypeCode(partIdx);

    const Tran<float>* tv = nullptr ; 
    if( splay != 0.f )
    {
        tv = Tran<float>::make_translate(0.f, float(primIdx)*splay, float(partIdxRel)*splay ); 
    }
    else
    {
        unsigned gt = comp->getGTransform(partIdx);
        if( gt > 0 )
        {
            glm::mat4 t = comp->getTran(gt-1,0) ;
            glm::mat4 v = comp->getTran(gt-1,1); 
            tv = new Tran<float>(t, v); 
        }
    }

    unsigned tranIdx = tv ?  1 + foundry->addTran(*tv) : 0 ; 
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

    if(tranIdx > 0 )
    {    
        const qat4* q = foundry->getTran(tranIdx-1u) ;
        q->transform_aabb_inplace( n->AABB() );
    }

    return n ; 

}


