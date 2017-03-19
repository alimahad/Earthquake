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
	_mesh3.reset(Sphere::generate(30, 60, textures));
}

void Earth::setupGeometry() {
    // TODO: Write your own code here to tesselate and create texture coordinates for the Earth
    // and then save the data to a mesh (i.e. VBO). The tesselation
    // should be based on the STACKS and SLICES parameters.
    
    std::vector<Mesh::Vertex> cpuVertexArray;
    std::vector<int> cpuIndexArray;
    std::vector<std::shared_ptr<Texture>> textures;
    std::shared_ptr<Texture> tex = Texture::create2DTextureFromFile("earthSampleImage.jpg");
    textures.push_back(tex);
    
    
    Mesh::Vertex vert;
    int p = 0;
    
    float Slices = 30;
    float Stacks = 60;
    float Radius = 1.0f;
    
    for (int currStack = 0; currStack <= Stacks; ++currStack){
        
        for (int currSlice = 0; currSlice <= Slices; ++currSlice){
            
            float V   = currStack / Stacks;
            float phi = V * glm::pi <float> ();
            
            float U = currSlice / Slices;
            //used to set texture
            float U1 = (currSlice+1) / Slices;
            
            float theta = 2 * U * (glm::pi <float>());
            
            //vertices for top of the currStack
            //geometric formulas taken from: mathinsight(DOT)org/spherical_coordinates
            float z = Radius * sinf (phi) * cosf (theta);
            float x = Radius * sinf (phi) * sinf (theta);
            float y = Radius * cosf (phi);
            
            vert.position = vec3(x,y,z);
            vert.normal = vec3(x,y,z);
            vert.texCoord0 = glm::vec2(U+(U1-U),V);
            cpuVertexArray.push_back(vert);
            cpuIndexArray.push_back(p);
            p++;
            
            //vertices for bottom of the currStack
            float V1   = (currStack+1) / Stacks;
            float phi2 = V1 * glm::pi <float> ();
            
            float z2 = Radius * sinf (phi2) * cosf (theta);
            float x2 = Radius * sinf (phi2) * sinf (theta);
            float y2 = Radius * cosf (phi2);
            
            vert.position = vec3(x2,y2,z2);
            vert.normal = vec3(x2,y2,z2);
            vert.texCoord0 = glm::vec2(U1,V1);
            cpuVertexArray.push_back(vert);
            cpuIndexArray.push_back(p);
            p++;
            
        }
        
    }
    
    const int numVertices = cpuVertexArray.size();
    const int cpuVertexByteSize = sizeof(Mesh::Vertex) * numVertices;
    const int cpuIndexByteSize = sizeof(int) * cpuIndexArray.size();
    _mesh3.reset(new Mesh(textures, GL_TRIANGLE_STRIP, GL_STATIC_DRAW, cpuVertexByteSize, cpuIndexByteSize, 0, cpuVertexArray, cpuIndexArray.size(), cpuIndexByteSize, &cpuIndexArray[0]));
    
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
