//
//  Connection.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 10/19/13.
//
//

#include "Connection.h"

Connection::Connection(){
    color = ofColor(255, 0, 0, 150);
}

void Connection::makeGlyph(vector<Subject> &_subjects, float &_freqscale){
    subjects = &_subjects;
    freqscale = &_freqscale;
    
    for(int i = 0; i < subIx.size(); i++){
        Subject s = _subjects[subIx[i]];
        
        float rad = ( log(freq+2.0)* (*freqscale) )*0.5;
        ofPoint diff = ofPoint(s)-ofPoint(*this);
        diff.normalize();
        
        if ( rad < MIN_LINE ) {
            diff *= MIN_LINE;
        } else {
            diff *= rad;
        }
        lines.push_back(diff);
    }
}

void Connection::draw(){
    ofPushStyle();
    ofPoint mouse = ofPoint(ofGetMouseX(),ofGetMouseY());
    float circle_radius = log(freq+2.0)* (*freqscale);
    
    if( distance(mouse)<circle_radius*0.5){
        ofSetColor(color);
        for(int i = 0; i < subIx.size(); i++){
            (*subjects)[subIx[i]].bSelected = true;
        }
    } else {
        ofSetColor(color,150);
    }
    ofEllipse(*this, circle_radius, circle_radius);
    ofPopStyle();
    
    ofPushMatrix();
    ofTranslate(*this);
    for(int i = 0; i < lines.size(); i++){
        ofLine(ofPoint(0,0),lines[i]);
    }
    ofPopMatrix();
}