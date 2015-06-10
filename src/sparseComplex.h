//
//  sparseComplex.h
//  ofxConstantQExample
//
//  Created by Roy Macdonald on 8/9/13.
//  Copyright (c) 2013 micasa. All rights reserved.
//

#pragma once
#include "complex.h"

class sparseComplex{
public:
    sparseComplex(complex c,int ind):complex(c.r, c.i){
        index=ind;
        r=c.r;
        i=c.i;
    }
    ~sparseComplex(){}
    int index;
    float r;
    float i;
    complex complex;
};
