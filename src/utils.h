#include "ofConstants.h"
//--------------------------------------------------------------
inline float getMax(vector<float> a){
    float tmax = FLT_MIN;
    if(a.size()>0){
        tmax=a[0];
        for(int i=0; i<a.size(); i++){
            if(tmax<a[i]){
                tmax=a[i];
            }
        }
    }
    return tmax;
}
//--------------------------------------------------------------
inline float getMax(vector<vector<float> > a){
    float tmax = FLT_MIN;
    if(a.size()>0){
        if (a[0].size()) {
            tmax=a[0][0];
            for(int i=0; i<a.size(); i++){
                float temp = getMax(a[i]);
                if(tmax<temp){
                    tmax=temp;
                }
            }
        }
    }
    return tmax;
}
//--------------------------------------------------------------
inline vector<float> subset( vector<float> a, int start, int length){
    vector<float>temp (a.begin() + start, a.begin() + start + length);
    return temp;
}
//--------------------------------------------------------------
inline int nextpow2(int in){
    int i=1;
    while(true){
        i++;
        if(pow((double)2,i)>=in){
            return i;
        }
    }
}
//--------------------------------------------------------------
inline float log2 (float x) {
    return (log(x) / log(2));
}
//--------------------------------------------------------------
vector<float>  hamming(int n){
    vector<float>  temp(n, 0.0f);
    for(int i = 0; i<n;i++){
        temp[i]= 0.53836-(0.46164*cos(TWO_PI*i/(n-1)));
    }
    return temp;
}
