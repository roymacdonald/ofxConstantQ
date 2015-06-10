//
//  ofxConstantQ.h
//  ofxConstantQExample
//
//  Created by Roy Macdonald on 8/9/13.
//  Copyright (c) 2013 micasa. All rights reserved.
//
#pragma once
#include "sparseArray.h"
#include "sparseComplex.h"
#include "complex.h"
#include "fft.h"
#include "ofMain.h"
class ofxConstantQ : public ofThread{
public:
    ofxConstantQ():bIsSetup(false){}
    ~ofxConstantQ();
    
    void setup(float minFreq, float maxFreq, int bins, int fs, float thresh);
    

    
    int sparseSize();
    void drawSparse();
    void draw(int m);
    void drawBands();
    void drawSingle(int m);

    
    void processAll(vector<float> a, bool overlap);
    
    vector<float> applyWindow(vector<float> a);
    
    vector<float> process(vector<float> x);
    
    void printSpar();
    
   // vector<complex> sparse(vector<complex> c);
    
    
    vector <sparseArray> sparKernel;
    float Q;
    int K;
    int fftLen;
    int sparLen;
    vector<vector<float> > procesado;
    bool isProcesado;
    float maximo;
    float maxF;
    float minF;
    int bns;
    vector<float> hammingWindow;
    void start();
    void stop();
    int getPorcentaje();
    bool bIsReady(){return isProcesado;}
private:
    bool bIsSetup;
    void processData();
    vector<float> data;
    bool bOverlaped;
    int porcentaje;
    void threadedFunction();
    void sparseKernel(float minFreq, float maxFreq, int bins, int fs, float thresh);


};
