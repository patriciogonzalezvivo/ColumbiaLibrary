//
//  Subject.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 10/19/13.
//
//

#include "Subject.h"

Subject::Subject(){
    color = ofColor(50, 150);
    bSelected = false;
}

void Subject::draw(){
    ofPushStyle();
    
    if(bSelected){
        ofSetColor(color,255);
    } else {
        ofSetColor(color,100);
    }
    
    ofNoFill();
    ofEllipse(*this, size, size);
    
    ofPushMatrix();
    float angle = atan2(y,x);
    float disp = size;
    ofTranslate(*this);
    if(angle > HALF_PI || angle < -HALF_PI){
        angle += PI;
        disp *= -1;
        disp -= font->getStringBoundingBox(ofToString(nId) + " " + name, 0, 0).width;
    }
    ofRotateZ(ofRadToDeg(angle));
    font->drawString(ofToString(nId) + " " + name, disp, 0);
    ofPopMatrix();
    
    ofPopStyle();
}