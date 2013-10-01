//
//  InBoxes.cpp
//
//  Created by Patricio Gonzalez Vivo on 8/10/13.
//
//

#include "InBoxes.h"

void InBoxes::selfSetup(){
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    //  Total dimensions of the Cubelibrary
    //
    totalWith = 1000;
    totalHeight = 1000;
    
    font.loadFont("GUI/NewMedia Fett.ttf", 14);
    
    //  Place the books rectangles that represent the 4 sections of all the data
    //
    archiveBooks[0].x = 0-totalWith*0.5;
    archiveBooks[0].y = totalHeight*0.8934-totalHeight*0.5;
    archiveBooks[0].width = totalWith;
    archiveBooks[0].height = totalHeight*0.1066;
    archiveBooks[0].borderColor.a = 0.2;
//    archiveBooks[0].font = &font;
    
    archiveBooks[1].x = 0-totalWith*0.5;
    archiveBooks[1].y = 0-totalHeight*0.5;
    archiveBooks[1].width = totalWith;
    archiveBooks[1].height = totalHeight*0.8934;
    archiveBooks[1].borderColor.a = 0.2;
//    archiveBooks[1].font = &font;
    
    archiveBooks[2].x = archiveBooks[1].getLeft()+totalWith*0.548;
    archiveBooks[2].y = archiveBooks[1].getTop()+totalHeight*0.6434;
    archiveBooks[2].width = totalWith*0.452;
    archiveBooks[2].height = totalHeight*0.25;
    archiveBooks[2].borderColor.a = 0.2;
//    archiveBooks[2].font = &font;
    
    //  Public books
    //
    archiveBooks[3].x = archiveBooks[1].getLeft()+totalWith*0.849;
    archiveBooks[3].y = archiveBooks[1].getTop()+totalHeight*0.7734;
    archiveBooks[3].width = totalWith*0.151;
    archiveBooks[3].height = totalHeight*0.12;
    archiveBooks[3].borderColor.a = 0.2;
//    archiveBooks[3].font = &font;
    archiveBooks[3].color = 0.0;
    
    //  Set the categories
    //
    categories.clear();
    
    Category cat;
    for (int i = 0; i < 4; i++) {
        cat.loadSettings(getDataPath()+ofToString(i)+".xml");
        cat.font = &font;
        cat.color.a = 0.0;
        cat.borderColor.a = 0.0;
        categories.push_back(cat);
    }
    
    // Place the Books
    //
    float width = 435;
    float height = 345;
    float scaleWidth = archiveBooks[3].width/width;
    float scaleHeight = archiveBooks[3].height/height;
    
    publicBooks.clear();
    for (int x = 0; x < width; x+=2) {
        for(int y = 0; y < height; y+=2){
            BookParticle p;
            p.init(ofPoint(archiveBooks[3].x + (float)(x)*scaleWidth ,
                           archiveBooks[3].y + (float)(y)*scaleHeight ),
                   ofPoint(0,0));
            p.color = 0.7;
            publicBooks.push_back(p);
        }
    }
    
    
    for (int i = 0; i < categories.size(); i++) {
        vector<ofPoint> positions;
        categories[i].getPoints(positions);
        
        for(int j = 0; j < positions.size(); j++ ){
            
            bool foundDestiny = true;
            while ( foundDestiny ) {
                int index = ofRandom( publicBooks.size() );
                
                if ( publicBooks[index].destiny == ofPoint(0,0,0) ){
                    publicBooks[index].destiny = positions[j];
                    foundDestiny = false;
                }
            }
        }
    }
    
    debugTranslation = 0.25;
    debugScale = 1.25;
    
    width = ofGetScreenWidth()*2.0;
    height = ofGetScreenHeight()*2.0;
    
}

void InBoxes::selfSetupGui(){
    
}

void InBoxes::selfGuiEvent(ofxUIEventArgs &e){

}

void InBoxes::selfSetupSystemGui(){
	sysGui->addLabel("Books");
    sysGui->addSlider("Group_A_Brigness", 0.0, 1.0, &archiveBooks[0].color.a);
    sysGui->addSlider("Group_B_Brigness", 0.0, 1.0, &archiveBooks[1].color.a);
    sysGui->addSlider("Group_C_Brigness", 0.0, 1.0, &archiveBooks[2].color.a);
    sysGui->addSlider("Group_D_Brigness", 0.0, 1.0, &archiveBooks[3].color.a);
    sysGui->addSlider("Particles_Atraction", 0.0, 0.2, &bookAtraction);
    sysGui->addSlider("Particles_Turbulence", 0.0, 0.2, &bookTurbulence);
}

void InBoxes::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name == "DEBUG"){
        if (bDebug){
            cam.disableMouseInput();
            timeline->stop();
            isPlaying = false;
        } else {
            
            //  Clean final destinations
            //
            for (int i = 0; i < publicBooks.size(); i++) {
                publicBooks[i].destiny = ofPoint(0,0,0);
            }
            
            for(int i = 0; i < categories.size(); i++){
                
                //  Save settings
                //
                categories[i].saveSettings(getDataPath()+ofToString(i)+".xml");
                
                //  Re-assign final destinations
                //
                vector<ofPoint> positions;
                categories[i].getPoints(positions);
                for(int j = 0; j < positions.size(); j++ ){
                    
                    bool foundDestiny = true;
                    while ( foundDestiny ) {
                        int index = ofRandom( publicBooks.size() );
                        
                        if ( publicBooks[index].destiny == ofPoint(0,0,0) ){
                            publicBooks[index].destiny = positions[j];
                            foundDestiny = false;
                        }
                    }
                }
            }
            cam.enableMouseInput();
        }
    }
}

