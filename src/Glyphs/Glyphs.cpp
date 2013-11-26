//
//  Glyphs.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#include "Glyphs.h"

bool sortSubject(  Subject *a, Subject *b ) {
	return a->locOrder < b->locOrder;
}

bool sortConnection(  Connection *a, Connection *b ) {
	return a->angle < b->angle;
}

void Glyphs::selfSetup(){
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
  
    radius_levels[0] = 0;
    radius_levels[1] = 0;
    radius_levels[2] = 55;
    radius_levels[3] = 40;
    radius_levels[4] = 25;
    radius_levels[5] = 15;
    radius_levels[6] = 5;
    radius_levels[7] = 0;
    radius_levels[8] = 0;
    radius_levels[9] = 0;
    
    colors_levels[0].set(0);
    colors_levels[1].set(0);
    colors_levels[2].set(200,100,0);
    colors_levels[3].set(200,0,0);
    colors_levels[4].set(100,0,100);
    colors_levels[5].set(50,0,200);
    colors_levels[6].set(0,0,250);
    colors_levels[7].set(0);
    colors_levels[8].set(0);
    colors_levels[9].set(0);
    
    freqscale = 10.0;
    
    circle.loadImage(getDataPath()+"images/circle.png");
    
    for(int i = 0; i < 5; i++){
        text[i].setFontsDir( getDataPath()+"fonts/" );
        string textName;
        if (i == 0){
            textName = "zero";
        } else if (i == 1){
            textName = "one";
        } else if (i == 2){
            textName = "two";
        } else if (i == 3){
            textName = "three";
        } else if (i == 4){
            textName = "four";
        }
        
        text[i].loadSettings(getDataPath()+"text.xml",textName);
    }
}

void Glyphs::selfSetupGuis(){
    backgroundSet(new UISuperBackground());
    
    for(int i = 0; i < 5; i++){
        guiAdd(text[i]);
    }
    
    lightAdd("TEXT_LIGHT", OF_LIGHT_SPOT);
    guiAdd(grid);
}

void Glyphs::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Glyphs::selfSetupSystemGui(){
    sysGui->addLabel("General");
    sysGui->addSlider("Scale", 5, 50, &freqscale);
    sysGui->addSlider("speed", 0.1, 1.0, &speed);
    sysGui->addSlider("toOriging", 0.0, 0.25, &forceToOrigin);
    sysGui->addSlider("toCenter", 0.0, 0.25, &forceToCenter);
    sysGui->addSlider("toClosest", 0.0, 0.25, &forceToClosest);
    sysGui->addSlider("repulsion", 0.0, 10.0, &forceRepel);
}

void Glyphs::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
}

void Glyphs::selfSetupRenderGui(){

}

void Glyphs::guiRenderEvent(ofxUIEventArgs &e){

}

void Glyphs::selfBegin(){
    //  RESET EVERYTHING
    //
    center.set(0,0,0);
    selfEnd();
    
    // =======================  SUBJECTS
    //
    {
        //  Read TSV file
        //
        ofBuffer buffer = ofBufferFromFile(getDataPath()+"subjects.csv");
        int counter = 0;
        while(!buffer.isLastLine()) {
            string temp = buffer.getNextLine();
            
            if(temp.length() != 0) {
                if (counter > 0){
                    vector<string> value = ofSplitString(temp, ",");
                    
                    for(int i = 0; i < value.size(); i++){
                        cout << value[i] << " ";
                    }
                    cout << endl;
                    
                    Subject *s = new Subject();
                    s->nId = ofToInt(value[0]);
                    s->name = value[1];
                    s->freq = ofToInt(value[2]);
                    s->size = log(s->freq+2.0)* (freqscale);
                    s->locOrder = ofToInt(value[3]);
                    s->font = gui->getFontLarge();
                    subjects.push_back(s);
                }
                counter++;
            }
        }
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
                    Connection *c = new Connection;
                    vector<string> subCon = ofSplitString(value[0], ",");
                    for (int i = 0; i < subCon.size();i++) {
                        int pointsTo = ofToInt( subCon[i] ) - 1;
                        c->subIx.push_back(pointsTo);
                    }
                    counters[ subCon.size() ]++;
                    c->freq = ofToInt(value[1]);
                    c->nId = counter;
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
    }
    
    //  Calculate position
    //
    for(int i = 0; i < connections.size(); i++) {
        int level       = connections[i]->subIx.size();
        float angle     = angles[level];
        float radius    = radius_levels[level]*freqscale;
        
        //  Set position
        //
        ofPoint pos;
        pos.x = center.x+radius*cos(angle);
        pos.y = center.y+radius*sin(-angle);
        
        connections[i]->set(pos);
        connections[i]->origin.set(pos);
        
        connections[i]->color.set(210,50);
        //connections[i]->color.set(colors_levels[level]);
        connections[i]->size = log(connections[i]->freq+2.0)*freqscale;
        
        connections[i]->makeGlyph(subjects);
        
        angles[level]+=jumps[level];
        
        //  Add Springs
        //
        connections[i]->bFix = false;
        Spring toCenter;
        toCenter.A = connections[i];
        toCenter.B = &center;
        toCenter.length = radius;
        toCenter.k = 2.0;
        springsToCenter.push_back(toCenter);
        
        for (int j = 0; j < connections[i]->subIx.size();j++) {
            Spring toSubject;
            toSubject.A = connections[i];
            toSubject.B = subjects[connections[i]->subIx[j]];
            toSubject.k = 0.1/((float)connections[i]->subIx.size());
            springsToSubjects.push_back(toSubject);
        }
    }
    
    //  Calculate Subject positions
    //
    sort(subjects.begin(), subjects.end(), sortSubject);
    float jump = TWO_PI/(float)subjects.size();
    float angle = 0;
    float radius = 80*freqscale;
    for(int i = 0; i < subjects.size(); i++){
        subjects[i]->x = center.x+radius*cos(angle);
        subjects[i]->y = center.y+radius*sin(-angle);
        
        if ( i < 6 ) {
            subjects[i]->color = ofColor(144, 230, 56, 130);
        } else if ( i < 11 ) {
            subjects[i]->color = ofColor(47, 75, 255, 108);
        } else if ( i < 15 ) {
            subjects[i]->color = ofColor(0, 184, 244, 100);
        } else {
            subjects[i]->color = ofColor(255, 149, 57, 120);
        }
        
        angle+=jump;
    }
}

