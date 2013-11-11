//
//  Connection.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 10/19/13.
//
//

#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "Subject.h"

#define MIN_LINE 8.0

class Connection : public Particle {
public:
    
    Connection();
    
    void    getPoints(vector<ofVec3f> &_pnts, float _size);
    void    makeGlyph(vector<Subject*> &_subjects);
    
    void    draw(ofTexture &_tex);
    void    drawLines(bool _clamp);
    
    vector<int>     subIx;
    
    ofColor color;
    ofPoint origin;
    int     freq;
    
    bool    bHover;
    bool    bSelected;
    
private:
    vector<Subject*> subCon;
};