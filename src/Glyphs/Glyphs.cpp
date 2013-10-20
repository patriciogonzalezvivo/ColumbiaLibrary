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
    radius_levels[2] = 55;
    radius_levels[3] = 40;
    radius_levels[4] = 25;
    radius_levels[5] = 15;
    radius_levels[6] = 5;
    radius_levels[7] = 0;
    radius_levels[8] = 0;
    radius_levels[9] = 0;
    
    freqscale = 10.0;
    
    //  DATA
    //
    spriteShader.load(getDataPath()+"shaders/sprites");
    ofDisableArbTex();
    ofLoadImage( spriteTexture , getDataPath()+"images/sprite.png");
    ofEnableArbTex();
    colorSampleImage.loadImage("GUI/defaultColorPalette.png");
    
    spritePositions = NULL;
    spriteColors = NULL;
    
    //  Particles
    //
    globalOffset.set(0,0,0);
    float networkRadio = 68*freqscale;
    for(int i = 0; i < TOTAL_PARTICLES; i++){
        CParticle *p = new CParticle();
        p->set( ofRandom(-100,100),ofRandom(-100,100), ofRandom(-100,100) );
        p->color.set(ofRandom(0.5,1.0));
        
        ofQuaternion xRot,yRot;
        xRot.makeRotate(ofRandom(-180,180), 1, 0, 0);
        yRot.makeRotate(ofRandom(-180,180), 0, 1, 0);
        p->networkPos = xRot * yRot * ofPoint(0,0,ofRandom(-networkRadio,networkRadio) );
        particles.push_back(p);
    }
    
    //  Sprites
    //
    spritePositions = new ofVec3f[ TOTAL_PARTICLES ];
    spriteColors    = new ofFloatColor[ TOTAL_PARTICLES ];
    spriteVBO.setVertexData( &spritePositions[0], TOTAL_PARTICLES, GL_DYNAMIC_DRAW );
    spriteVBO.setColorData( &spriteColors[0], TOTAL_PARTICLES, GL_DYNAMIC_DRAW );
}

void Glyphs::selfSetupGui(){
    
}

void Glyphs::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Glyphs::selfSetupSystemGui(){
    sysGui->addLabel("General");
    sysGui->addSlider("Scale", 5, 50, &freqscale);
    sysGui->addSlider("speed", 0.1, 1.0, &speed);
    
    sysGui->addLabel("Flocking");
    sysGui->addSlider("turbulence", 0.0, 0.09, &turbulence);
    sysGui->addSlider("neigbordhood", 1, 500, &neigbordhood);
    sysGui->addSlider("independence", 0, 1.0, &independence);
    
    sysGui->addLabel("Attractors");
    sysGui->addSlider("attraction_destiny", 0.0, 1.0, &destinyAttraction);
    sysGui->addSlider("attraction_network", 0.0, 1.0, &networkAttraction);
}

void Glyphs::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name == "DEBUG"){

    } else {
        subjects.clear();
        connections.clear();

        loadStuff();
        createParticles();
    }
}

void Glyphs::selfSetupRenderGui(){
    rdrGui->addLabel("Sprites");
    rdrGui->addSlider("Sprites_size", 0.0,1.0, &spriteSize);
    //    rdrGui->addSlider("Sprites_size_Min", 1.0,100.0, &spriteSizeMin);
    rdrGui->addSlider("Sprites_size_Max", 1.0,100.0, &spriteSizeMax);
    rdrGui->addSlider("Sprites_focus_Distance", 0.0, 1000.0, &spriteFocusDistance);
    rdrGui->addSlider("Sprites_focus_Aperture", 0.0, 1.0, &spriteFocusAperture);
    particlesColorSampler =  rdrGui->addImageSampler("Sprite_Color_Tint", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
}

void Glyphs::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if( name == "Sprite_Color_Tint"){
        ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
        float alpha = particlesTintColor.a;
        particlesTintColor = sampler->getColor();
        particlesTintColor.a = alpha;
    }
}

void Glyphs::selfPresetLoaded(string presetPath){
	
}

void Glyphs::selfBegin(){
//	loadStuff();
//    createParticles();
}

