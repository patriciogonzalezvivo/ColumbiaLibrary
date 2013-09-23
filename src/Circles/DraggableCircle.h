//
//  DragCircle.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

class DraggableCircle : ofPoint {
public:
    
    bool    loadSettings(string _fileConfig);
    bool    loadSettings(int _num, string _fileConfig);
    bool    loadSettings(string _name, string _fileConfig);
    
    bool    saveSettings(string _fileConfig);
    bool    saveSettings(int _num , string _fileConfig);
    bool    saveSettings(string _name, string _fileConfig);
    
    void    getPoints(vector<ofVec3f> &_pnts, float _size);
    
    bool    mouseDragged(ofPoint _mouse);
    void    draw();
    
    float   scale;
    
    int     parent;
protected:
    string  name;
    float   pct;
    int     nId;
};