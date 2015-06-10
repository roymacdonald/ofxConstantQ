#include "ofxConstantQ.h"
#include "utils.h"
//--------------------------------------------------------------
void ofxConstantQ::start(){
    if(bIsSetup){
        startThread(false, false);
    }
}
//--------------------------------------------------------------
void ofxConstantQ::stop(){
    if(bIsSetup){
        stopThread();
        cout << "stoping Thread" << endl;
    }
}
//--------------------------------------------------------------
void ofxConstantQ::threadedFunction(){
    if (isThreadRunning()) {
        processData();
        stop();
    }
}
//--------------------------------------------------------------
void ofxConstantQ::setup(float minFreq, float maxFreq, int bins, int fs, float thresh){
    bIsSetup =false;
    minF=minFreq;
    maxF=maxFreq;
    bns=bins;
    isProcesado = false;
    porcentaje =0;
    sparseKernel(minFreq, maxFreq, bins, fs, thresh);
    bIsSetup = true;
}
//--------------------------------------------------------------
ofxConstantQ::~ofxConstantQ(){}
//--------------------------------------------------------------
void ofxConstantQ::drawSparse(){
    //PGraphics pg = createGraphics(width, height);
    float factor = 6*ofGetWidth()/float(fftLen);
    //println("factor x: "+factor);
    //pg.beginDraw();
    //pg.background(0);
    ofPushStyle();
    ofSetColor(255);
    
    //pg.smooth();
    float maxReal;
    float yPos=0;
    for(int c=0; c<K; c++){
        vector<float> real; //(sparKernel[c].sparse.size(), 0.0f);
        for(int i= 0; i<sparKernel[c].sparse.size(); i++){
            float r = sparKernel[c].sparse[i].r;
            real.push_back(r);
        }
        maxReal =(ofGetHeight()/float(K*2))/getMax(real);
        if(c>0){
            yPos=c*ofGetHeight()/float(K);
        }
        //        println("yPos: "+yPos+"  max: "+maxReal);
        for(int i=1; i<sparKernel[c].sparse.size(); i++){
            float x1 = factor*sparKernel[c].sparse[i-1].index;
            float y1 = (sparKernel[c].sparse[i-1].r*maxReal)+yPos;
            float x2 = factor*sparKernel[c].sparse[i].index;
            float y2 = (sparKernel[c].sparse[i].r*maxReal)+yPos;
            ofLine(x1,y1,x2,y2);
        }
        
    }
    ofPopStyle();
}
//--------------------------------------------------------------
int ofxConstantQ::sparseSize(){
    cout << "//==========================================================================" << endl;
    cout << "sparseSize()"<< endl;
    int m=0;
    for(int i = 0; i<sparKernel.size();i++){
        cout << sparKernel[i].sparse.size() << ",  ";
        for(int j=0; j<sparKernel[i].sparse.size();j++){
            if(sparKernel[i].sparse[j].index>m){
                m=sparKernel[i].sparse[j].index;
            }
        }
    }
    cout << "//==========================================================================" << endl;
    return m;
}

//--------------------------------------------------------------
void ofxConstantQ::draw(int m){
    if(isProcesado){    
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_POINTS);
        for(int i=0; i<procesado.size(); i++){
            for(int j =1; j<K;j++){
                mesh.addVertex(ofVec3f(i, j));
                mesh.addColor(ofColor(255,(procesado[i][j]/maximo)*255*m));
            }
        }
        mesh.draw();
    }
}

