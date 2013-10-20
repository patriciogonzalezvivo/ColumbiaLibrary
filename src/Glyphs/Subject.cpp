//
//  Subject.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 10/19/13.
//
//

#include "Subject.h"

Subject::Subject(){
    color = ofColor(0, 204, 0, 150);
    bSelected = false;
}

void Subject::draw(){
    ofPushStyle();
    
    float circle_radius = log(freq+2.0)* (*freqscale);
    
    if(bSelected){
        ofSetColor(color);
    } else {
        ofSetColor(color,150);
    }
    ofEllipse(*this, circle_radius, circle_radius);
    
    ofDrawBitmapString(ofToString(nId) + " " + name, *this);
    
    ofPopStyle();
}