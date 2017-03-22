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
    
	vec4 pos(0, 0, -1, 1);
	pos = glm::rotate(mat4(1), glm::radians((float)latitude), vec3(0, 1, 0))
		* glm::rotate(mat4(1), glm::radians((float)longitude), vec3(1, 0, 0))
		* pos;
	return vec3(pos);
}


void Earth::draw(GLSLProgram &shader) {
	// TODO: Draw your mesh.

    _mesh3->draw(shader);
}
}
