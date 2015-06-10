//
//  Header.h
//  sparseArray
//
//  Created by Roy Macdonald on 8/9/13.
//  Copyright (c) 2013 micasa. All rights reserved.
//
#pragma once
#include "sparseComplex.h"
#include "ofMain.h"

class sparseArray{
public:
    sparseArray(){}
    ~sparseArray(){}
    void addSparse(complex c, int ind){
        sparseComplex sc(c, ind);
        sparse.push_back(sc);
    }
    vector<sparseComplex>sparse;
};