void InBoxes::selfSetupRenderGui(){
}

void InBoxes::guiRenderEvent(ofxUIEventArgs &e){

}

void InBoxes::selfPresetLoaded(string presetPath){
	
}

void InBoxes::selfBegin(){
	
}

void InBoxes::selfSceneTransformation(){
	
}

void InBoxes::selfUpdate(){
    
    for (int i = 0; i < publicBooks.size(); i++) {
        
        //  Every Book is atracted to his origin place
        //  So they tend to come back slowly
        //
        publicBooks[i].addAttractionForce(publicBooks[i].origin, 1000, 0.1);
        
        //  Adding a little of noise will make it to look interesting
        //
        publicBooks[i].addNoise(ofRandom(PI/2.0,-PI/2.0), ofRandom(bookTurbulence));
        publicBooks[i].addAttractionForce(publicBooks[i].destiny, 500, bookAtraction);

        //  After all the forces have been added update the position.
        //
        publicBooks[i].update(0.93);
        
    }
}

void InBoxes::selfDraw(){
    
    if (!bDebug){
        glDisable(GL_DEPTH_TEST);
        ofEnableAlphaBlending();
        
        ofPushMatrix();
        ofPushStyle();
        materials["MATERIAL 1"]->begin();
        
        drawBrakets(ofRectangle(-totalWith*0.5,-totalHeight*0.5,totalWith,totalHeight), 15,10);
        
        //  Draw black book boxes
        //
        for (int i = 0; i < 4; i++) {
            archiveBooks[i].draw(bDebug);
        }
        
        for(int i = 0; i < categories.size(); i++){
            drawBrakets(categories[i], 15, 10);
            categories[i].draw(bDebug);
        }
        
        //  Draw the Books
        //
        glBegin(GL_POINTS);
        ofPushStyle();
        glPointSize(50);
        
        for (int i = 0; i < publicBooks.size(); i++) {
            glVertex3f(publicBooks[i].x,publicBooks[i].y,publicBooks[i].z);
        }
        ofPopStyle();
        glEnd();
        
        materials["MATERIAL 1"]->end();
        ofPopStyle();
        ofPopMatrix();
    }
}

void InBoxes::selfDrawDebug(){
    
}

void InBoxes::selfDrawBackground(){
    
    if (bDebug){
        ofEnableAlphaBlending();
        
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(ofGetWidth()*debugTranslation, ofGetHeight()*debugTranslation);
        ofScale(1.0/debugScale,1.0/debugScale,1.0/debugScale);
        
        //  Draw black book boxes
        //
        for (int i = 0; i < 4; i++) {
            archiveBooks[i].draw(bDebug);
        }
        
        for(int i = 0; i < categories.size(); i++){
            categories[i].draw(bDebug);
        }
        
        //  Draw the Books
        //
        for (int i = 0; i < publicBooks.size(); i++) {
            publicBooks[i].draw();
        }
        
        ofPopStyle();
        ofPopMatrix();
        
        if ( timeline->getIsShowing() ){
            ofPushStyle();
            ofFill();
            ofSetColor(0,100);
            ofRect(4, ofGetHeight() - timeline->getHeight() - 4, ofGetWidth() - 8, timeline->getHeight());
            ofPopStyle();
        }
    }
    
}

void InBoxes::selfPostDraw(){
    if (bDebug){
        UITimeProject::selfPostDraw();
    } else {
        UIProject::selfPostDraw();
    }
}

void InBoxes::drawBrakets(ofRectangle _rect, float size, float margin){
    
    ofPushStyle();
    ofSetLineWidth(2);
    
    ofPushMatrix();
    ofTranslate(_rect.getTopLeft() + ofPoint(-margin,-margin));
    ofLine(0, 0, size, 0);
    ofLine(0, 0, 0, size);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(_rect.getTopRight() + ofPoint(margin,-margin));
    ofLine(0, 0, -size, 0);
    ofLine(0, 0, 0, size);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(_rect.getBottomLeft() + ofPoint(-margin,margin));
    ofLine(0, 0, size, 0);
    ofLine(0, 0, 0, -size);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(_rect.getBottomRight() + ofPoint(margin,margin));
    ofLine(0, 0, -size, 0);
    ofLine(0, 0, 0, -size);
    ofPopMatrix();
    
    ofPopStyle();
}

void InBoxes::selfEnd(){
	
}

void InBoxes::selfExit(){
	
}

void InBoxes::selfKeyPressed(ofKeyEventArgs & args){
	
}
void InBoxes::selfKeyReleased(ofKeyEventArgs & args){
	
}

void InBoxes::selfMouseDragged(ofMouseEventArgs& data){
    ofPoint mouse = ofPoint(data.x,data.y);
    
	if (bDebug){
        
        mouse -= ofPoint(ofGetWidth()*debugTranslation,ofGetHeight()*debugTranslation);
        mouse *= debugScale;
        
        for(int i = 0; i < categories.size(); i++){
            categories[i].mouseDragged( mouse );
        }
    }
}

void InBoxes::selfMouseMoved(ofMouseEventArgs& data){
	
}

void InBoxes::selfMousePressed(ofMouseEventArgs& data){
	
}

void InBoxes::selfMouseReleased(ofMouseEventArgs& data){
	if (bDebug){
        cam.disableMouseInput();
    }
}