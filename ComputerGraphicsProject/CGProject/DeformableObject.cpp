//
//  DeformableObject.cpp
//  CGProject
//
//  Created by Srikant Aggarwal on 31/10/14.
//  Copyright (c) 2014 Srikant Aggarwal. All rights reserved.
//

#include "DeformableObject.h"

float colors[3][3] = {{(0x00 * 1.0f)/0xFF, (0xCC * 1.0f)/0xFF, (0xFF * 1.0f)/0xFF},
    {(0xFF * 1.0f)/0xFF, (0x7F * 1.0f)/0xFF, (0x00 * 1.0f)/0xFF},
    {(0xD8 * 1.0f)/0xFF, (0xE0 * 1.0f)/0xFF, (0x22 * 1.0f)/0xFF}};

/***************************************************
 * Constructor
 **************************************************/
DeformableObject::DeformableObject()
{
}

/***************************************************
 * Method calculates the original(initial) center of mass
 **************************************************/
Particle DeformableObject::getOriginalCenterOfMass(void)
{
    int particle_count = (int)particles.size();
    Vector sum_mass_original_position = Vector(0, 0, 0);
    float sum_mass = 0;
    
    for (int i = 0; i < particle_count; ++i)
    {
        Particle particle = particles[i];
        sum_mass_original_position = sum_mass_original_position + particle.getOriginalPosition() * particle.getMass();
        sum_mass = sum_mass + particle.getMass();
    }
    
    original_center_of_mass = Particle(sum_mass_original_position/sum_mass, sum_mass);
    
    return original_center_of_mass;
}

/***************************************************
 * Method to load the obj file
 **************************************************/
void DeformableObject::loadFile(string filename)
{
    fstream file;
    file.open(filename, ios::in | ios::binary);
    if (!file.is_open())
    {
        cout << "Error: File Not Found.";
    }
    
    string s;
    float x, y, z;
    
    file >> s;
    
    while(!file.eof())
    {
        if (s == "v")
        {
            file >> x >> y >> z;
            particles.push_back(Particle(x, y, z));
        }
        else if (s == "vn")
        {
            file >> x >> y >> z;
            normals.push_back(Vector(x, y, z));
        }
        else if (s == "f")
        {
            Triangle triangle;
            for (int i = 0; i < 3; ++i)
            {
                file >> s;
                int index = (int)s.find("//");
                string s1 = s.substr( 0, index );
                string s2 = s.substr( index + 2, s.length() - index - 2 );
                int vert = atoi( s1.c_str() );
                int norm = atoi( s2.c_str() );
                
                triangle.vertices[i]  = vert - 1;
                triangle.normals[i] = norm - 1;
            }
            triangles.push_back(triangle);
        }
        file >> s;
    }
    file.close();
}

/***************************************************
 * Method initializes the object by loading obj file
 * Calculating matrixes Aqq etc.
 **************************************************/
