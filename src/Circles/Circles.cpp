//
//  circles.cpp
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#include "Circles.h"

void Circles::selfSetup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    //  DATA
    //
    spriteShader.load(getDataPath()+"shaders/sprites");
    ofDisableArbTex();
    ofLoadImage( spriteTexture , getDataPath()+"images/sprite.png");
    ofEnableArbTex();
    colorSampleImage.loadImage("GUI/defaultColorPalette.png");
    
    //  Particles
    //
    globalOffset.set(0,0,0);
    float networkRadio = 200;
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
    
    //  Load Categories
    //
    vector<ofVec3f> positions;
    for (int i = 0; i < TOTAL_LEVELS; i++){
        loadCircles( getDataPath()+"level"+ofToString(i)+".xml", level[i] );
        for (int j = 0; j < level[i].size(); j++) {
            level[i][j].getPoints( positions, 2.0);
        }
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

void Circles::loadCircles(string _file, vector<DraggableCircle> &_circles){
    ofxXmlSettings XML;
    XML.load( _file );
    int total = XML.getNumTags("circle");
    
    for (int i = 0; i < total; i++) {
        DraggableCircle newCircle;
        newCircle.loadSettings(i, _file);
        newCircle.scale = 600;
        _circles.push_back( newCircle );
    }
}

void Circles::selfSetupGui(){
    
}

void Circles::selfGuiEvent(ofxUIEventArgs &e){
    
}

void Circles::selfSetupSystemGui(){
    sysGui->addLabel("General");
    sysGui->addSlider("speed", 0.1, 1.0, &speed);
    
    sysGui->addLabel("Flocking");
    sysGui->addSlider("turbulence", 0.0, 0.09, &turbulence);
    sysGui->addSlider("neigbordhood", 1, 500, &neigbordhood);
    sysGui->addSlider("independence", 0, 1.0, &independence);
    
    sysGui->addLabel("Attractors");
    sysGui->addSlider("attraction_destiny", 0.0, 1.0, &destinyAttraction);
    sysGui->addSlider("attraction_network", 0.0, 1.0, &networkAttraction);
}

void Circles::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name == "DEBUG"){
        if (bDebug){
            cam.disableMouseInput();
        } else {
            
            for(int j = 0; j < TOTAL_LEVELS; j++){
                for (int i = 0; i < level[j].size(); i++) {
                    level[j][i].saveSettings(i, getDataPath()+"level"+ofToString(j)+".xml" );
                }
            }
            
            cam.enableMouseInput();
        }
    }
}

void Circles::selfSetupRenderGui(){
    rdrGui->addLabel("Sprites");
    rdrGui->addSlider("Sprites_size", 0.0,1.0, &spriteSize);
    //    rdrGui->addSlider("Sprites_size_Min", 1.0,100.0, &spriteSizeMin);
    rdrGui->addSlider("Sprites_size_Max", 1.0,100.0, &spriteSizeMax);
    rdrGui->addSlider("Sprites_focus_Distance", 0.0, 1000.0, &spriteFocusDistance);
    rdrGui->addSlider("Sprites_focus_Aperture", 0.0, 1.0, &spriteFocusAperture);
    particlesColorSampler =  rdrGui->addImageSampler("Sprite_Color_Tint", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );

}

void Circles::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if( name == "Sprite_Color_Tint"){
        ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
        float alpha = particlesTintColor.a;
        particlesTintColor = sampler->getColor();
        particlesTintColor.a = alpha;
    }
}

void Circles::selfPresetLoaded(string presetPath){
	
}

void Circles::selfBegin(){
	
}

void Circles::selfSceneTransformation(){
	
}

void Circles::selfUpdate(){
    if ( !bDebug ){
        
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

void Circles::selfDraw(){
    
    if (!bDebug){
        ofEnableAlphaBlending();
        
        ofPushMatrix();
        ofPushStyle();
        mat->begin();
        
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
            
//            ofPushStyle();
//            ofSetCircleResolution(60);
//            for(int j = 0; j < TOTAL_LEVELS; j++){
//                for (int i = 0; i < level[j].size(); i++) {
//                    level[j][i].draw();
//                }
//            }
            
            ofPopStyle();
        }
    
        mat->end();
        ofPopStyle();
        ofPopMatrix();
    }
}

void Circles::selfDrawDebug(){
}

void Circles::selfDrawBackground(){
    
}

void Circles::selfDrawOverlay(){
    if(bDebug){
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
        ofNoFill();
        for(int j = 0; j < TOTAL_LEVELS; j++){
            for (int i = 0; i < level[j].size(); i++) {
                level[j][i].draw();
            }
        }
        ofPopStyle();
        ofPopMatrix();
    }
}

void Circles::selfPostDraw(){
    UIProject::selfPostDraw();
}

void Circles::selfEnd(){

}

void Circles::selfExit(){
	
}

void Circles::selfKeyPressed(ofKeyEventArgs & args){
	
}

void Circles::selfKeyReleased(ofKeyEventArgs & args){
	
}

void Circles::selfMouseDragged(ofMouseEventArgs& data){
    ofPoint mouse = ofPoint(data.x-ofGetWidth()*0.5,
                            data.y-ofGetHeight()*0.5);
    
	if (bDebug){
        for(int j = 0; j < TOTAL_LEVELS; j++){
            for (int i = 0; i < level[j].size(); i++) {
                if (level[j][i].mouseDragged( mouse )){
                    return;
                }
            }
        }
    }
}

void Circles::selfMouseMoved(ofMouseEventArgs& data){
	
}

void Circles::selfMousePressed(ofMouseEventArgs& data){
	
}

void Circles::selfMouseReleased(ofMouseEventArgs& data){
    if (bDebug){
        cam.disableMouseInput();
    }
}