//
//  Dimentions.cpp
//
//  Created by Patricio Gonzalez Vivo on 8/10/13.
//
//

#include "Dimentions.h"

float phi = (1.0+sqrtf(5.0))/2.0;

bool sortByCategory(  BookParticle * a, BookParticle * b ) {
	return a->category < b->category;
}

void Dimentions::selfSetup(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    font.loadFont("GUI/NewMedia Fett.ttf", 14, false, false, true);

    spriteShader.load(getDataPath()+"shaders/sprites");
    ofDisableArbTex();
    ofLoadImage( spriteTexture , getDataPath()+"images/square-sprite.png");
    ofEnableArbTex();
    
    mapImage.loadImage(getDataPath()+"images/map.jpg");
    colorSampleImage.loadImage("GUI/defaultColorPalette.png");
    
    
    //  Categories
    //
    categories.clear();
    for (int i = 0; i < 13; i++) {
        Category cat;
        cat.loadSettings(i,getDataPath()+"categories.xml");
        cat.font = &font;
        cat.color.a = 0.0;
        cat.borderColor.a = 0.0;
        
        //  Add the nodes to our subjects list ( subcategories )
        //
        for(int j = 0; j < cat.nodes.size(); j++){
            subjects.push_back(cat.nodes[j]);
        }
        
        categories.push_back(cat);
    }
    
    debugTranslation = 0.5;
    debugScale = 1.25;
    float networkRadio = 500;
    
    //  Books
    //
    globalOffset.set(0,0,0);
    
    TOTAL_OF_BOOKS = 50000;
    for(int i = 0; i < TOTAL_OF_BOOKS; i++){
        BookParticle *p = new BookParticle();
        p->init( ofPoint(ofRandom(-10,10),ofRandom(-10,10), -500), ofPoint(ofRandom(-0.1,0.1),ofRandom(-0.1,0.1),0.0) );
        p->destiny = ofPoint(0,0,0);
        p->color.set(ofRandom(0.5,1.0));
        
        ofQuaternion xRot,yRot;
        xRot.makeRotate(ofRandom(-180,180), 1, 0, 0);
        yRot.makeRotate(ofRandom(-180,180), 0, 1, 0);
        p->networkPos = xRot * yRot * ofPoint(0,0,ofRandom(-networkRadio,networkRadio) );
        
        p->text = ofToString(i);
        books.push_back(p);
    }
       
    assignDataToBooks();
    
    //  Devide the particles in 4 subgroups
    //
    updateIndexMap.clear();
    int numberOfSubgroups = 4;
    int jump = books.size()/4;
    for( int i = 0; i < numberOfSubgroups ; i++ ){
        vector<int> beginEnd;
        beginEnd.push_back(i*jump);
        beginEnd.push_back((i+1)*jump);
        updateIndexMap.push_back( beginEnd );
    }
    
    updateIndex = 0.0;
    updateCycle = true;

    //  Setup VBO for fast rendering
    //
    spritePositions = new ofVec3f[ TOTAL_OF_BOOKS ];
    spriteColors    = new ofFloatColor[ TOTAL_OF_BOOKS ];
    spriteVBO.setVertexData( &spritePositions[0], TOTAL_OF_BOOKS, GL_DYNAMIC_DRAW );
    spriteVBO.setColorData( &spriteColors[0], TOTAL_OF_BOOKS, GL_DYNAMIC_DRAW );
    
}