//--------------------------------------------------------------
void ofxConstantQ::drawBands(){
    bool  sharps [] = {false,true,false,false,true,false,true,false,false,true,false,true    };
    string notes []= {"A","","B","C","","D","","E","F","","G",""};
    float octaves = log2(maxF/minF);
    
    float inc = ofGetHeight()/(octaves*12.0f);
    int o=0;
    ofPushStyle();
    for (float i=0; i<ofGetHeight(); i+=inc,o++){
        if(sharps[o%12]){
            ofSetColor(200,15);
        }
        else{
            ofSetColor(255,15);
        }
        ofFill();
        ofRect(0,i-(inc/2),ofGetWidth(),inc);
        ofSetColor(0);
        ofNoFill();
        ofRect(0,i-(inc/2),ofGetWidth(),inc);
        ofSetColor(255,20);
        ofDrawBitmapString(notes[o%12],ofGetWidth()/2,i+7);
    }
    
    ofPopStyle();
}
//--------------------------------------------------------------
void ofxConstantQ::drawSingle(int m){
    m/=10;
    
    int mx=(int)floor(procesado.size()*ofGetMouseX()/float(ofGetWidth()));
    float fw = float(ofGetWidth())/(K-1);
    
    if(mx<ofGetWidth() && mx<procesado.size()){
        float s1=procesado[mx][0];
        float maxx=getMax(procesado[mx]);
        float s2;
        ofSetColor(255);
        ofDrawBitmapString("bin: "+ofToString(mx/fw)+" note: "+ofToString((12*(int)(mx/fw)/bns)%12),mx,ofGetMouseY());
        for(int i=1; i<K; i++){
            s2=procesado[mx][i];
            ofSetColor(255,0,0);
            ofLine(fw*(i-1),ofGetHeight()-(s1*m/maximo)*ofGetHeight(), fw*i, ofGetHeight()-(s2*m/maximo)*ofGetHeight());
            s1=s2;
        }
    }
}
//--------------------------------------------------------------
void ofxConstantQ::processData(){
    porcentaje =0;
    procesado.clear();
    if(bOverlaped){

//        procesado.assign(ofGetWidth(),t);
        for (int i = 0; i< ofGetWidth(); i++) {
            vector<float>t;// (K, 0.0f);
            for (int i = 0; i < K; i++) {
                t.push_back(0.0f);
            }
            procesado.push_back(t);
        }
        int cuantos =ofGetWidth()-1;
        int inc = floor(data.size()/float(ofGetWidth()));
        for(int i=0; i<cuantos; i++){
            vector <float> temp;
            if(data.size()-(i*inc)>=sparLen){
                temp = process(applyWindow(subset(data,i*inc,sparLen)));
            }
            else{
                temp = process(applyWindow(subset(data,i*inc,data.size()-i*inc)));
            }
            for(int j =1; j<K;j++){
                procesado[i][j]=temp[j];
            }
            if(lock()){
                porcentaje =floor(100*float(i)/cuantos);
                cout << porcentaje << endl;
                unlock();
            }
        }
    }
    else{
        int cuantos =ceil(float(data.size())/sparLen);
        
      // vector<float>t (K, 0.0f);
    //procesado.assign(cuantos,t);   
        for (int i = 0; i< cuantos; i++) {
            vector<float>t;// (K, 0.0f);
            for (int i = 0; i < K; i++) {
                t.push_back(0.0f);
            }
            procesado.push_back(t);
        }

        for(int i=0; i<cuantos; i++){
            vector<float> temp;
            if(data.size()-(i*sparLen)>=sparLen){
                temp = process(subset(data,i*sparLen,sparLen));
            }
            else{
                temp = process(subset(data,i*sparLen,data.size()-i*sparLen));
            }
            for(int j =1; j<K;j++){
                procesado[i][j]=temp[j];
            }
            if(lock()){
                porcentaje =floor(100*float(i)/cuantos);
                cout << porcentaje << endl;
                unlock();
            }
        }
    }
    maximo= getMax(procesado);
    isProcesado=true;
}
//--------------------------------------------------------------
int ofxConstantQ::getPorcentaje(){
    return porcentaje;
}
//--------------------------------------------------------------
void ofxConstantQ::processAll(vector<float> a, bool overlap){
    data = a;
    bOverlaped = overlap;
    start();
}

//--------------------------------------------------------------
vector<float> ofxConstantQ::applyWindow(vector<float> a){
    vector<float> temp;// (a.size(), 0.0f);
    for (int i=0; i<a.size(); i++) {
        temp.push_back(0.0f);
    }
    for(int i=0; i<a.size(); i++){
        temp[i]=a[i]*hammingWindow[i];
    }
    return temp;
}
//--------------------------------------------------------------
vector <float> ofxConstantQ::process(vector<float> x){// x must be a row vector  
    fft fftx(sparLen);
    if(x.size() >sparLen){
        x.resize(sparLen);
    }
    else if(x.size()<sparLen){
        int largozeros = sparLen-x.size();
        vector<float> zeros;// (largozeros, 0.0f);
        for(int z=0; z<largozeros;z++){
            zeros.push_back(0.0f);
        }
        x.insert(x.end(), zeros.begin(), zeros.end());
    }
    fftx.forward(x);
    vector<float> temp;// (sparKernel.size(), 0.0f);
    for(int i=0; i<sparKernel.size(); i++){
        temp.push_back(0.0f);
    }
    for(int i=0; i<sparKernel.size(); i++){
        for(int j=0; j<sparKernel[i].sparse.size(); j++){
            temp[i]+=fftx.getSpectrum()[sparKernel[i].sparse[j].index] * sparKernel[i].sparse[j].complex.magnitude();
        }
    }
    return temp;
}

