//
//  Connection.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 10/19/13.
//
//

#pragma once

#include "ofMain.h"
#include "Subject.h"

#define MIN_LINE 8.0

class Connection : public ofPoint {
public:
    
    Connection();
    
    void makeGlyph(vector<Subject> &_subjects, float &_freqscale);
    void draw();
    
    vector<ofPoint> lines;
    vector<int>     subIx;
    
    ofColor color;
    int     freq;
    
private:
    float     *freqscale;
    vector<Subject> *subjects;
};