////////////////////////////
/////Project Name: Okagesama
/////12.03.2017
/////Written by Soonpart
/////soonpart@gmail.com
////////////////////////////

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // listen on the given port
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
    
    ofSetCircleResolution(60);
    ofBackground(50);
    music.load("catOnMarsReverse.mp3");
    music.setLoop(true);
    volume = 0.5;
    music.play();
    
    groundHeight = 350;
    x = ofGetWidth()/2;
    y = groundHeight - 25;
    xv = 2;
    yv = 0;
    gravity = 2;
    radiusN = 50;
    radius = radiusN;
    
    playing = false;
    gameOver = false;
    
    faceTH = 0.15; //threshold
    eyebrowTH = 1.5; //threshold
    mouthTH = 3; //threshold
    eyebrowN = 100; //set it high to avoid error
    
    playerC = 0; //player color
    shadowC = 100; //shadow color
    
    //set noise seed for noise function
    for(int i=0; i<30; i++) noiseSeeds.push_back(ofRandom(1000));
    
    mouthOpen = false;
    picked = false;
    haveItem = false;
    
    count = 0; //count for shadow spreading duration
    spreadingLength = 100; //shadow spreading duration
    spreading = false;
    shadowSize = 12;
    darkness = 0;
    
    getItem.load("twinkleReverse.wav");
    
    points.push_back(ofPoint(ofRandom(ofGetWidth()), ofRandom(groundHeight)));
}

//--------------------------------------------------------------
void ofApp::update(){
    //set music volume
    music.setVolume(volume);
    
    //FaceOSC receiver: check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        // check for eyebrow height left
        if(m.getAddress() == "/gesture/eyebrow/left") eyebrowL = m.getArgAsFloat(0);
        
        // check for eyebrow height right
        else if(m.getAddress() == "/gesture/eyebrow/right") eyebrowR = m.getArgAsFloat(0);
        
        // check for mouth height
        else if(m.getAddress() == "/gesture/mouth/height") mouthH = m.getArgAsFloat(0);
        
        //check for horizontal directioin of face
        else if(m.getAddress() == "/pose/orientation") faceX = m.getArgAsFloat(1);
        
        eyebrow = (eyebrowL + eyebrowR)/2; //get average of eyebrows
        mouth = int(mouthH); //get int value of mouth
    }
    
    
    //X axes movement
    if(faceX<-faceTH) x -= xv; //if face towars left move to left
    else if(faceX>faceTH) x += xv; //if face towars right move right
    
    
    //Y axes movement
    if(eyebrow - eyebrowN > eyebrowTH) yv = -eyebrow; //eyebrow controls vertical movement
    else if (y < groundHeight - radius/2) yv = gravity; //back to ground with gravity
    else yv = 0; //no Y axes movement after it gets ground
    y += yv;
    
    
    //is mouth open?
    if(mouth>mouthTH) mouthOpen == true;
    else mouthOpen == false;
    
    
    //set radius
    radius = radiusN + mouthH; // when mouth is open body gets bigger
    
    
    //game system
    if(playing){
        //collision
        if(!haveItem && !picked && ofDist(x, y, points[0].x, points[0].y) < radius){
            points.erase(points.begin());
            haveItem = true;
            picked = true;
            count = spreadingLength; //fill up the count
            getItem.play();
        }
        
        //set each shadow parameter
        else if(haveItem && picked && mouth > mouthTH && count > 0){
            shadows.push_back(ofPoint(x, y, mouthH * shadowSize)); //z parameter is for radius
            count--; //can spread the shadow while count left
            spreading = true;
        }
        else if(haveItem && picked && count == 0){
            spreading = false; //as count gets 0 stop creating shadow
            haveItem = false;
        }
        
        //set item location
        else if(!haveItem && picked && !spreading && count == 0){
            points.push_back(ofPoint(ofRandom(ofGetWidth()), ofRandom(groundHeight)));
            picked = false;
        }
    }
    
    //check how much the shadow spread
    
    //grab the screenshot
    screen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    
    //reset number of shadow pixels
    numShadowPixelTemp = 0;
    
    //check all pixels color to see if it is shadow
    for (int y = 0; y < groundHeight; y += 2) { //every another pixel to avoid slow process
        for (int x = 0; x < ofGetWidth(); x += 2) { //every another pixel to avoid slow process
            ofColor color = screen.getColor(x, y);
            if(color == shadowC) numShadowPixelTemp++; //how many pixels are shadow
        }
    }
    
    //keep the biggest number always as the charactor's overlaps declease the number
    if(numShadowPixelTemp > numShadowPixel) numShadowPixel = numShadowPixelTemp;
    
    int numCheckedPixel = ofGetWidth() * groundHeight /2 /2;
    int pixelTH = 3000; //threshold
    
    darkness = ofMap(numShadowPixel, 0, numCheckedPixel - pixelTH, 0, 255, true);
}


