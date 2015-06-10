//
//  complex.h
//  ofxConstantQExample
//
//  Created by Roy Macdonald on 8/9/13.
//  Copyright (c) 2013 micasa. All rights reserved.
//

#pragma once

#include "complex.h"

//REEMPLAZAR POR Vec2f
class complex{
    public:
    float r, i;
    complex():r(0.0f), i(0.0f){}
    complex(float _r, float _i){
        r=_r;
        i=_i;
    }
    void zero(){
        r=0;
        i=0;
    }
    float magnitude(){
        return (float)sqrt(r*r + i*i);
    }
};

inline static complex mlt(complex a, complex c){ //(a + bi)(c + di) = (ac − bd) + (bc + ad)i
    return complex(a.r*c.r - a.i*c.i, a.i*c.r + a.r*c.i);
}