void Glyphs::selfUpdate(){
    
    for(int i = 0; i < springsToCenter.size(); i++){
        springsToCenter[i].update(forceToCenter);
    }
    
    for(int i = 0; i < springsToSubjects.size(); i++){
        springsToSubjects[i].update(forceToClosest);
    }
    
    ofPoint mouse = ofPoint(ofGetMouseX(),ofGetMouseY());
    
    for(int i = 0; i < connections.size();i++){
        if(forceToOrigin>0.0){
            connections[i]->addSlowdonForceTo(connections[i]->origin, forceToOrigin, false);
        }
        
        if(forceRepel>0.0){
            for(int j = i+1; j<connections.size(); j++){
                connections[i]->addRepulsionForce(connections[j],forceRepel);
            }
        }
        connections[i]->update(speed);
    }
}

void Glyphs::selfDraw(){
    glDepthMask(false);
    ofPushMatrix();
    ofPushStyle();
    materials["MATERIAL 1"]->begin();
    
    ofPushMatrix();
    ofTranslate(0,0,-20);
    ofRotateX(90);
    grid.draw();
    ofPopMatrix();
    
    
    
    //  Draw Connections
    //
    for(int i = 0; i < connections.size(); i++){
        connections[i]->draw(circle.getTextureReference());
        connections[i]->drawLines(!(connections[i]->bHover||connections[i]->bSelected));
    }
    
    //  Draw Subjects
    //
    for(int i = 0; i < subjects.size(); i++){
        subjects[i]->draw();
        subjects[i]->bSelected = false;
    }
    
    for(int i = 0; i<5;i++){
        ofPushMatrix();
        string cameraName = "0"+ofToString(i);
        ofMultMatrix( camera.getGlobalTransformMatrix(cameraName));
        ofTranslate(-text[i].getCenter().x,-text[i].getCenter().y, -1000);
        text[i].draw();
        ofPopMatrix();
    }
    
    materials["MATERIAL 1"]->end();
    ofPopStyle();
    ofPopMatrix();
    glDepthMask(true);
}

void Glyphs::selfDrawOverlay(){
    
}

void Glyphs::selfPostDraw(){
    UI2DProject::selfPostDraw();
}

void Glyphs::selfEnd(){
    springsToCenter.clear();
    springsToSubjects.clear();
    for(int i=subjects.size()-1;i>=0;i--){
        delete subjects[i];
        subjects.erase(subjects.begin()+i);
    }
    for(int i=connections.size()-1;i>=0;i--){
        delete connections[i];
        connections.erase(connections.begin()+i);
    }
}

void Glyphs::selfExit(){
	
}

void Glyphs::selfKeyPressed(ofKeyEventArgs & args){
	if(args.key == 'p'){
        for(int i = 0; i < connections.size();i++){
            connections[i]->angle = atan2(connections[i]->y,connections[i]->x);
        }
        sort(connections.begin(), connections.end(), sortConnection);
        
        ofBuffer buffer;
        for(int i = 0; i < connections.size();i++){
            buffer.append(ofToString(connections[i]->nId)+","+ofToString(connections[i]->angle)+"\n");
        }
        
        ofBufferToFile(getDataPath()+"sorted.csv", buffer);
        
    }
}

void Glyphs::selfKeyReleased(ofKeyEventArgs & args){
	
}

void Glyphs::selfMouseDragged(ofMouseEventArgs& data){
   
}

void Glyphs::selfMouseMoved(ofMouseEventArgs& data){
    ofPoint mouse(data.x,data.y);
    
	for(int i = 0; i < connections.size();i++){
        ofPoint screenPos = camera.getCameraPtn()->worldToScreen( *connections[i] );
        if (mouse.distance(screenPos)<10){
            connections[i]->bHover = true;
        } else {
            connections[i]->bHover = false;
        }
    }
}

void Glyphs::selfMouseDoublePressed(ofMouseEventArgs &data){
//    if(!bEdit){
//        camera.disableMouseInput();
//    }
//    
//    ofPoint mouse(data.x,data.y);
//	for(int i = 0; i < connections.size();i++){
//        ofPoint screenPos = camera.getCameraPtn()->worldToScreen( *connections[i] );
//        if (mouse.distance(screenPos)<10){
//            connections[i]->bSelected = true;
//        } else {
//            connections[i]->bSelected = false;
//        }
//    }
}

void Glyphs::selfMousePressed(ofMouseEventArgs& data){
	ofPoint mouse(data.x,data.y);
	for(int i = 0; i < connections.size();i++){
        ofPoint screenPos = camera.getCameraPtn()->worldToScreen( *connections[i] );
        if (mouse.distance(screenPos)<10){
            connections[i]->bSelected = true;
        } else {
            connections[i]->bSelected = false;
        }
    }
}

void Glyphs::selfMouseReleased(ofMouseEventArgs& data){

}