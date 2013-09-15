//
//  Category.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/1/13.
//
//

#pragma once
#include "DraggableRectangle.h"

class Category : public DraggableRectangle {
public:
    Category();
    
    bool loadSettings(string _fileConfig);
    bool loadSettings(int _num, string _fileConfig);
    
    void getPoints(vector<ofPoint> &_pnts);
    
    void draw(bool _bEdit);
    
    vector<ofVec4f*>    nodes;
    vector<string>      nodesText;
    
    ofFloatColor    color;
    ofFloatColor    textColor;
    ofFloatColor    borderColor;
    
    ofTrueTypeFont  *font;
    
    float           totalPct;
    int             canHold;
};