//
//  Subject.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 10/19/13.
//
//
#pragma once

#include "ofMain.h"

class Subject : public ofPoint {
public:

    Subject();
    
    void draw();
    
    string  name;
    
    ofColor color;
    
    float   *freqscale;
    int     nId, freq;
    bool    bSelected;

};
