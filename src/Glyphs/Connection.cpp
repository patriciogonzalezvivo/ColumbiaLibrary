//
//  Connection.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 10/19/13.
//
//

#include "Connection.h"

Connection::Connection(){
    color = ofColor(155, 0, 0, 150);
}

void Connection::getPoints(vector<ofVec3f> &_pnts, float _size){
    radius = (log(freq+2.0)*(*freqscale))*0.5;
    
    int totalRings = radius/_size;
    for (int i = 0; i < freq/10; i++) {
        int totalParticles = (2.0*PI*radius)/_size;
        
        float angle = 0.0;
        float jump = (2.0*PI)/(float)(totalParticles);
        for(int j = 0; j < totalParticles; j++){
            ofVec3f point = ofVec3f(x + radius*cos(angle),
                                    y + radius*sin(angle),
                                    z);
            _pnts.push_back(point);
            angle += jump;
        }
        
        radius -= _size;
    }
    
}

void Connection::makeGlyph(vector<Subject> &_subjects, float &_freqscale){
    subjects = &_subjects;
    freqscale = &_freqscale;
    
    for(int i = 0; i < subIx.size(); i++){
        Subject s = _subjects[subIx[i]];
        
        radius = ( log(freq+2.0)* (*freqscale) )*0.5;
        ofPoint diff = ofPoint(s)-ofPoint(*this);
        diff.normalize();
        
        if ( radius < MIN_LINE ) {
            diff *= MIN_LINE;
        } else {
            diff *= radius;
        }
        lines.push_back(diff);
    }
}

void Connection::draw(){
    ofPushStyle();
    ofPoint mouse = ofPoint(ofGetMouseX(),ofGetMouseY());
    radius = log(freq+2.0)* (*freqscale);
    
    if( distance(mouse)<radius){
        ofSetColor(color);
        for(int i = 0; i < subIx.size(); i++){
            (*subjects)[subIx[i]].bSelected = true;
        }
    } else {
        ofSetColor(color,150);
    }
    ofNoFill();
    ofEllipse(*this, radius, radius);
    
    ofSetColor(100,200);
    ofPushMatrix();
    ofTranslate(*this);
    for(int i = 0; i < lines.size(); i++){
        ofLine(ofPoint(0,0),lines[i]);
    }
    ofPopMatrix();
    
    ofPopStyle();
}