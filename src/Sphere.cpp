//
//  Sphere.cpp
//  
//
//  Created by Bret Jackson on 2/3/17.
//
//

#include "Sphere.h"

namespace basicgraphics {


	Sphere::Sphere(const glm::vec3 &position, const float radius, const glm::vec4 &color) : _position(position), _radius(radius), _color(color)
	{
        _model = getModelInstance();
	}

	Sphere::~Sphere()
	{

	}
	Mesh* Sphere::generate(int slices, int stacks,std::vector<std::shared_ptr<Texture>> &textures, bool textured, float uCoord, float vCoord) {

		std::vector<Mesh::Vertex> cpuVertexArray;
		std::vector<int> cpuIndexArray;
		Mesh::Vertex vert;
		int p = 0;

		float Slices = slices;
		float Stacks = stacks;
		float Radius = 1.0f;

		for (int currStack = 0; currStack <= Stacks; ++currStack) {

			for (int currSlice = 0; currSlice <= Slices; ++currSlice) {

				float V = currStack / Stacks;
				float phi = V * glm::pi <float>();

				float U = currSlice / Slices;
				//used to set texture
				float U1 = (currSlice + 1) / Slices;

				float theta = 2 * U * (glm::pi <float>());

				//vertices for top of the currStack
				//geometric formulas taken from: mathinsight(DOT)org/spherical_coordinates
				float z = Radius * sinf(phi) * cosf(theta);
				float x = Radius * sinf(phi) * sinf(theta);
				float y = Radius * cosf(phi);

				vert.position = vec3(x, y, z);
				vert.normal = vec3(x, y, z);
                if(textured == 1){
				vert.texCoord0 = glm::vec2(U + (U1 - U), V);
                } else {
                    vert.texCoord0 = glm::vec2(uCoord, vCoord);
                }
				cpuVertexArray.push_back(vert);
				cpuIndexArray.push_back(p);
				p++;

				//vertices for bottom of the currStack
				float V1 = (currStack + 1) / Stacks;
				float phi2 = V1 * glm::pi <float>();

				float z2 = Radius * sinf(phi2) * cosf(theta);
				float x2 = Radius * sinf(phi2) * sinf(theta);
				float y2 = Radius * cosf(phi2);

				vert.position = vec3(x2, y2, z2);
				vert.normal = vec3(x2, y2, z2);
                if(textured == 1){
				vert.texCoord0 = glm::vec2(U1, V1);
                } else {
                    vert.texCoord0 = glm::vec2(uCoord, vCoord);
                }
				cpuVertexArray.push_back(vert);
				cpuIndexArray.push_back(p);
				p++;

			}

		}

		const int numVertices = cpuVertexArray.size();
		const int cpuVertexByteSize = sizeof(Mesh::Vertex) * numVertices;
		const int cpuIndexByteSize = sizeof(int) * cpuIndexArray.size();
        
        
		return new Mesh(textures, GL_TRIANGLE_STRIP, GL_STATIC_DRAW, cpuVertexByteSize, cpuIndexByteSize, 0, cpuVertexArray, cpuIndexArray.size(), cpuIndexByteSize, &cpuIndexArray[0]);

	}
    
    std::shared_ptr<Model> Sphere::getModelInstance(){
        static std::shared_ptr<Model> model(new Model("sphere.obj", 1.0, glm::vec4(1.0)));
        return model;
    }

	void Sphere::draw(GLSLProgram &shader, const glm::mat4 &modelMatrix) {

		glm::mat4 translate = glm::translate(glm::mat4(1.0), _position);
        glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(_radius));
		glm::mat4 model = modelMatrix * translate * scale;
		shader.setUniform("model_mat", model);
        _model->setMaterialColor(_color);
		_model->draw(shader);
		shader.setUniform("model_mat", modelMatrix);
	}

}
