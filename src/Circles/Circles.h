//
//  circles.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#pragma once

#include "UITimeProject.h"

#include "DraggableCircle.h"
#include "CParticle.h"

#define TOTAL_LEVELS 2
#define TOTAL_PARTICLES 50000

class Circles : public UITimeProject {
public:
    
    string getSystemName(){
		return "Circles";
	}
    
    void selfSetupGui();
    void selfGuiEvent(ofxUIEventArgs &e);
    
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
    
    void selfSetup();
    
    void selfBegin();
	void selfPresetLoaded(string presetPath);
    void selfSceneTransformation();
	
    void selfUpdate();
    
    void selfDraw();
	void selfDrawDebug();
	void selfDrawBackground();
    
    void selfDrawOverlay();
    void selfPostDraw();
    
	void selfEnd();
    void selfExit();
    
    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);
    
    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseReleased(ofMouseEventArgs& data);
    
protected:
    
    //  Levels of Categories
    //
    void    loadCircles(string _file, vector<DraggableCircle> &_circles);
    vector<DraggableCircle> level[TOTAL_LEVELS];
    
    ofImage                 colorSampleImage;
    
    //  Particles ( Books )
    //
    vector<CParticle*>      particles;
    ofFloatColor            particlesTintColor;
    ofxUIImageSampler*      particlesColorSampler;
    
    ofPoint                 globalOffset;
    float                   speed;
    float                   turbulence,neigbordhood,independence;
    float                   networkAttraction, destinyAttraction;
    
    ofVbo                   spriteVBO;
    ofShader                spriteShader;
    ofTexture               spriteTexture;
    ofVec3f*                spritePositions;
    ofFloatColor*           spriteColors;
    float                   spriteSize,spriteSizeMin,spriteSizeMax,spriteFocusDistance,spriteFocusAperture;
    int                     spriteIndexCount;
    int                     spriteNextIndex;
};