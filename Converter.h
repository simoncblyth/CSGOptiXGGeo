#pragma once

struct CSGFoundry ;
struct CSGSolid ; 
struct CSGPrim ; 
struct CSGNode ; 

class GGeo ;
class GParts ; 

struct Converter
{   
    CSGFoundry* foundry ; 
    const GGeo* ggeo ; 
    bool dump ; 
    float splay ; 

    Converter(CSGFoundry* foundry, const GGeo* ggeo, bool dump ) ; 

    void convert(int repeatIdx,  int primIdx, int partIdxRel );
    void convert_();

    CSGSolid* convert_(unsigned repeatIdx );
    CSGPrim*  convert_(const GParts* comp, unsigned primIdx );
    CSGNode*  convert_(const GParts* comp, unsigned primIdx, unsigned partIdxRel );
};


