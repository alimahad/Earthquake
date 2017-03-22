#include "App.h"
#include "config.h"
#include "EarthquakeDatabase.h"
#include <iostream>
#include <sstream>
#include <fstream>

namespace basicgraphics{
    
#define FONTSTASH_IMPLEMENTATION
#include "fontstash.h"
#define GLFONTSTASH_IMPLEMENTATION
#include "glfontstash.h"
    
const int PLAYBACK_WINDOW = 365 * 24 * 60 * 60; // Year in seconds

using namespace std;
    using namespace glm;

App::App(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight) : BaseApp(argc, argv, windowName, windowWidth, windowHeight) {

    lastTime = glfwGetTime();
    glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
    
    
    eye_world = glm::vec3(-2, 1, 2);
    mouseDown = false;
    
    // Check that the config.h paths exist
    std::ifstream infile(TEXTURE_PATH);
    if (!infile.good()) {
        std::cout<< "ERROR: Unable to load the TEXTURE_PATH: \n\t"<< TEXTURE_PATH<<std::endl;
        std::cout << "\t Check that the path exists and modify config.h if needed"<<std::endl;
        assert(false);
    }
    else {
        infile.close();
    }
    std::ifstream infile2(DATA_PATH);
    if (!infile2.good()) {
        std::cout<< "ERROR: Unable to load the DATA_PATH: \n\t"<< DATA_PATH<<std::endl;
        std::cout << "\t Check that the path exists and modify config.h if needed"<<std::endl;
        assert(false);
    }
    else {
        infile2.close();
    }
    
    
    earth.reset(new Earth());
    
    eqd = EarthquakeDatabase(DATA_PATH);
    playbackScale = 86400;
	Date test = eqd.getByIndex(eqd.getMinIndex()).getDate();
	double secs = eqd.getByIndex(eqd.getMinIndex()).getDate().asSeconds();
	currentTime = eqd.getByIndex(eqd.getMinIndex()).getDate().asSeconds();
    initializeText();
    
    rotation = mat4(1.0);
    playing = true;
    
    
    
    
        //MAKING THE CYLINGER TUBE
        std::vector<Mesh::Vertex> cpuVertexArray2;
        std::vector<int> cpuIndexArray2;
        std::vector<std::shared_ptr<Texture>> textures2;
        std::shared_ptr<Texture> tex2 = Texture::create2DTextureFromFile("sun.jpg");
        textures2.push_back(tex2);
    
        int p = 0;
    
    
        Mesh::Vertex vert2;
    
    float Slices = 30;
    float Stacks = 60;
    float Radius = 4.0;
    //float Radius2 = 1.0;


    for (int i = 0; i <= Stacks; ++i){
        
        
        
        for (int j = 0; j <= Slices; ++j){
            
            float V   = i / Stacks;
            float phi = V * glm::pi <float> ();
            
            float U = j / Slices;
            float U1 = (j+1) / Slices;

            float theta = 2 * U * (glm::pi <float>());


            //mathinsight(DOT)org/spherical_coordinates
            float z = Radius * sinf (phi) * cosf (theta);
            float x = Radius *sinf (phi) * sinf (theta);
            float y = Radius * cosf (phi);
            
            vert2.position = vec3(x,y,z);
            vert2.normal = vec3(0,0,0);
            vert2.texCoord0 = glm::vec2(U+(U1-U)+0.6,V);
            cpuVertexArray2.push_back(vert2);
            cpuIndexArray2.push_back(p);
            
            p++;
            
            float V1   = (i+1) / Stacks;
            float phi2 = V1 * glm::pi <float> ();
            
            float z2 = Radius * sinf (phi2) * cosf (theta);
            float x2 = Radius *sinf (phi2) * sinf (theta);
            float y2 = Radius * cosf (phi2);
            
            vert2.position = vec3(x2,y2,z2);
            vert2.normal = vec3(0,0,0);
            vert2.texCoord0 = glm::vec2(U1+0.6,V1);
            cpuVertexArray2.push_back(vert2);
            cpuIndexArray2.push_back(p);
            
            p++;

            
        }
        
        
        
    }
    
        const int numVertices2 = cpuVertexArray2.size();
        const int cpuVertexByteSize2 = sizeof(Mesh::Vertex) * numVertices2;
        const int cpuIndexByteSize2 = sizeof(int) * cpuIndexArray2.size();
        _mesh3.reset(new Mesh(textures2, GL_TRIANGLE_STRIP, GL_STATIC_DRAW, cpuVertexByteSize2, cpuIndexByteSize2, 0, cpuVertexArray2, cpuIndexArray2.size(), cpuIndexByteSize2, &cpuIndexArray2[0]));


}

App::~App(){
    glfonsDelete(fs);
}
    
void App::initializeText(){
    int fontNormal = FONS_INVALID;
    fs = nullptr;
    
    fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
    if (fs == NULL) {
        assert(false);//Could not create stash
    }
    
    fontNormal = fonsAddFont(fs, "sans", "DroidSansMono.ttf");
    if (fontNormal == FONS_INVALID) {
        assert(false);// Could not add font normal.
    }
    
    unsigned int white = glfonsRGBA(255,255,255,255);
    
    fonsClearState(fs);
    fonsSetSize(fs, 40);
    fonsSetFont(fs, fontNormal);
    fonsSetColor(fs, white);
    fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
    
    _textShader.compileShader("textRendering.vert", GLSLShader::VERTEX);
    _textShader.compileShader("textRendering.frag", GLSLShader::FRAGMENT);
    _textShader.link();
}
    
void App::onEvent(shared_ptr<Event> event) {
    string name = event->getName();
    
	// Speedup or slowdown the playback
	if (name == "kbd_LEFT_down" || name == "kbd_LEFT_repeat") {
		playbackScale /= 1.3;
	}
	if (name == "kbd_RIGHT_down" || name == "kbd_RIGHT_repeat") {
		playbackScale *= 1.3;
	}

	// Dolly the camera closer or farther away from the earth
	if (name == "kbd_UP_down"|| name == "kbd_UP_repeat") {
		vec3 newCamPos = eye_world + vec3(0,0,-0.01);
		if (newCamPos.z > 1.2) {
			eye_world = newCamPos;
		}
	}
	else if (name == "kbd_DOWN_down" || name == "kbd_DOWN_repeat") {
		vec3 newCamPos = eye_world + vec3(0,0,0.01);
        eye_world = newCamPos;
	}

	// Rotate the earth when the user clicks and drags the mouse
	else if (name == "mouse_btn_left_down") {
        mouseDown = true;
        clickPos = event->get2DData();
		lastPos = clickPos;
    }
    else if (name == "mouse_btn_left_up") {
        mouseDown = false;
    }
    else if (name == "mouse_pointer"){
        // TODO: Update the "rotation" matrix based on how the user has dragged the mouse
        // Note: the mouse movement since the last frame is stored in dxy.
        if (mouseDown){
            vec2 dxy = vec2(event->get2DData()) - lastPos;
			lastPos = event->get2DData();
			vec2 axis = clickPos - vec2(_windowWidth / 2.f, _windowHeight / 2.f);
			rotation = glm::rotate(mat4(1), dxy.x/_windowWidth, vec3(0, 1.f, 0.f))*rotation;
			rotation = glm::rotate(mat4(1), dxy.y/_windowHeight, vec3(1, 0.f, 0.f))*rotation;
		}
	}
}




void App::onSimulation(double rdt) {
    // Advance the current time and loop back to the start if time is past the last earthquake
	currentTime += playbackScale * rdt;
	if (currentTime > eqd.getByIndex(eqd.getMaxIndex()).getDate().asSeconds()) {
		currentTime = eqd.getByIndex(eqd.getMinIndex()).getDate().asSeconds();
	}
	if (currentTime < eqd.getByIndex(eqd.getMinIndex()).getDate().asSeconds()) {
		currentTime = eqd.getByIndex(eqd.getMaxIndex()).getDate().asSeconds();
	}
}



void App::onRenderGraphics() {
    double curTime = glfwGetTime();
    onSimulation(curTime - lastTime);
    lastTime = curTime;


    glm::mat4 view = glm::lookAt(eye_world, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    
    // Setup the projection matrix so that things are rendered in perspective
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.1f, 500.0f);
    
    // Set a rotation matrix to apply when drawing the earth and earthquakes
    glm::mat4 model = rotation;
    
    // Update shader variables
    _shader.setUniform("view_mat", view);
    _shader.setUniform("projection_mat", projection);
    _shader.setUniform("model_mat", model);
    _shader.setUniform("eye_world", eye_world);

	
	
	//draw the night sky
	_mesh3->draw(_shader);



	// Draw the earth 
	earth->draw(_shader);

	Box zerozero(vec3(0, 0, 0), vec3(0.1, 0.1, -2), vec4(1, 1, 1,1));
	zerozero.draw(_shader, model);
	//Line zerozero(vec3(0,0,0),vec3(2,0,0),)


	// Draw earthquakes
	int start = eqd.getIndexByDate(Date(currentTime - PLAYBACK_WINDOW));
	int end = eqd.getIndexByDate(Date(currentTime));
	for (int x=start; x<end; x++) {
		Earthquake e = eqd.getByIndex(x);
		// TODO: Draw earthquake e
        

		e.draw(_shader,model);
        
        
        
	}
    
    
    
    drawText();
}


void App::drawText() {
	Date d(currentTime);
	stringstream s;
	s << "Current date: " << d.getMonth()
		<< "/" << d.getDay()
		<< "/" << d.getYear()
		<< "  " << d.getHour()
		<< ":" << d.getMinute();
    const std::string text = s.str();
    
    _textShader.use();
    _textShader.setUniform("projection_mat", glm::ortho(0.f, (GLfloat)_windowWidth, (GLfloat)_windowHeight, 0.f, -1.f, 1.f));
    _textShader.setUniform("view_mat", glm::mat4(1.0));
    _textShader.setUniform("model_mat", glm::mat4(1.0));
    _textShader.setUniform("lambertian_texture", 0);
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    fonsDrawText(fs, 100, 100, text.c_str(), NULL);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    _shader.use();

}
                
}//namespace