void Dimentions::assignDataToBooks(){
    
    //  Clear the categories counters
    //
    for (int i = 0; i < categories.size(); i++) {
        categories[i].canHold = ((float)TOTAL_OF_BOOKS)*categories[i].totalPct;
    }
    
    // MESS WITH THE DATA
    //
    for(int i = 0; i < books.size() ; i++ ){
        
        books[i]->nodes.clear();
        
        //  Assign Books to Random Categories with out 
        //
        bool place = true;
        while ( place ){
            int randomCat = ofRandom(categories.size());
            
            if ( categories[randomCat].canHold > 0) {
            
                vector<ofPoint> positions;
                
                //  Asign the position of the category
                categories[randomCat].getPoints(positions);
                books[i]->destiny = positions[ofRandom(positions.size())];
                books[i]->category = randomCat;
                categories[randomCat].canHold--;
        
                //  Asign one random subcategory ( subject ) of the category stash
                int totalNodes = categories[randomCat].nodes.size();
                if (totalNodes > 0){
                    books[i]->nodes.push_back( categories[randomCat].nodes[ ofRandom(totalNodes) ] );
                }
                
                place = false;
            }
        }
        
        int randomNodesNumber = ofRandom(0,2);
        for (int j = 0; j < randomNodesNumber; j++) {
            books[i]->nodes.push_back( subjects[ofRandom(subjects.size())] );
        }
    }
    
    //  Sort by Category
    //
//    sort(books.begin(), books.end(), sortByCategory);
//    updateIndexMap.clear();
//    
//    int lastIndex = 0;
//    int lastCategory = 0;
//    for( int i = 0; i < books.size() ; i++ ){
//        if ( books[i]->category > lastCategory){
//            
//            vector<int> beginEnd;
//            beginEnd.push_back(lastIndex);
//            beginEnd.push_back(i);
//            
//            updateIndexMap.push_back( beginEnd );
//            
//            lastIndex = i;
//            lastCategory = books[i]->category;
//        }
//    }
//    
//    vector<int> beginEnd;
//    beginEnd.push_back(lastIndex);
//    beginEnd.push_back(books.size());
//    updateIndexMap.push_back( beginEnd );
    
    //  Edit Important books
    //
    books[0]->text = "Calculus";
    books[0]->category = 11;
    vector<ofPoint> positions;
    categories[ books[0]->category ].getPoints(positions);
    books[0]->destiny = positions[ofRandom(positions.size())];
    books[0]->nodes.clear();
    books[0]->nodes.push_back( categories[11].nodes[2] );
    books[0]->nodes.push_back( categories[2].nodes[0] );
    booksWithTags.push_back(books[0]);
}

void Dimentions::selfSetupGuis(){
    backgroundSet(new UISuperBackground());
}

void Dimentions::selfGuiEvent(ofxUIEventArgs &e){

}

void Dimentions::selfSetupSystemGui(){
    sysGui->addLabel("General");
    sysGui->addSlider("speed", 0.1, 1.0, &speed);
    
    sysGui->addLabel("Flocking");
    sysGui->addSlider("turbulence", 0.0, 0.09, &turbulence);
    sysGui->addSlider("neigbordhood", 1, 500, &neigbordhood);
    sysGui->addSlider("independence", 0, 1.0, &independence);
    
    sysGui->addLabel("Attractors");
    sysGui->addSlider("attraction_origin", 0.0, 1.0, &originAttraction);
    sysGui->addSlider("attraction_destiny", 0.0, 1.0, &destinyAttraction);
    sysGui->addSlider("attraction_network", 0.0, 1.0, &networkAttraction);
    sysGui->addSlider("attraction_subjects", 0.0, 1.0, &subjectsAttraction);
    
//    sysGui->addSlider("Update", 0, updateIndexMap.size(), &updateIndex );
//    sysGui->addToggle("Update_Cycle", &updateCycle);
}

void Dimentions::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if (name == "DEBUG"){
        if (bDebug){
            camera.disableMouseInput();
            timeline->stop();
            bPlaying = false;
        } else {
            
            for(int i = 0; i < categories.size(); i++){
                categories[i].saveSettings(i,getDataPath()+"categories.xml");
            }
            
            assignDataToBooks();
            
            camera.enableMouseInput();
        }
    }
}

