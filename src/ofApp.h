////////////////////////////
/////Project Name: Okagesama
/////12.03.2017
/////Written by Soonpart
/////soonpart@gmail.com
////////////////////////////

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// listen on port 8338
#define PORT 8338
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
        void keyPressed(int key); //debug use only
        void drawEye(float x,float y, float radius);

		ofxOscReceiver receiver;

        float eyebrowL, eyebrowR, mouthH, faceX;
        float groundHeight;
        float eyebrow, mouth, radius, eyebrowN, mouthN, radiusN;

        bool playing, gameOver;

        int x, y, xv, yv, gravity, mouthX, mouthY, eyeLX, eyeRX, eyeLY, eyeRY;
        float faceTH, eyebrowTH, mouthTH;
    
        ofColor bg, playerC, shadowC;
        vector <float> noiseSeeds;
    
        vector <ofPoint> points;
        vector <ofPoint> shadows;
        bool mouthOpen, picked, haveItem, spreading;
        int count, spreadingLength, shadowSize;
    
        ofImage screen;
        int darkness;
        int numShadowPixel, numShadowPixelTemp;
    
        ofSoundPlayer getItem;
    
        ofSoundPlayer music;
        float volume;
};
