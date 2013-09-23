//
//  DragCircle.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#include "DraggableCircle.h"

bool DraggableCircle::loadSettings(string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        fileFound = true;
        x = XML.getValue("circle:x", 0);
        y = XML.getValue("circle:y", 0);
        z = XML.getValue("circle:z", 0);
        pct = XML.getValue("circle:pct", 10.0);
        name = XML.getValue("circle:name", " ");
        parent = XML.getValue("circle:parent", -1);
    }
    
    return fileFound;
}

bool DraggableCircle::loadSettings(int _num, string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        if (XML.pushTag("circle", _num) ){
            fileFound = true;
            x = XML.getValue("x", 0);
            y = XML.getValue("y", 0);
            z = XML.getValue("z", 0);
            pct = XML.getValue("pct", 10.0);
            name = XML.getValue("name", " ");
            parent = XML.getValue("parent", -1);
        }
    }
    
    return fileFound;
}

bool DraggableCircle::loadSettings(string _name, string _fileConfig){
    bool fileFound = false;
    name = _name;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        int total = XML.getNumTags("circle");
        for (int i = 0; i < total; i++){
            
            if ( XML.pushTag("circle", i) ){
                if ( name == XML.getValue("name", " ") ){
                    fileFound = true;
                    x = XML.getValue("x", 0);
                    y = XML.getValue("y", 0);
                    z = XML.getValue("z", 0);
                    pct = XML.getValue("pct", 10.0);
                    parent = XML.getValue("parent", -1);
                    break;
                }
                XML.popTag();
            }
        }
    }
    
    return fileFound;
}

bool DraggableCircle::saveSettings(string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        int total = XML.getNumTags("circle");
        
        if (total == 1){
            if ( XML.pushTag("circle") ){
                XML.setValue("x", x);
                XML.setValue("y", y);
                XML.popTag();
                fileFound = XML.saveFile();
            }
        } else {
            saveSettings(name,_fileConfig);
        }
    }
    
    return fileFound;
}

bool DraggableCircle::saveSettings(int _num, string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        if ( XML.pushTag("circle",_num) ){
            XML.setValue("x", x);
            XML.setValue("y", y);
            XML.popTag();
            fileFound = XML.saveFile();
        }
    }
    
    return fileFound;
}

bool DraggableCircle::saveSettings(string _name, string _fileConfig){
    bool fileFound = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        int total = XML.getNumTags("circle");
        
        for (int i = 0; i < total; i++){
            if ( XML.pushTag("circle", i) ){
                if ( name == XML.getValue("name", " ") ){
                    XML.setValue("x", x);
                    XML.setValue("y", y);
                    XML.popTag();
                    fileFound = XML.saveFile();
                    return fileFound;
                }
                XML.popTag();
            }
        }
    }
    
    return fileFound;
}

void DraggableCircle::getPoints(vector<ofVec3f> &_pnts, float _size){
    float radius = pct*scale;
    
    int totalRings = radius/_size;
    for (int i = 0; i < totalRings; i++) {
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

bool DraggableCircle::mouseDragged(ofPoint _mouse){
    if ( ofPoint(x,y).distance( _mouse) < pct*scale ){
        x += _mouse.x - x;
        y += _mouse.y - y;
        this->set(x,y,z);
        return true;
    } else {
        return false;
    }
}

void DraggableCircle::draw(){
    float radius = pct*scale;
    ofPushMatrix();
    ofTranslate(*this);
    ofSetColor(255);
    ofCircle(0, 0, radius);
    ofDrawBitmapString(name, ofPoint(radius,radius));
    ofPopMatrix();
}