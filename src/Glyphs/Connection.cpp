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
    bHover = false;
    bSelected = false;
}

void Connection::getPoints(vector<ofVec3f> &_pnts, float _size){
    float radius = size;
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

void Connection::makeGlyph(vector<Subject*> &_subjects){
    for(int i = 0; i < subIx.size(); i++){
        Subject *s = _subjects[subIx[i]];
        subCon.push_back(s);
    }
}

void Connection::draw(ofTexture &_tex){
    ofPushStyle();
    ofPushMatrix();
    
    if(bHover||bSelected){
        ofTranslate(0, 0, 2);
    }
    
    ofSetColor(color);
    ofFill();
    ofTranslate(*this);
    _tex.draw(-size*0.5, -size*0.5, size,size);
    //    ofEllipse(*this, size, size);
    
    ofPopMatrix();
    ofPopStyle();
}

void Connection::drawLines(bool _clamp){
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(*this);
    
    if(bHover||bSelected){
        ofTranslate(0, 0, 2);
    }
    
    ofSetLineWidth(3);
    for(int i = 0; i < subCon.size(); i++){
        ofPoint diff = (*subCon[i])-ofPoint(*this);
        
        if(_clamp){
            diff.normalize();
            if ( size*0.5 < MIN_LINE ) {
                diff *= MIN_LINE;
            } else {
                diff *= size*0.5;
            }
        } else {
            subCon[i]->bSelected = bHover||bSelected;
        }
        
        ofTranslate(0, 0, 0.1);
        if(bHover||bSelected){
            ofSetColor(subCon[i]->color);
        } else {
            ofSetColor(subCon[i]->color);
        }
        ofLine(ofPoint(0,0),diff);
    }
    ofPopMatrix();
    ofPopStyle();

}