void Dimentions::selfSetupRenderGui(){
    
    rdrGui->addLabel("Books Sprites");
    rdrGui->addSlider("Sprites_size", 0.0,1.0, &spriteSize);
//    rdrGui->addSlider("Sprites_size_Min", 1.0,100.0, &spriteSizeMin);
    rdrGui->addSlider("Sprites_size_Max", 1.0,100.0, &spriteSizeMax);
    rdrGui->addSlider("Sprites_focus_Distance", 0.0, 1000.0, &spriteFocusDistance);
    rdrGui->addSlider("Sprites_focus_Aperture", 0.0, 1.0, &spriteFocusAperture);
    booksColorSampler =  rdrGui->addImageSampler("Sprite_Color_Tint", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
//    rdrGui->addSlider("Sprites_Color_Alpha", 0.0, 1.0, &bookTintColor.a);
    booksColorSampler =  rdrGui->addImageSampler("Books_Text_Color", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
    
    rdrGui->addLabel("Categories");
    categoriesColorSampler =  rdrGui->addImageSampler("Categories_Color_Tint", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
    rdrGui->addSlider("Categories_Color_Alpha", 0.0, 1.0, &categoriesTintColor.a);
    
    categoriesNodesColorSampler = rdrGui->addImageSampler("Categories_Nodes_Color_Tint", &colorSampleImage, (float)colorSampleImage.getWidth()/2, (float)colorSampleImage.getHeight()/2 );
    rdrGui->addSlider("Categories_Nodes_Alpha", 0.0, 1.0, &categoriesNodesTintColor.a);
}

void Dimentions::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if( name == "Books_Text_Color"){
        ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
        bookTextColor = sampler->getColor();
    } else if( name == "Sprite_Color_Tint"){
        ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
        float alpha = bookTintColor.a;
        bookTintColor = sampler->getColor();
        bookTintColor.a = alpha;
    } else if( name == "Categories_Color_Tint"){
        ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
        float alpha = categoriesTintColor.a;
        categoriesTintColor = sampler->getColor();
        categoriesTintColor.a = alpha;
        
        for (int i = 0; i < categories.size(); i++) {
            categories[i].textColor = categoriesTintColor;
        }
    } else if( name == "Categories_Color_Alpha"){
        for (int i = 0; i < categories.size(); i++) {
            categories[i].textColor = categoriesTintColor;
        }
    } else if( name == "Categories_Nodes_Color_Tint"){
        ofxUIImageSampler* sampler = (ofxUIImageSampler *) e.widget;
        float alpha = categoriesNodesTintColor.a;
        categoriesNodesTintColor = sampler->getColor();
        categoriesNodesTintColor.a = alpha;
    }
    
}

void Dimentions::selfPresetLoaded(string presetPath){
	
}

void Dimentions::selfBegin(){
	
}

void Dimentions::selfSceneTransformation(){
	
}

void Dimentions::selfUpdate(){
    if (!bDebug && updateIndex < categories.size()){
        
        if (updateCycle){
            updateIndex=(((int)updateIndex)+1)%updateIndexMap.size();
        }
        
        int begin = updateIndexMap[ updateIndex ][0];
        int end = updateIndexMap[ updateIndex ][1];
        for(int i = begin ; i < end ; i++){
            if(neigbordhood>1.0)
                books[i]->applyFlockingForce(&globalOffset,neigbordhood,independence);
            
            if (originAttraction>0.0)
                books[i]->addSlowdonForceTo( books[i]->origin , originAttraction);
            
            if(destinyAttraction>0.0)
                books[i]->addSlowdonForceTo( books[i]->destiny , destinyAttraction);
            
            if(networkAttraction>0.0)
                books[i]->addSlowdonForceTo( books[i]->networkPos , networkAttraction);
            
            if(subjectsAttraction>0.0)
                books[i]->applyAttractionToNodes(subjectsAttraction);
            
            if(!updateCycle)
                books[i]->update(speed);
        }
        
        if(neigbordhood>0.0)
            globalOffset += ofPoint(turbulence/neigbordhood, turbulence/neigbordhood, turbulence/neigbordhood);
        
        for(int i = 0 ; i < books.size(); i++){
            
            if(updateCycle)
                books[i]->update(speed);
            
            spritePositions[i] = *books[i];
            spriteColors[i] = books[i]->color * bookTintColor;
        }
        
        spriteVBO.updateVertexData( &spritePositions[0], TOTAL_OF_BOOKS );
        spriteVBO.updateColorData( &spriteColors[0], TOTAL_OF_BOOKS );
    }
}

void Dimentions::selfDraw(){
    
    if (!bDebug){
        ofEnableAlphaBlending();
        
        ofPushMatrix();
        ofPushStyle();
        materials["MATERIAL 1"]->begin();
        
        //  Labels
        //
        if(categoriesTintColor.a > 0.0 ){
            for(int i = 0; i < categories.size(); i++){
                ofSetColor(categoriesTintColor);
                drawBrakets(categories[i], 15, 10);
                
                categories[i].draw(bDebug);
            }
        }
        
        for (int i = 0; i < booksWithTags.size(); i++) {
            drawLabel( *booksWithTags[i], booksWithTags[i]->text );
        }
        
        if(categoriesNodesTintColor.a > 0.0){
            for(int i = 0; i < categories.size(); i++){
                for(int j = 0; j < categories[i].nodes.size(); j++){
                    ofPushMatrix();
                    ofTranslate(categories[i].nodes[j]->x,
                                categories[i].nodes[j]->y,
                                categories[i].nodes[j]->z);
                    ofSetColor(categoriesNodesTintColor);
                    float scale = 0.4;
                    ofScale(scale,scale,scale);
                    //orientBillBoard();
                    ofRectangle box = font.getStringBoundingBox(categories[i].nodesText[j], 0, 0);
                    font.drawStringAsShapes(categories[i].nodesText[j], -box.width*0.5,0);
                    ofPopMatrix();
                }
            }
        }
    
        
        {
            ofSetColor(255);
            ofDisableArbTex();
            glDepthMask(GL_FALSE);
            glEnable(GL_NORMALIZE);
            ofEnablePointSprites();
            
            spriteShader.begin();
            spriteTexture.bind();
            
            spriteShader.setUniformTexture("tex", spriteTexture, 0);
            spriteShader.setUniform1f("nearClip", getCameraRef().getNearClip());
            spriteShader.setUniform1f("farClip", getCameraRef().getFarClip());
            spriteShader.setUniform1f("size",spriteSize);
            spriteShader.setUniform1f("minPointSize", 1.0);//spriteSizeMin);
            spriteShader.setUniform1f("maxPointSize", spriteSizeMax);
            spriteShader.setUniform1f("focusDistance", spriteFocusDistance);
            spriteShader.setUniform1f("focusAperture", spriteFocusAperture*0.01);
            
            spriteVBO.draw(GL_POINTS, 0, TOTAL_OF_BOOKS );
            
            spriteTexture.unbind();
            spriteShader.end();
            
            ofDisablePointSprites();
            glDisable(GL_NORMALIZE);
            glDepthMask(GL_TRUE);
            ofEnableArbTex();
        }

        materials["MATERIAL 1"]->end();
        ofPopStyle();
        ofPopMatrix();
    }
}

void Dimentions::selfDrawDebug(){
}

void Dimentions::selfDrawBackground(){
    
    if (bDebug){
        ofEnableAlphaBlending();
        
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(ofGetWidth()*debugTranslation, ofGetHeight()*debugTranslation);
        ofScale(1.0/debugScale,(1.0/debugScale)*-1.0,1.0/debugScale);
        
        //mapImage.draw(-mapImage.getWidth()*0.5,mapImage.getHeight()*0.5,mapImage.getWidth(), -mapImage.getHeight());
        
        //  Draw black book boxes
        //
        for(int i = 0; i < categories.size(); i++){
            categories[i].draw(bDebug);
        }
        
        ofPopStyle();
        ofPopMatrix();
    }
    
}

void Dimentions::selfPostDraw(){
    if (bDebug){
        UITimeProject::selfPostDraw();
    } else {
        UIProject::selfPostDraw();
    }
}

void Dimentions::drawBrakets(ofRectangle _rect, float size, float margin){
    
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

void Dimentions::drawLabel(ofPoint _loc, string _text){
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(_loc);
	camera.billboard();
    
	// DRAW
    ofSetColor(bookTextColor);
    
    float arm = 15;
    ofLine(0,0, 10, 10);
    ofLine(10, 10,arm,10);
    
    float scale = 0.4;
    ofScale(scale,scale,scale);
//    font.drawString(_text, arm , 5 + 10);
    font.drawStringAsShapes(_text, arm , 5 + 10);
    
    ofPopStyle();
	ofPopMatrix();
}

//void Dimentions::orientBillBoard(){
//    ofVec3f objectLookAt = ofVec3f(0,0,1);
//	
//    ofVec3f objToCam = camera.getGlobalPosition();
//    
//	objToCam.normalize();
//	float theta = objectLookAt.angle(objToCam);
//	
//	ofVec3f axisOfRotation = objToCam.crossed(objectLookAt);
////	cout << " . " << axisOfRotation << endl;
//    axisOfRotation.normalize();
////    cout << " Normaliced: " << axisOfRotation << endl;
//    
////	glRotatef(-zRot->getPos(), 0.0, 0.0, 1.0);
////	glRotatef(-yRot->getPos(), 0.0, 1.0, 0.0);
////	glRotatef(-xRot->getPos(), 1.0, 0.0, 0.0);
//	glRotatef(-theta, axisOfRotation.x, axisOfRotation.y, axisOfRotation.z);
//}

void Dimentions::selfEnd(){
//	for(int i = books.size()-1; i >= 0; i++){
//        delete books[i];
//    }
    books.clear();
    
    delete []spritePositions;
    delete []spriteColors;
}

void Dimentions::selfExit(){
	
}

void Dimentions::selfKeyPressed(ofKeyEventArgs & args){
	
}
void Dimentions::selfKeyReleased(ofKeyEventArgs & args){
	
}

void Dimentions::selfMouseDragged(ofMouseEventArgs& data){
    ofPoint mouse = ofPoint(data.x,data.y);
    
	if (bDebug){
        mouse -= ofPoint(ofGetWidth()*debugTranslation,ofGetHeight()*debugTranslation);
        mouse *= debugScale;
        mouse.y *= -1.0;
        
        for(int i = 0; i < categories.size(); i++){
            categories[i].mouseDragged( mouse );
        }
    }
}

void Dimentions::selfMouseMoved(ofMouseEventArgs& data){
	
}

void Dimentions::selfMousePressed(ofMouseEventArgs& data){
	
}

void Dimentions::selfMouseReleased(ofMouseEventArgs& data){
	if (bDebug){
        camera.disableMouseInput();
    }
}