void Glyphs::loadStuff(){
    
    ofPoint screenCenter = ofPoint(0,0);//ofPoint(ofGetScreenWidth()*0.5, ofGetScreenHeight()*0.5);
    
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
    float radius = 68*freqscale;
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
        float radius    = radius_levels[level]*freqscale;
        
        connections[i].x = screenCenter.x+radius*cos(angle);
        connections[i].y = screenCenter.y+radius*sin(angle);
        
        connections[i].makeGlyph(subjects,freqscale);
        
        angles[level]+=jumps[level];
    }
}

void Glyphs::createParticles(){
    
    //  Load Categories
    //
    vector<ofVec3f> positions;
    for (int i = 0; i < connections.size(); i++){
            connections[i].getPoints( positions, 3.0);
    }
    
    for(int i = 0; i < positions.size(); i++ ){
        
        //        bool foundDestiny = true;
        //        while ( foundDestiny ) {
        //            int index = 0;//ofRandom( particles.size() );
        //            if ( particles[index]->destinyPos == ofPoint(0,0,0) ){
        //                particles[index]->destinyPos = positions[i];
        //                foundDestiny = false;
        //            }
        //        }
        
        if (i < particles.size()){
            particles[i]->destinyPos = positions[i];
        } else {
            break;
        }
    }
}

void Glyphs::selfSceneTransformation(){
	
}

void Glyphs::selfUpdate(){
    if ( !bDebug && spritePositions != NULL && spritePositions != NULL){
        
        for(int i = 0 ; i < particles.size() ; i++){
            if(neigbordhood>1.0)
                particles[i]->applyFlockingForce(&globalOffset,neigbordhood,independence);
            
            if(networkAttraction>0.0)
                particles[i]->applyGravityForceTo( &particles[i]->networkPos , networkAttraction);
            
            if(destinyAttraction>0.0)
                particles[i]->applyGravityForceTo( &particles[i]->destinyPos , destinyAttraction);
            
            particles[i]->update(speed);
            
            spritePositions[i] = *particles[i];
            spriteColors[i] = particles[i]->color * particlesTintColor;
        }
        
        if(neigbordhood>0.0)
            globalOffset += ofPoint(turbulence/neigbordhood, turbulence/neigbordhood, turbulence/neigbordhood);
        
        spriteVBO.updateVertexData( &spritePositions[0], TOTAL_PARTICLES );
        spriteVBO.updateColorData( &spriteColors[0], TOTAL_PARTICLES );
    }
}

void Glyphs::selfDraw(){
    
    ofPushMatrix();
    ofPushStyle();
    materials["MATERIAL 1"]->begin();
    
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
    
    if (!bDebug && spritePositions != NULL && spritePositions != NULL ){
        ofEnableAlphaBlending();
        {
            ofSetColor(255);
            ofDisableArbTex();
            glDepthMask(GL_FALSE);
            glEnable(GL_NORMALIZE);
            ofEnablePointSprites();
            
            spriteShader.begin();
            spriteTexture.bind();
            
            spriteShader.setUniformTexture("tex", spriteTexture, 0);
            spriteShader.setUniform1f("nearClip", cam.getNearClip());
            spriteShader.setUniform1f("farClip", cam.getFarClip());
            spriteShader.setUniform1f("size",spriteSize);
            spriteShader.setUniform1f("minPointSize", 1.0);//spriteSizeMin);
            spriteShader.setUniform1f("maxPointSize", spriteSizeMax);
            spriteShader.setUniform1f("focusDistance", spriteFocusDistance);
            spriteShader.setUniform1f("focusAperture", spriteFocusAperture*0.01);
            
            spriteVBO.draw(GL_POINTS, 0, TOTAL_PARTICLES );
            
            spriteTexture.unbind();
            spriteShader.end();
            
            ofDisablePointSprites();
            glDisable(GL_NORMALIZE);
            glDepthMask(GL_TRUE);
            ofEnableArbTex();
            
            ofPopStyle();
        }
    }
    
    materials["MATERIAL 1"]->end();
    ofPopStyle();
    ofPopMatrix();
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