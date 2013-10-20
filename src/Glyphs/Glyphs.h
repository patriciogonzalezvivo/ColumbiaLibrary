//
//  Glyphs.h
//  ColumbiaLibrary
//
//  Created by Patricio Gonzalez Vivo on 9/22/13.
//
//

#pragma once

#include "UITimeProject.h"

#include "Subject.h"
#include "Connection.h"

#define MAX_LEVELS 10

class Glyphs : public UI2DProject{ // UITimeProject {
public:
    
    string getSystemName(){
		return "Glyphs";
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
    void loadStuff();
    
    vector<Subject> subjects;
    vector<Connection> connections;
    
    float radius_levels[MAX_LEVELS];
    float freqscale;

};