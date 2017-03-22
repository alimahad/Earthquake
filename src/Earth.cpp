#include "Earth.h"

namespace basicgraphics{

using namespace std;
using namespace glm;

Earth::Earth() {

    // Note: TEXTURE_PATH is set in config.h
    shared_ptr<Texture> tex = Texture::create2DTextureFromFile(TEXTURE_PATH);
    tex->setTexParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex->setTexParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);
    tex->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    tex->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //textures.push_back(tex);
	//setupGeometry();
	std::vector<std::shared_ptr<Texture>> textures;
	textures.push_back(tex);
	_mesh3.reset(Sphere::generate(30, 60, textures,1,0.5,0.5));
}

void Earth::setupGeometry() {
    // TODO: Write your own code here to tesselate and create texture coordinates for the Earth
    // and then save the data to a mesh (i.e. VBO). The tesselation
    // should be based on the STACKS and SLICES parameters.
    
}

glm::vec3 Earth::getPosition(double latitude, double longitude) {
    
    // TODO: Given a latitude and longitude as input, return the corresponding 3D x,y,z position
    // on your Earth geometry
    
    return vec3(1.0); // Dummy return so that it compiles. Replace me with the correct position.
}


void Earth::draw(GLSLProgram &shader) {
	// TODO: Draw your mesh.

    _mesh3->draw(shader);
}
}
