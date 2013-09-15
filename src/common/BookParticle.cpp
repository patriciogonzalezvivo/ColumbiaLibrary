#include "BookParticle.h"

void BookParticle::init(ofPoint _pos, ofPoint _vel){
    set(_pos);
    vel.set(_vel);
    
    origin.set(_pos);
    destiny = ofPoint(0,0,0);
}

void BookParticle::applyAttractionToNodes(float _pct){
    for (int i = 0; i < nodes.size(); i++) {
        addAttractionForce(ofPoint( nodes[i]->x, nodes[i]->y, nodes[i]->z ), 5000, _pct);
        addRepulsionForce(ofPoint( nodes[i]->x, nodes[i]->y, nodes[i]->z ), nodes[i]->w, 1.0);
    }
}