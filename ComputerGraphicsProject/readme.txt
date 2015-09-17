For the project to run properly, ensure that :
a. Eigen library is extracted and kept parallel to the .xcodeproject file. 
b. The textures and models folders are inside the CGProject folder which is parallel to the .xcodeproject file.
c. The custom working directory has been set to the position of the main folder (containing .xcodeproject file, the CGPRoject, the eigen folder and the project report).  This can be done by using Edit Scheme. 

The quadratic deformation has been implemented based on the algorithm discussed in paper. 
The object simulation shows unrealistic results. The user menu has been disabled for quadratic deformations. This can be enabled by uncommenting in makeMenu method in main.cpp file.