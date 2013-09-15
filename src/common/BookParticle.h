#pragma once

#include "SuperParticle.h"

class BookParticle : public SuperParticle {
public:
    void    init(ofPoint _pos, ofPoint _vel);
    
    void    applyAttractionToNodes(float _pct);
    
    ofPoint origin;
    ofPoint destiny;
    
    ofPoint networkPos;
    vector<ofVec4f*>    nodes;
    
    string  text;
    
    int     category;
};