#include "testApp.h"
#include "ofSoundFile.h"
//--------------------------------------------------------------
void testApp::setup(){
    
    gui.setup();
    gui.add(multiplier.setup("multiplier", 1, 1, 255));
    gui.add(waveformOpacity.setup("waveformOpacity", 100, 0, 255));
    gui.add(xoffset.setup("X offset", 0,0,200));
    
    multiplier.addListener(this, &testApp::multiplierChanged);
    waveformOpacity.addListener(this, &testApp::waveformOpacityChanged);
    
    bNeedRedraw = true;
    
    fbo.allocate(ofGetWidth(),ofGetHeight(), GL_RGBA);
    
	fbo.begin();
	ofClear(255,255,255, 0);
    fbo.end();
    bDrawGui = true;
    bDrawSparse = false;
    cargar();
}
//--------------------------------------------------------------
void testApp::multiplierChanged(int &i){
    bNeedRedraw = true;
}
//--------------------------------------------------------------
void testApp::waveformOpacityChanged(int &i){
    multiplier = i;
    waveformOpacity = multiplier;
    bNeedRedraw = true;
}
//--------------------------------------------------------------
void testApp::update(){
    if (constQ.isProcesado) {
        if (bNeedRedraw) {
            fbo.begin();
            ofSetColor(255);
            constQ.drawBands();
            constQ.draw(multiplier);
            ofSetColor(255,waveformOpacity);
            waveform();
            ofSetColor(255);
            //constQ.drawSingle(multiplier);
            fbo.end();
            bNeedRedraw = false;
            cout << "redrawn!"<< endl;
        }else if(bDrawSparse){
            fbo.begin();
            ofSetColor(255);
            constQ.drawSparse();
            ofSetColor(255,waveformOpacity);
            waveform();
            fbo.end();
            bDrawSparse = false;
        }
    }
}
//--------------------------------------------------------------
void testApp::cargar(){
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select audio file..."); 
    if (openFileResult.bSuccess){
        ofSoundFile sound;
        sound.open(openFileResult.getPath());
        ofSoundBuffer buff;
        sound.readTo(buff);
        sampRate = sound.getSampleRate();
        audio = buff.getBuffer();
        constQ.setup(220, 7040, 96, sampRate, 0.0054 );//float minFreq, float maxFreq, int bins per octave, sampleRate, float threshold for sparseKernel(don't change unless you know what it does)
        constQ.processAll(audio,true);
    }
}
//--------------------------------------------------------------
void testApp::waveform(){
    if (audio.size() >0) {
        ofPushMatrix();
        float h=float(ofGetHeight())/5;
        float s1=audio[0];
        float s2;
        float factor=float(ofGetWidth())/audio.size();
        ofPushStyle();
        ofSetColor(255);
        for(int i=1; i<audio.size(); i++){
            s2=audio[i];
            ofLine(factor*(i-1),(s1+1)*h/2,factor*i,(s2+1)*h/2);
            s1=s2;
        }
        ofPopMatrix();
        ofPopStyle();
    }
}
//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    fbo.draw(0,0);
    if (bDrawGui) {
        gui.draw();
    }
    ofDrawBitmapStringHighlight(ofToString(constQ.getPorcentaje() + "% processed."), 50, ofGetHeight() -50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key=='p'){
        constQ.printSpar();
    }else if(key == 'g'){
        bDrawGui ^=true;
    }else if(key == 'r'){
        bNeedRedraw = true;
        bDrawSparse = false;
    }else if(key == 's'){
        bDrawSparse = true;
        bNeedRedraw = false;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}