//--------------------------------------------------------------
void ofxConstantQ::printSpar(){
    int mx=ofGetMouseX();
    if(mx<ofGetWidth()){
        for(int i=0; i<K;i++){
            cout << "K: " << i << "  value: " << procesado[mx][i]<< endl;
        }
    }
}

//--------------------------------------------------------------
/*
vector<complex> ofxConstantQ::sparse(vector<complex> c){
    vector<complex>spar;
    for(int a=0; a<c.size(); a++){
        if(c[a].r != 0.0f && c[a].i != 0.0f){
            spar.push_back(c[a]);
        }
    }
    return spar;
}
//*/
//--------------------------------------------------------------
void ofxConstantQ::sparseKernel(float minFreq, float maxFreq, int bins, int fs, float thresh){
    
    Q= 1/(pow(2,1.0f/bins)-1);                                         //Q= 1/(2^(1/bins)-1);
    
    K= (int)ceil(bins*log2(maxFreq/minFreq));                                 //K= ceil( bins * log2(maxFreq/minFreq) );
    cout << "Q: "<< Q << endl;
    cout << "K: " << K << endl;
    
    fftLen= (int)round(pow((double)2, nextpow2(ceil((Q*fs)/minFreq))));               //fftLen= 2^nextpow2( ceil(Q*fs/minFreq) );
    
    cout << "fftLen: " << fftLen << endl;
    
    vector< vector<float> > tempKernel;// (fftLen, temp);
    for (int i = 0; i < fftLen; i++) {
        vector<float> temp;// (2, 0.0f);
        temp.push_back(0.0f);
        temp.push_back(0.0f);
        tempKernel.push_back(temp);
        tempKernel[i][0]=0;//real
        tempKernel[i][1]=0;//img
    }
    vector<complex> specKernel;// (fftLen, complex());   //tempKernel= zeros(fftLen, 1);
    for (int i = 0; i < fftLen; i++) {
        complex c;
        specKernel.push_back(c);
    }
    
    
    sparKernel.resize(K);
    //---------------------
    fft fft1(fftLen);
    
    float yy=0;
    
    for(int k=K;k>=1;k--){            //for k= K:-1:1;
        int len= (int)ceil(Q*fs/(minFreq*pow(2,((k-1)/(float)bins))));                     //len= ceil( Q * fs / (minFreq*2^((k-1)/bins)) );
        vector<float> hamm = hamming(len);                                          // tempKernel(1:len)= hamming(len)/len .* exp(2*pi*i*Q*(0:len-1)'/len)
        vector<float>  tmp;//(len,0.0f);
        for(int i = 0; i<len; i++){
            tmp.push_back(0.0f);
        }
        float u = 2*PI*Q/len;
        for(int n=0;n<len;n++){
            float hl =hamm[n]/len;
            float un=u*n;
            tempKernel[n][0] = hl * cos(un);//real
            tempKernel[n][1] = hl * sin(un);//img
            tmp[n] = tempKernel[n][0];
        }
        //---------------------
        fft1.forward(tempKernel);                                              //specKernel= fft(tempKernel);
        
        for(int c=0; c<specKernel.size(); c++){
            specKernel[c]=complex(fft1.real[c],fft1.imag[c]);
        }                                                                      //---------------------
        sparKernel[k-1] = sparseArray(); 
        for(int z=0; z<specKernel.size(); z++){                                //specKernel(find(abs(specKernel)<=thresh))= 0;
            if(specKernel[z].magnitude()>=thresh){
                specKernel[z].r /= fftLen;
                specKernel[z].i *= -fftLen;
                complex c(specKernel[z].r,specKernel[z].i);
                sparKernel[k-1].addSparse(c,z);
            }
        }                                                                     //---------------------
        //cout << k << endl;
    }
    
    
    //---------------------
    sparLen =  (int)round(pow((double)2,nextpow2(sparseSize())))*2;
    
    hammingWindow=hamming(sparLen);
    
    cout << "spalen: " + ofToString(sparLen) << endl;
    cout << "sparse Kernel length: "+ ofToString(sparKernel.size()) << endl;
    cout << "sparseSize(): "+ ofToString(sparseSize()) << endl;
    cout << "K: "+ ofToString(K) << endl;
    cout << "Octaves: "+ ofToString(log2(maxFreq/minFreq)) << endl;
}

