#include "Sun.h"
#include <sstream>
#include "Texture.h"

using namespace std;
using namespace glm;


namespace render {
    
    shared_ptr<basicgraphics::Mesh> model1 = NULL;
    
    void init1() {
        if(model1 == NULL){
        //shared_ptr<basicgraphics::Texture> tex = basicgraphics::Texture::create2DTextureFromFile("/Users/Mahad/Downloads/Earthquake/data/sun.jpg");
        std::vector<std::shared_ptr<basicgraphics::Texture>> textures;
//        vector<shared_ptr<basicgraphics::Texture>> texture;
//            textures.push_back(tex);

    model1.reset(basicgraphics::Sphere::generate(15,30,textures,1,0.0f,0.0f));
        
        }
    }
    void draw(basicgraphics::GLSLProgram &shader, mat4 model) {
        vec4 pos(3, -8, 9,1);

        //vec4 pos(0, 1, 0,1);
        pos = glm::rotate(mat4(1), 1.0f, vec3(0, 1, 0))
        * glm::rotate(mat4(1), 1.0f, vec3(0, 0, 1))
        * pos;
        model = glm::translate(model, vec3(pos));
        model = glm::scale(model, vec3(1.2f));
        shader.setUniform("model_mat", model);
        
        shared_ptr<basicgraphics::Texture> tex = basicgraphics::Texture::create2DTextureFromFile("/Users/Mahad/Downloads/Earthquake/data/earth.jpg");
        vector<shared_ptr<basicgraphics::Texture>> textures;
        textures.push_back(tex);
        
        model1.reset(basicgraphics::Sphere::generate(15,30,textures,1,0.0f,0.0f));
        model1->draw(shader);
    }
    
}

void Sun::draw(basicgraphics::GLSLProgram &shader,mat4 model)
{
    
    //_mesh4->draw(shader, model);
    render::draw(shader, model);
}

Sun::Sun() {
    render::init1();
}