void DeformableObject::initialize(string filename, float y_offset, bool rotate)
{
    loadFile(filename);
    int particle_count = (int)particles.size();
    
    //Initializing original position of particles and applying rotation
    for (int i = 0; i < particle_count; ++i)
    {
        float add_offset = 0.0f;
        if (rotate)
        {
            if (particles[i].getOriginalPosition().x < 0)
                add_offset = 2.0f;
            else if (particles[i].getOriginalPosition().x > 0)
                add_offset = -2.0f;
        }
        particles[i].initialize(y_offset+add_offset);
    }
    
    //Initializing original center of mass
    original_center_of_mass = getOriginalCenterOfMass();
    
    //For LINEAR mode
    if (mode == LINEAR)
    {
        //Initializing Aqq matrix
        Aqq = MatrixXd(3,3);
        MatrixXd Aqq_inverse = MatrixXd::Zero(3,3);
        MatrixXd qi(3, 1);
        MatrixXd qit(1, 3);
        MatrixXd interim3X3(3,3);
        
        for( int i = 0; i < particle_count; i++ ) {
            Particle particle = particles[i];
            
            qi(0, 0) = particles[i].getOriginalPosition().x -  original_center_of_mass.getPosition().x;
            qi(1, 0) = particles[i].getOriginalPosition().y -  original_center_of_mass.getPosition().y;
            qi(2, 0) = particles[i].getOriginalPosition().z -  original_center_of_mass.getPosition().z;
            
            qit = qi.transpose();
            
            interim3X3 = qi*qit*particle.getMass();
            Aqq_inverse = Aqq_inverse + interim3X3;
        }
        
        Aqq = Aqq_inverse.inverse();
    }
    //For QUADRATIC mode
    else if (mode == QUADRATIC)
    {
        //Initializing tildeAqq matrix
        tildeAqq = MatrixXd(9, 9);
        
        MatrixXd tildeAqq_inverse = MatrixXd::Zero(9, 9);
        MatrixXd qi(3, 1);
        MatrixXd tildeQi(9, 1);
        MatrixXd tildeQit(1, 9);
        MatrixXd interim9X9(9, 9);
        
        for( int i = 0; i < particle_count; i++ )
        {
            Particle particle = particles[i];
            
            qi(0, 0) = particles[i].getOriginalPosition().x -  original_center_of_mass.getPosition().x;
            qi(1, 0) = particles[i].getOriginalPosition().y -  original_center_of_mass.getPosition().y;
            qi(2, 0) = particles[i].getOriginalPosition().z -  original_center_of_mass.getPosition().z;
            
            double qx = qi(0, 0);
            double qy = qi(1, 0);
            double qz = qi(2, 0);
            
            tildeQi(0, 0) = qx;
            tildeQi(1, 0) = qy;
            tildeQi(2, 0) = qz;
            tildeQi(3, 0) = qx*qx;
            tildeQi(4, 0) = qy*qy;
            tildeQi(5, 0) = qz*qz;
            tildeQi(6, 0) = qx*qy;
            tildeQi(7, 0) = qy*qz;
            tildeQi(8, 0) = qz*qx;
            
            tildeQit = tildeQi.transpose();
            
            interim9X9 = tildeQi*tildeQit*particle.getMass();
            tildeAqq_inverse = tildeAqq_inverse + interim9X9;
        }
        
        //Calculating tildeAqq from using SVD
        JacobiSVD<MatrixXd> svd(tildeAqq_inverse, ComputeFullU | ComputeFullV);
        
        MatrixXd S = MatrixXd::Zero(9, 9);
        for (int i = 0; i < 9; ++i)
            S(i, i) = svd.singularValues()[i];
        
        MatrixXd SInverse = MatrixXd::Zero(9, 9);
        for (int i = 0; i < 9; ++i)
        {
            if (S(i, i) == 0)
                SInverse(i, i) = 0;
            else
                SInverse(i, i) = 1/S(i, i);
        }
        
        
        tildeAqq = svd.matrixV() * SInverse * svd.matrixU().transpose();
    }
}

/***************************************************
 * Method calculates the center of mass using 
 * temp_position of particles
 **************************************************/
Particle DeformableObject::getCenterOfMass(void)
{
    int particle_count = (int)particles.size();
    Vector sum_mass_position = Vector(0, 0, 0);
    float sum_mass = 0;
    
    for (int i = 0; i < particle_count; ++i)
    {
        Particle particle = particles[i];
        sum_mass_position = sum_mass_position + particle.getTempPosition() * particle.getMass();
        sum_mass = sum_mass + particle.getMass();
    }
    
    center_of_mass = Particle(sum_mass_position/sum_mass, sum_mass);
    
    return center_of_mass;
}

/***************************************************
 * Method to calculate the goal position for each 
 * paticle based on shape matching technique
 **************************************************/
