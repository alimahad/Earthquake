#ifndef Sun_h
#define Sun_h

#include "GLSLProgram.h"
#include "config.h"
#include "Texture.h"
#include "Mesh.h"
#define GLM_FORCE_RADIANS
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/constants.hpp>
#include "Sphere.h"
#include <memory>
    class Sun {
    public:
        Sun();
        
        // Draw the Earth to screen
        //void draw(GLSLProgram &shader, mat4 model1);
        void draw(basicgraphics::GLSLProgram &shader,mat4 model);
        
    protected:
		std::shared_ptr<basicgraphics::Texture> tex;
		std::shared_ptr<basicgraphics::Mesh> model1;

        // Stores the earth texture data
        //std::vector< std::shared_ptr<Texture> > textures1;
        
        //std::unique_ptr<Mesh> _mesh4;
    
        
    };



#endif

