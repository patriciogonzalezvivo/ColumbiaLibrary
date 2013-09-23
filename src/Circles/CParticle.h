//
//  CParticle.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#pragma once

#include "SuperParticle.h"

class CParticle : public SuperParticle {
public:
    CParticle();
    
    ofPoint networkPos;
    ofPoint destinyPos;
};