void DeformableObject::matchShape(long h)
{
    int particle_count = (int)particles.size();
    
    for (int i = 0; i < particle_count; ++i)
    {
        Particle particle = particles[i];
        Vector temp_velocity = particle.getVelocity() + (particle.getForce()*h)/particle.getMass();

        Vector temp_position = particle.getPosition() + temp_velocity*h;
        
        particles[i].setTempPosition(temp_position);
    }
    
    center_of_mass = getCenterOfMass();
    
    if (mode == RIGID || mode == LINEAR)
    {
        //Initializing Apq matrix
        MatrixXd Apq = MatrixXd::Zero(3, 3);
        MatrixXd qi(3, 1);
        MatrixXd pi(3, 1);
        MatrixXd qit(1, 3);
        MatrixXd interim3X3(3, 3);
        
        for( int i = 0; i < particle_count; i++ ) {
            Particle particle = particles[i];
            
            qi(0, 0) = particles[i].getOriginalPosition().x -  original_center_of_mass.getPosition().x;
            qi(1, 0) = particles[i].getOriginalPosition().y -  original_center_of_mass.getPosition().y;
            qi(2, 0) = particles[i].getOriginalPosition().z -  original_center_of_mass.getPosition().z;
            
            qit = qi.transpose();
            
            pi(0, 0) = particles[i].getTempPosition().x -  center_of_mass.getPosition().x;
            pi(1, 0) = particles[i].getTempPosition().y -  center_of_mass.getPosition().y;
            pi(2, 0) = particles[i].getTempPosition().z -  center_of_mass.getPosition().z;
            
            interim3X3 = pi*qit*particle.getMass();
            Apq = Apq+interim3X3;
        }
        
        //Apq = RS and polar decomposition
        interim3X3 = Apq.transpose() * Apq;
        SelfAdjointEigenSolver<MatrixXd> es(interim3X3);
        MatrixXd SInverse = es.operatorInverseSqrt();
        MatrixXd R = Apq * SInverse;
        
        //For linear mode new R = beta*A + (1-beta)*R
        if(mode == LINEAR) {
            MatrixXd A(3, 3);
            A = Apq * Aqq;
            double detA = A.determinant();
            if (detA < 0)
                A = A / (-(pow(-detA, 1/3.0)));
            else
                A = A / (pow(detA, 1/3.0));
            
            R = beta*A + (1-beta)*R;
        }
        
        //Assinging goal position to each particle if it moves
        for (int i = 0; i < particle_count; ++i)
        {
            qi(0, 0) = particles[i].getOriginalPosition().x -  original_center_of_mass.getPosition().x;
            qi(1, 0) = particles[i].getOriginalPosition().y -  original_center_of_mass.getPosition().y;
            qi(2, 0) = particles[i].getOriginalPosition().z -  original_center_of_mass.getPosition().z;
            
            MatrixXd interim3X1 = R*qi;
            
            Vector goal_position = Vector(interim3X1(0, 0)+center_of_mass.getPosition().x,
                                          interim3X1(1, 0)+center_of_mass.getPosition().y,
                                          interim3X1(2, 0)+center_of_mass.getPosition().z);
            
            if (particles[i].canMove(goal_position, particles, i))
                particles[i].setGoalPosition(goal_position);
        }
    }
    else if (mode == QUADRATIC)
    {
        //Initializing tildeApq matrix
        MatrixXd tildeApq = MatrixXd::Zero(3, 9);
        MatrixXd qi(3, 1 );
        MatrixXd tildeQi( 9, 1 );
        MatrixXd pi( 3, 1 );
        MatrixXd tildeQit( 1, 9 );
        MatrixXd interim3X9( 3, 9 );
        
        for( int i = 0; i < particle_count; i++ )
        {
            Particle particle = particles[i];
            
            qi(0, 0) = particles[i].getOriginalPosition().x -  original_center_of_mass.getPosition().x;
            qi(1, 0) = particles[i].getOriginalPosition().y -  original_center_of_mass.getPosition().y;
            qi(2, 0) = particles[i].getOriginalPosition().z -  original_center_of_mass.getPosition().z;
            
            double qx = qi(0, 0);
            double qy = qi(1, 0);
            double qz = qi(2, 0);
            
            tildeQi(0, 0) = qx;
            tildeQi(1, 0) = qy;
            tildeQi(2, 0) = qz;
            tildeQi(3, 0) = qx*qx;
            tildeQi(4, 0) = qy*qy;
            tildeQi(5, 0) = qz*qz;
            tildeQi(6, 0) = qx*qy;
            tildeQi(7, 0) = qy*qz;
            tildeQi(8, 0) = qz*qx;
            
            tildeQit = tildeQi.transpose();
            
            pi(0, 0) = particles[i].getTempPosition().x -  center_of_mass.getPosition().x;
            pi(1, 0) = particles[i].getTempPosition().y -  center_of_mass.getPosition().y;
            pi(2, 0) = particles[i].getTempPosition().z -  center_of_mass.getPosition().z;
            
            interim3X9 = pi * tildeQit * particle.getMass();
            tildeApq = tildeApq + interim3X9;
        }
        
        MatrixXd tildeA = tildeApq * tildeAqq;
        
        //Calculating tildeR
        MatrixXd Apq = MatrixXd::Zero(3, 3);
        MatrixXd qit(1, 3);
        MatrixXd interim3X3(3, 3);
        
        for( int i = 0; i < particle_count; i++ ) {
            Particle particle = particles[i];
            
            qi(0, 0) = particles[i].getOriginalPosition().x -  original_center_of_mass.getPosition().x;
            qi(1, 0) = particles[i].getOriginalPosition().y -  original_center_of_mass.getPosition().y;
            qi(2, 0) = particles[i].getOriginalPosition().z -  original_center_of_mass.getPosition().z;
            
            qit = qi.transpose();
            
            pi(0, 0) = particles[i].getTempPosition().x -  center_of_mass.getPosition().x;
            pi(1, 0) = particles[i].getTempPosition().y -  center_of_mass.getPosition().y;
            pi(2, 0) = particles[i].getTempPosition().z -  center_of_mass.getPosition().z;
            
            interim3X3 = pi*qit*particle.getMass();
            Apq = Apq+interim3X3;
        }
        
        //Apq = R*S and using polar decomposition
        interim3X3 = Apq.transpose() * Apq;
        SelfAdjointEigenSolver<MatrixXd> es(interim3X3);
        MatrixXd SInverse = es.operatorInverseSqrt();
        MatrixXd R = Apq * SInverse;
        
        MatrixXd tildeR = MatrixXd::Zero(3, 9);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                tildeR(i, j) = R(i, j);
        
        //Volume conservation using dvision by cube root of det(A)
        //Creating a square matrix of tildeA to take square root
        MatrixXd tildeASq(9, 9);
        int c = 5;
		tildeASq = MatrixXd::Identity(9, 9);
		for(int j=0;j<3;j++) {
			for(int i=0;i<9;i++) {
				tildeASq(j, i) = tildeA(j, i);
			}
		}
        
        double tildeADet = tildeASq.determinant();
		double cbrt = pow(fabs(tildeADet), 1.0/9.0);
		cbrt = (tildeADet < 0) ? -cbrt : cbrt;
		tildeA = tildeA / (cbrt);
        
        tildeR = beta*tildeA + (1-beta)*tildeR;
        
        //Setting goal position for each particle
        for (int i = 0; i < particle_count; ++i)
        {
    
            MatrixXd interim3X1 = tildeR*tildeQi;
            
            Vector goal_position = Vector(interim3X1(0, 0)+center_of_mass.getPosition().x,
                                          interim3X1(1, 0)+center_of_mass.getPosition().y,
                                          interim3X1(2, 0)+center_of_mass.getPosition().z);
            
            if (particles[i].canMove(goal_position, particles, i))
                particles[i].setGoalPosition(goal_position);
        }
    }
}

