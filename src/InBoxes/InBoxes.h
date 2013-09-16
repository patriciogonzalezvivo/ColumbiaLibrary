#pragma once

#include "UITimeProject.h"

#include "UIPostRender.h"
#include "BookParticle.h"
#include "Category.h"

class InBoxes : public UITimeProject {
public:
    
    string getSystemName(){
		return "InBoxes";
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
    
    void    drawBrakets(ofRectangle _rect, float _size, float _margin);
    UIPostRender  post;
    
    //  Scene
    //
    Category            archiveBooks[5];
    ofTrueTypeFont      font;
    vector<BookParticle> publicBooks;
    vector<Category>    categories;
    
    float   totalWith, totalHeight;
    float   bookTurbulence,bookAtraction;
    
    float   debugTranslation, debugScale;
};