//
//  Subject.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 10/19/13.
//
//
#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "Particle.h"

class Subject : public Particle {
public:

    Subject();
    
    void draw();
    
    string  name;
    ofColor color;
    
    float   *freqscale;
    int     nId, freq, locOrder;
    bool    bSelected;

    ofxUIFont    *font;
};