//--------------------------------------------------------------
void ofApp::draw(){
    //keep background color (light gray) slightly changing all the time to dipict natural light
    bg = ofColor(245 + 20 * (-0.5 + ofNoise(noiseSeeds[0])), 245 + 20 * (-0.5 + ofNoise(noiseSeeds[1])), 245 + 20 * (-0.5 + ofNoise(noiseSeeds[2])));
    playerC = ofMap(darkness, 0, 255, 0, 50);
    shadowC = ofMap(darkness, 0, 255, 100, 50);
    
    ofSetColor(bg, 255 - darkness); //disappears as darkness gets bigger
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    
    //draw shadows
    ofSetColor(shadowC);
    for(int i = 0; i < shadows.size(); i++){
        ofDrawCircle(shadows[i].x, shadows[i].y, shadows[i].z);
        ofDrawCircle(shadows[i].x - shadows[i].z * 4/5, shadows[i].y - shadows[i].z * 4/5, shadows[i].z * 2/5);
        ofDrawCircle(shadows[i].x + shadows[i].z * 4/5, shadows[i].y - shadows[i].z * 4/5, shadows[i].z * 2/5);
    }
    
    
    //draw body
    ofSetColor(playerC);
    ofDrawCircle(x, y, radius);
    ofDrawCircle(x - radius * 4/5, y - radius * 4/5, radius * 2/5);
    ofDrawCircle(x + radius * 4/5, y - radius * 4/5, radius * 2/5);
    ofDrawRectangle(x - radius, y, radius * 2, groundHeight - y + radius * 2);
    
    
    //draw ground
    ofSetColor(playerC);
    ofDrawRectangle(0, groundHeight, ofGetWidth(), ofGetHeight());
    
    
    //obliteration
    if(gameOver){
        ofSetColor(50);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    
    //draw eyes
    ofPushMatrix();
    ofTranslate(x, y);
    eyeLX = -(radius * 23/50) + faceX * radius * 2/5;
    eyeRX = radius * 23/50 + faceX * radius * 2/5;
    eyeLY = 0;
    eyeRY = 0;
    float noiseMovement = radius * (2 * ofNoise(noiseSeeds[0]) - 1)/25;
    
    //draw eye balls
    float darkEye = ofMap(count, 0, spreadingLength, 255, 100);
    ofSetColor(bg, darkEye);
    drawEye(eyeLX, eyeLY, radius * 3/10);
    drawEye(eyeRX, eyeRY, radius * 3/10);
    
    //draw pupils
    ofSetColor(playerC);
    ofDrawEllipse(eyeLX + noiseMovement, eyeLY, radius * 2/5 + noiseMovement, radius * 1/5 + radius * 1/5 / mouthH);
    ofDrawEllipse(eyeRX + noiseMovement, eyeRY, radius * 2/5 + noiseMovement, radius * 1/5 + radius * 1/5 / mouthH);
    ofPopMatrix();
    
    
    //draw item
    if(playing && !picked){
        ofSetColor(playerC); //item is always black
        drawEye(points[0].x, points[0].y, 20);
    }
    
    
    
    //draw gauge and instraction
    if(playing && !gameOver) {
        //gauge outline
        ofSetColor(darkness, 50);
        ofDrawRectangle(ofGetWidth()/20, ofGetHeight()* 1/20, ofGetWidth() * 5/20, 10);
        
        //gauge
        ofSetColor(darkness);
        ofDrawBitmapString("Dark Matter", ofGetWidth()/20, ofGetHeight()* 2/20);
        float gauge = ofMap(count, 0, spreadingLength, 0, ofGetWidth() * 5/20);
        ofFill();
        ofDrawRectangle(ofGetWidth()/20, ofGetHeight()* 1/20, gauge, 10);
        
        //introduction
        ofSetColor(bg);
        ofDrawBitmapString("Eyebrow to extend shadow.   Mouth to spread shadow.   Face angle to move.", ofGetWidth()/20, ofGetHeight()* 19/20);
    }
    
    
    //setting
    if(!playing){
        ofSetColor(0);
        string intro;
        intro = "Keep your eyebrows in a neutral position\nand open your mouth widely to start.";
        ofDrawBitmapString(intro, ofGetWidth()/4, ofGetHeight()/3);
        if(mouth > 3){
            eyebrowN = eyebrow;
            mouthN = mouth;
            playing = true;
        }
    }
    
    //finale
    if(darkness >= 250) gameOver = true;
    if(gameOver){
        ofSetColor(bg);
        string intro;
        intro = "Congratulations.\nYou have spread enough of your shadow\nand safely obliterated yourself into it.\nNow you are nobody.";
        ofDrawBitmapString(intro, ofGetWidth()/4, ofGetHeight()/3);
    }
    
    
///////////////DEBUG USE ONLY//////////////////////////////
//    //draw parameter
//        ofSetColor(darkness);
//        buf = ofToString(haveItem, 4) +  " " + ofToString(picked, 4) +  " " + ofToString(faceX, 4) +  " " + ofToString(count, 4) +  " " + ofToString(darkness, 4) +  " " + ofToString(numShadowPixel, 4);
//        ofDrawBitmapString(buf, 10, 20);
////////////////////////////////////////////////////////////

}

//--------------------------------------------------------------
///////////////DEBUG USE ONLY//////////////////////////////
void ofApp::keyPressed(int key){
    //set neutral position of face
    if (key == 'n'){
        eyebrowN = eyebrow;
        mouthN = mouth;
        playing = true;
    }
    
    if (key == OF_KEY_DOWN) volume -= 0.1;
    if (key == OF_KEY_UP) volume += 0.1;
}
////////////////////////////////////////////////////////////


//--------------------------------------------------------------
void ofApp::drawEye(float x,float y, float radius)
{
    int resolution = 30;
    float angleStep = 360/resolution;
    ofPushMatrix();
    ofTranslate(x, y);
    ofFill();
    ofBeginShape();
    for (int i=0; i<resolution; i++)
    {
        float noisedRadius = radius * (1 + 0.2 * ofNoise(noiseSeeds[i]));
        float endX = sin(ofDegToRad(i*angleStep)) * noisedRadius;
        float endY = cos(ofDegToRad(i*angleStep)) * noisedRadius;
        noiseSeeds[i] += 0.01;
        ofVertex(endX, endY);
    }
    ofEndShape(true);
    ofPopMatrix();
}














