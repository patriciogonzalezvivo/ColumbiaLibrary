//
//  Glyphs.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#include "Glyphs.h"

void Glyphs::selfSetup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
  
    radius_levels[0] = 0;
    radius_levels[1] = 0;
    radius_levels[2] = 275;
    radius_levels[3] = 200;
    radius_levels[4] = 125;
    radius_levels[5] = 75;
    radius_levels[6] = 25;
    radius_levels[7] = 0;
    radius_levels[8] = 0;
    radius_levels[9] = 0;
    
    freqscale = 5.0;
}

void Glyphs::selfSetupGui(){
    
}

void Glyphs::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Glyphs::selfSetupSystemGui(){

}

void Glyphs::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name == "DEBUG"){

    }
}

void Glyphs::selfSetupRenderGui(){

}

void Glyphs::guiRenderEvent(ofxUIEventArgs &e){

}

void Glyphs::selfPresetLoaded(string presetPath){
	
}

void Glyphs::selfBegin(){
	loadStuff();
}

void Glyphs::loadStuff(){
    
    ofPoint screenCenter = ofPoint(ofGetWidth()*0.5, ofGetHeight()*0.5);
    
    // =======================  SUBJECTS
    //
    {
        //  Read TSV file
        //
        ofBuffer buffer = ofBufferFromFile(getDataPath()+"subjects.tsv");
        int counter = 0;
        while(!buffer.isLastLine()) {
            string temp = buffer.getNextLine();
            if(temp.length() != 0) {
                if (counter > 0){
                    vector<string> value = ofSplitString(temp, "\t");
                    Subject s;
                    s.nId = ofToInt(value[0]);
                    s.name = value[1];
                    s.freq = ofToInt(value[4]);
                    s.freqscale = &freqscale;
                    subjects.push_back(s);
                }
                counter++;
            }
        }
    }
    
    //  Calculate positions
    //
    float jump = TWO_PI/(float)subjects.size();
    float angle = 0;
    float radius = 340;
    for(int i = 0; i < subjects.size(); i++){
        subjects[i].x = screenCenter.x+radius*cos(angle);
        subjects[i].y = screenCenter.y+radius*sin(angle);
        angle+=jump;
    }
    
    // ============================================= CONECTIONS
    //
    int counters[MAX_LEVELS];
    for(int i = 0; i < MAX_LEVELS;i++){
        counters[i] = 0;
    }
    
    {
        //  Read TSV file
        //
        ofBuffer buffer = ofBufferFromFile(getDataPath()+"connectionfreqcodes.tsv");
        int counter = 0;
        while(!buffer.isLastLine()) {
            string temp = buffer.getNextLine();
            if(temp.length() != 0) {
                if (counter > 0){
                    vector<string> value = ofSplitString(temp, "\t");
                    cout << "Connection " << counter << " points to: ";
                    
                    Connection c;
                    vector<string> subCon = ofSplitString(value[0], ",");
                    for (int i = 0; i < subCon.size();i++) {
                        int pointsTo = ofToInt( subCon[i] ) - 1;
                        cout << pointsTo << " ";
                        c.subIx.push_back(pointsTo);
                    }
                    counters[ subCon.size() ]++;
                    
                    c.freq = ofToInt(value[1]);
                    
                    cout << " width a freq of " << c.freq << endl;
                    connections.push_back(c);
                }
                counter++;
            }
        }
    }
    
    float jumps[MAX_LEVELS];
    float angles[MAX_LEVELS];
    for(int i = 0; i < MAX_LEVELS;i++){
        jumps[i] = TWO_PI/(float)(counters[i]);
        angles[i] = 0.0;
        
        cout << "Level " << i << " have " << counters[i] << endl;
    }
    
    for(int i = 0; i < connections.size(); i++) {
        int level       = connections[i].subIx.size();
        float angle     = angles[level];
        float radius    = radius_levels[level];
        
        connections[i].x = screenCenter.x+radius*cos(angle);
        connections[i].y = screenCenter.y+radius*sin(angle);
        
        connections[i].makeGlyph(subjects,freqscale);
        
        angles[level]+=jumps[level];
    }
}

void Glyphs::selfSceneTransformation(){
	
}

void Glyphs::selfUpdate(){
    
}

void Glyphs::selfDraw(){
    //  Draw Connections
    //
    for(int i = 0; i < connections.size(); i++){
        connections[i].draw();
    }
    
    //  Draw Subjects
    //
    for(int i = 0; i < subjects.size(); i++){
        subjects[i].draw();
        subjects[i].bSelected = false;
    }
}

void Glyphs::selfDrawDebug(){
    
}

void Glyphs::selfDrawBackground(){
    
}

void Glyphs::selfDrawOverlay(){
    
}

void Glyphs::selfPostDraw(){
    UI2DProject::selfPostDraw();
}

void Glyphs::selfEnd(){

}

void Glyphs::selfExit(){
	
}

void Glyphs::selfKeyPressed(ofKeyEventArgs & args){
	
}

void Glyphs::selfKeyReleased(ofKeyEventArgs & args){
	
}

void Glyphs::selfMouseDragged(ofMouseEventArgs& data){
   
}

void Glyphs::selfMouseMoved(ofMouseEventArgs& data){
	
}

void Glyphs::selfMousePressed(ofMouseEventArgs& data){
	
}

void Glyphs::selfMouseReleased(ofMouseEventArgs& data){

}