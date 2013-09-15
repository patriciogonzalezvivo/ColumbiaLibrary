//
//  File.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/2/13.
//
//

#include "Category.h"

Category::Category(){
    color.set(1.0);
    textColor.set(1.0);
    borderColor.set(1);
    
    font = NULL;
}

bool Category::loadSettings(string _fileConfig){
    DraggableRectangle::loadSettings( _fileConfig );
}

bool Category::loadSettings(int _num, string _fileConfig){
    
    float networkRadio = 250;
    
    ofVec3f randomPosition = ofVec3f(ofRandom(-networkRadio,networkRadio),
                                     ofRandom(-networkRadio,networkRadio),
                                     ofRandom(-networkRadio,networkRadio) );
    
    bool fileFound = false;
    ofxXmlSettings XML;
    if (XML.loadFile(_fileConfig)){
        if (XML.pushTag("rect", _num) ){
            fileFound = true;
            x = XML.getValue("x", 0);
            y = XML.getValue("y", 0);
            width = XML.getValue("width", ofGetScreenWidth());
            height = XML.getValue("height", ofGetScreenHeight());
            name = XML.getValue("name", " ");
            totalPct = XML.getValue("holding", 0.1);
            
            float offSet = (totalPct*1000);
            
            XML.pushTag("nodes");
            int totalNodes = XML.getNumTags("node");
            for(int i = 0; i < totalNodes; i++){
                XML.pushTag("node",i);
                string nodeName = XML.getValue("name", "name");
                float scale = XML.getValue("holding", 0.0);
                
                nodesText.push_back(nodeName);
                nodes.push_back( new ofVec4f(randomPosition.x+ofRandom(-offSet,offSet)+ofRandom(-20,20),
                                             randomPosition.y+ofRandom(-offSet,offSet)+ofRandom(-20,20),
                                             randomPosition.z+ofRandom(-offSet,offSet)+ofRandom(-20,20),
                                             scale*networkRadio ) );
                XML.popTag();
            }
            
            XML.popTag();
        }
    }
    
    this->set(x, y, width, height);
    
    return fileFound;
}

void Category::getPoints(vector<ofPoint> &_pnts){
    for (int i = 0; i < width; i++) {
        for(int j = 0; j < height; j+=3){
            ofPoint pos = ofPoint(x+i,y+j);
            _pnts.push_back( pos );
        }
    }
}

void Category::draw(bool _bEdit){
    
    if ( _bEdit ){
        ofPushMatrix();
        DraggableRectangle::draw();
        ofPopMatrix();
    } else {
        ofPushStyle();
        
        if (color.a != 0.0) {
            ofFill();
            ofSetColor(color);
            ofRect(*this);
        }
        
        if (borderColor.a != 0.0){
            ofNoFill();
            ofSetColor(borderColor);
            ofRect(*this);
        }
        
        if (textColor.a != 0.0) {
            if ( font != NULL){
                ofPushMatrix();
                ofTranslate(this->getBottomLeft());
                ofSetColor(textColor);
                font->drawString(name, -15, 20);
                ofPopMatrix();
            } else {
                ofPushMatrix();
                ofTranslate(this->getTopLeft());
                ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
                ofSetColor(textColor);
                ofDrawBitmapString(name,15,15);
                ofPopMatrix();
            }
        }
        ofPopStyle();
    }
}