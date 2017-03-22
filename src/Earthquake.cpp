#include <sstream>
#include "Earthquake.h"
using namespace std;


namespace render {
	shared_ptr<basicgraphics::Mesh> quake_model = NULL;
	void init() {
		if (quake_model == NULL) {
			vector<shared_ptr<basicgraphics::Texture>> textures;
			quake_model.reset(basicgraphics::Sphere::generate(30,60,textures));
		}
	}
	void draw(basicgraphics::GLSLProgram &shader, mat4 model, Earthquake & quake) {
		vec4 pos(0, 0, -1,1);
		pos = glm::rotate(mat4(1), glm::radians((float)quake.getLatitude()), vec3(0, 1, 0))
			* glm::rotate(mat4(1), glm::radians((float)quake.getLongitude()), vec3(1, 0, 0))
			* pos;
		model = glm::translate(model, vec3(pos));
		model = glm::scale(model, vec3(0.25 * exp(quake.getMagnitude())/exp(9.f)));
		shader.setUniform("model_mat", model);
		quake_model->draw(shader);
	}
}
void Earthquake::draw(basicgraphics::GLSLProgram & shader,mat4 model)
{
	render::draw(shader, model, *this);
}


Earthquake::Earthquake() {
	render::init();
}

Earthquake::Earthquake(std::string s) {
	Earthquake();
  line = s;
}

Date Earthquake::getDate() {
  int year = parseInt(line.substr(12,4));
  int month = parseInt(line.substr(17,2));
  int day = parseInt(line.substr(20,2));
  int hour = parseInt(line.substr(24,2));
  int minute = parseInt(line.substr(27,2));
  double second = parseFloat(line.substr(30,5));
  
  return Date(month, day, year, hour, minute, second);
}

double Earthquake::getLongitude() {
  return parseFloat(line.substr(44,8));
  //return parseFloat(line.substr(37,7));
}

double Earthquake::getLatitude() {
  //return parseFloat(line.substr(44,8));
  return parseFloat(line.substr(37,7));
}

double Earthquake::getMagnitude() {
  return parseFloat(line.substr(66,4));
}



double Earthquake::parseFloat(std::string s) {
  stringstream ss(s);
  double f;
  ss >> f;
  return f;
}

int Earthquake::parseInt(std::string s) {
  stringstream ss(s);
  int i;
  ss >> i;
  return i;
}