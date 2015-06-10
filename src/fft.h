//
//  fft.h
//  ofxConstantQExample
//
//  Created by Roy Macdonald on 8/9/13.
//  Copyright (c) 2013 micasa. All rights reserved.
//
#pragma once
#include "ofMain.h"



class fft{
public:
    fft() {}
    void set(int ts = 128){
        timeSize = ts;
        // sampleRate = (int)sr;
        //bandWidth = (2.0 / (float)timeSize) * ((float)sampleRate / 2.0);
       
        real.clear();
        imag.clear();
        spectrum.clear();
        allocateArrays();
        if((timeSize & timeSize - 1) != 0) {
            //cout << "FFT: timeSize must be a power of two.";
        }else {
            rvrs.clear();
            sinlookup.clear();
            coslookup.clear();
            fillVector(rvrs, timeSize, 0);//rvrs.reserve(timeSize);
           // fillVector(sinlookup, timeSize, 0.0f);//sinlookup.reserve(timeSize);
            //fillVector(coslookup, timeSize, 0.0f);//coslookup.reserve(timeSize);
            buildReverseTable();
            buildTrigTables();
        }
    }
    vector<float> getSpectrum(){
        return spectrum;
    }
    
    void forward(vector<vector<float> > buffer) {
        
        bitReverseSamples(buffer);
        doFft();
        fillSpectrum();
        //return;  
    }
    void forward(vector<float> buffer) {
        if(buffer.size() != timeSize) {
          // println("FFT.forward: The length of the passed sample buffer must be equal to timeSize().");
            return;
        } 
        else {
            bitReverseSamples(buffer);
            doFft();
            fillSpectrum();
            return;
        }
    }
    vector<float>real;
    vector<float>imag;
    template <class T>
    void fillVector(vector<T> &vec , unsigned int total, T val){
        for (int i = 0; i<total; i++) {
            T temp (0);
            vec.push_back(temp);
        }
    }
    vector<float>spectrum;
    vector<int> rvrs;
    vector<float> sinlookup;
    vector<float> coslookup;
protected:
    void allocateArrays() {
        fillVector(spectrum, int(timeSize * 0.5f) + 1, 0.0f);//spectrum.reserve(int(timeSize * 0.5f) + 1);
        fillVector(real, timeSize, 0.0f);//real.reserve(timeSize);
        fillVector(imag, timeSize, 0.0f);//imag.reserve(timeSize);
    }
    void fillSpectrum() {
        for(int i = 0; i < spectrum.size(); i++)
            spectrum[i] = (float)sqrt((real[i] * real[i]) + (imag[i] * imag[i]));
    }
    int timeSize;
 

private:
    void doFft() {
        for(int halfSize = 1; halfSize < real.size(); halfSize *= 2) {
            float phaseShiftStepR = coS(halfSize);
            float phaseShiftStepI = siN(halfSize);
            float currentPhaseShiftR = 1.0;
            float currentPhaseShiftI = 0.0;
            for(int fftStep = 0; fftStep < halfSize; fftStep++) {
                for(int i = fftStep; i < real.size(); i += 2 * halfSize) {
                    int off = i + halfSize;
                    float tr = currentPhaseShiftR * real[off] - currentPhaseShiftI * imag[off];
                    float ti = currentPhaseShiftR * imag[off] + currentPhaseShiftI * real[off];
                    real[off] = real[i] - tr;
                    imag[off] = imag[i] - ti;
                    real[i] += tr;
                    imag[i] += ti;
                }
                float tmpR = currentPhaseShiftR;
                currentPhaseShiftR = tmpR * phaseShiftStepR - currentPhaseShiftI * phaseShiftStepI;
                currentPhaseShiftI = tmpR * phaseShiftStepI + currentPhaseShiftI * phaseShiftStepR;
            }
        }
    }
    
    void buildReverseTable() {
        int N = timeSize;
        rvrs[0] = 0;
        int lmt = 1;
        //println("N: "+N+"  N/2: "+ N/ 2);
        for(int bit = N/ 2; lmt < N; bit >>= 1) {
            for(int i = 0; i < lmt; i++)
                rvrs[i + lmt] = rvrs[i] + bit;
            lmt <<= 1;
        }
    }
    
    void bitReverseSamples(vector<vector<float> > samples) {
        for(int i = 0; i < timeSize; i++) {
            real[i] = samples[rvrs[i]][0];
            imag[i] = samples[rvrs[i]][1];
        }
    }
    void bitReverseSamples(vector<float> samples) {
        for(int i = 0; i < timeSize; i++) {
   //         int r = rvrs[i];
 //           float s = samples[r];
//            real[i] = s;
            real[i] = samples[rvrs[i]];
        }
    }
    float siN(int i) {
        return sinlookup[i];
    }
    
    float coS(int i) {
        return coslookup[i];
    }
    
    void buildTrigTables() {
        int N = timeSize;
        for(int i = 0; i < N; i++) {
            //sinlookup.push_back(sin(-PI /float(i)));
            sinlookup.push_back(sin(-float(i)/PI));
            //coslookup.push_back(cos(-PI /float(i)));
            coslookup.push_back(cos(-float(i)/PI));
        }
        cout << "sinlookup, coslookup" <<endl;
        for(int i = 0; i < N; i++) {
            cout << sinlookup[i] << ", " << coslookup[i] << endl;
        }
        cout << "--------------------" << endl;
    }

    

};