/***************************************************
 * Method updates the position, velocity of particles
 **************************************************/
void DeformableObject::update(long h)
{
    int particle_count = (int)particles.size();
    
    for (int i = 0; i < particle_count; ++i)
    {
        particles[i].update(h, particles, i);
    }
}

/***************************************************
 * Method to render the object
 **************************************************/
void DeformableObject::draw( void )
{
    glPushMatrix();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
	glEnable( GL_CULL_FACE );
	Vector position;
	Vector a, b, normal;
    
    float mcolor[] = {colors[cube_color][0], colors[cube_color][1], colors[cube_color][2], 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mcolor);
    
	for( int i = 0; i < triangles.size(); i++ )
    {
		glBegin( GL_TRIANGLES );
        normal = normals[ triangles[i].normals[0] ];
        glNormal3f( normal.x, normal.y, normal.z );
        position = particles[ triangles[i].vertices[0] ].getPosition();
        glVertex3f( position.x, position.y, position.z );
        
        normal = normals[ triangles[i].normals[1] ];
        glNormal3f( normal.x, normal.y, normal.z );
        position = particles[ triangles[i].vertices[1] ].getPosition();
        glVertex3f( position.x, position.y, position.z );
        
        normal = normals[ triangles[i].normals[2] ];
        glNormal3f( normal.x, normal.y, normal.z );
        position = particles[ triangles[i].vertices[2] ].getPosition();
        glVertex3f( position.x, position.y, position.z );
        
        glEnd();
	}
	glDisable( GL_CULL_FACE );
    
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    
    glPopMatrix();
}

/***************************************************
 * Destructor
 **************************************************/
DeformableObject::~DeformableObject(void) {}

