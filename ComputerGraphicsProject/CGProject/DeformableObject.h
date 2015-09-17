//
//  DeformableObject.h
//  CGProject
//
//  Created by Srikant Aggarwal on 31/10/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#ifndef __CGProject__DeformableObject__
#define __CGProject__DeformableObject__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <GLUT/GLUT.h>

#include "gsl_matrix.h"
#include <gsl_vector.h>
#include <gsl_linalg.h>
#include <gsl_blas.h>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "Globals.h"
#include "Particle.h"
#include "Vector.h"
#include "Triangle.h"

#include "math.h"

using namespace std;
using namespace Eigen;

class DeformableObject
{
public:
    DeformableObject();
    void initialize(string path, float y, bool rotate = false);
    void loadFile(string filename);
    Particle getCenterOfMass(void);
    Particle getOriginalCenterOfMass(void);
    void draw(void);
    void update(long h);
    void matchShape(long h);
    void postUpdate(void);
    ~DeformableObject();
private:
    vector<Particle> particles;
    vector<Vector> normals;
    vector<Triangle> triangles;
    Particle center_of_mass;
    Particle original_center_of_mass;
    MatrixXd Aqq;
    MatrixXd tildeAqq;
};

#endif /* defined(__CGProject__DeformableObject__) */
