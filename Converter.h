#pragma once

struct CSGFoundry ;
class GGeo ;
class GParts ; 

struct Converter
{   
    CSGFoundry* foundry ; 
    const GGeo* ggeo ; 
    bool dump ; 

    Converter(CSGFoundry* foundry, const GGeo* ggeo, bool dump ) ; 

    void convert(int repeatIdx,  int primIdx, int partIdxRel );
    void convert_();
    void convert_(unsigned repeatIdx );
    void convert_(unsigned repeatIdx, unsigned primIdx );
    void convert_(unsigned repeatIdx, unsigned primIdx, unsigned partIdxRel );
    void convertPart(const GParts* comp, unsigned primIdx, unsigned partIdx );
};


