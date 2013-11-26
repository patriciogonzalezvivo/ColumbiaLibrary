//
//  Glyphs.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#pragma once
#include "UI3DGrid.h"
#include "UITimeProject.h"

#include "Particle.h"
#include "Spring.h"

#include "Subject.h"
#include "Connection.h"

#include "CParticle.h"
#include "UIText.h"

#define MAX_LEVELS 10
#define TOTAL_PARTICLES 50000

class Glyphs : public UI3DProject{ // UITimeProject {
public:
    
    string getSystemName(){
		return "Glyphs";
	}
    
    void selfSetupGuis();
    void selfGuiEvent(ofxUIEventArgs &e);
    
    void selfSetupSystemGui();
    void guiSystemEvent(ofxUIEventArgs &e);
    
    void selfSetupRenderGui();
    void guiRenderEvent(ofxUIEventArgs &e);
    
    void selfSetup();
    
    void selfBegin();
	
    void selfUpdate();
    
    void selfDraw();
    
    void selfDrawOverlay();
    void selfPostDraw();
    
	void selfEnd();
    void selfExit();
    
    void selfKeyPressed(ofKeyEventArgs & args);
    void selfKeyReleased(ofKeyEventArgs & args);
    
    void selfMouseMoved(ofMouseEventArgs& data);
    void selfMousePressed(ofMouseEventArgs& data);
    void selfMouseDragged(ofMouseEventArgs& data);
    void selfMouseDoublePressed(ofMouseEventArgs &data);
    void selfMouseReleased(ofMouseEventArgs& data);
    
    
protected:
    UI3DGrid    grid;
    ofImage     circle;
    
    Particle    center;
    vector<Spring>  springsToCenter;
    vector<Spring>  springsToSubjects;
    vector<Subject*> subjects;
    vector<Connection*> connections;
    
    UIText  text[5];
    
    ofColor colors_levels[MAX_LEVELS];
    float   radius_levels[MAX_LEVELS];
    float   freqscale;
    float   forceToOrigin;
    float   forceToCenter;
    float   forceToClosest;
    float   forceRepel;
    float   speed;

};