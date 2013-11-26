#pragma once

#include "UITimeProject.h"

#include "Category.h"
#include "BookParticle.h"

class Dimentions : public UITimeProject {
public:
    
    string getSystemName(){
		return "Dimentions";
	}
    
    void selfSetupGuis();
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
    
    ofImage                 colorSampleImage;
    
    //  Categories
    //
    void                    drawBrakets(ofRectangle _rect, float size, float margin);
    vector<Category>        categories;
    ofTrueTypeFont          font;
    ofImage                 mapImage;
    ofxUIImageSampler*      categoriesColorSampler;
    ofxUIImageSampler*      categoriesNodesColorSampler;
    ofFloatColor            categoriesTintColor;
    ofFloatColor            categoriesNodesTintColor;
    float                   debugTranslation, debugScale;
    
    //  Subjects ( Networks Atractors )
    //
    vector<ofVec4f*>        subjects;
    
    //  Books
    //
    void                    assignDataToBooks();
    void                    drawLabel(ofPoint _loc, string _name);
    
    int                     TOTAL_OF_BOOKS;
    vector<BookParticle*>   books;
    vector<BookParticle*>   booksWithTags;
    ofxUIImageSampler*      booksColorSampler;
    ofFloatColor            bookTintColor;
    ofxUIImageSampler*      booksTextColorSampler;
    ofFloatColor            bookTextColor;
    ofPoint                 globalOffset;
    float                   speed;
    float                   turbulence,neigbordhood,independence;
    float                   originAttraction, destinyAttraction, networkAttraction, subjectsAttraction;
    
    vector< vector<int> >   updateIndexMap;
    float                   updateIndex;
    bool                    updateCycle;
    
    ofVbo                   spriteVBO;
    ofShader                spriteShader;
    ofTexture               spriteTexture;
    ofVec3f*                spritePositions;
    ofFloatColor*           spriteColors;
    float                   spriteSize,spriteSizeMin,spriteSizeMax,spriteFocusDistance,spriteFocusAperture;
    int                     spriteIndexCount;
    int                     spriteNextIndex;
    
};