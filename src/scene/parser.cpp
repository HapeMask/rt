#include "parser.hpp"
#include "scene.hpp"
#include "geometry/primitive.hpp"
#include "geometry/shape.hpp"
#include "geometry/triangle.hpp"
#include "geometry/plane.hpp"
#include "geometry/sphere.hpp"

#include "camera/camera.hpp"

#include "color/color.hpp"
#include "materials/material.hpp"
#include "materials/brdf.hpp"

#include <vector>
#include <stdexcept>
#include <boost/regex.hpp>
#include <tr1/memory>

using namespace std;
using namespace tr1;
using boost::regex;
using boost::match_continuous;

sceneParser::sceneParser() : text(""), currentToken("")
{}

void sceneParser::parse(scene& s, istream& in){
	string temp("");
	while(getline(in, temp)){
		text += temp;
	}

	text = regex_replace(text, WHITESPACE, "");
	try{
		scn(s);
	}catch(const ParseException& e){
		cerr << "Syntax error. Expected token: " << e.expectedToken << endl;
		cerr << "Actual token was: " << currentToken << endl;
	}
}

void sceneParser::scn(scene& s){
	match(SCENE);
	match(LPAREN);

	string accel = currentToken;
	match(ACCELTYPE);
	match(RPAREN);
	match(LBRACE);

	s.addLight(li());
	s.setCamera(cam());

	vector<shapePtr> shapes;
	shapes = shapeList();
	match(RBRACE);
	for(size_t i=0; i<shapes.size(); i++){
		if(shapes[i]->getMaterial()->isEmissive()){
			s.addEmitter(shapes[i]);
		}else{
			s.addShape(shapes[i]);
		}
	}
}

vector<shapePtr> sceneParser::shapeList(){
	vector<shapePtr> shapes;

	match(SHAPE);
	match(LBRACE);
	shapePtr sh = shp();
	shapes.push_back(sh);
	match(RBRACE);
	if(is(SHAPE)){
		vector<shapePtr> sh2 = shapeList();
		for(size_t i=0; i<sh2.size(); i++){
			shapes.push_back(sh2[i]);
		}
	}

	return shapes;
}

shapePtr sceneParser::shp(){
	shapePtr p(new shape());
	vector<primitivePtr> prims = primitiveList();

	for(size_t i=0; i<prims.size(); i++){
		p->addPrimitive(prims[i]);
	}

	materialPtr m = mat();
	p->setMaterial(m);

	return p;
}

materialPtr sceneParser::mat(){
	match(MATERIAL);
	match(LPAREN);
	materialPtr m(new material(bd()));
	match(RPAREN);

	return m;
}

brdfPtr sceneParser::bd(){
	if(is(LAMBERT)){
		match(BRDF);
		match(LPAREN);
		float r = curFloat();
		match(FLOAT);
		match(COMMA);
		float g = curFloat();
		match(FLOAT);
		match(COMMA);
		float b = curFloat();
		match(FLOAT);
		match(RPAREN);
		return brdfPtr(new lambertianBrdf(rgbColor(r,g,b)));
	}else if(is(SPECULAR)){
		match(BRDF);
		match(LPAREN);
		match(RPAREN);
		return brdfPtr(new specularBrdf());
	}else if(is(PHONG)){
		match(BRDF);
		match(LPAREN);
		match(RPAREN);
	}else if(is(MICROFACET)){
		match(BRDF);
		match(LPAREN);
		match(RPAREN);
	}
}

cameraPtr sceneParser::cam(){
	match(CAMERA);
	match(LANGLE);
	float w, h;
	w = curFloat();
	match(FLOAT);
	match(COMMA);
	h = curFloat();
	match(FLOAT);
	match(RANGLE);
	match(LPAREN);
	float fov = curFloat();
	match(FLOAT);
	match(COMMA);
	float px = curFloat();
	match(FLOAT);
	match(COMMA);
	float py = curFloat();
	match(FLOAT);
	match(COMMA);
	float pz = curFloat();
	match(FLOAT);
	match(COMMA);
	float lx = curFloat();
	match(FLOAT);
	match(COMMA);
	float ly = curFloat();
	match(FLOAT);
	match(COMMA);
	float lz = curFloat();
	match(FLOAT);
	match(COMMA);
	float ux = curFloat();
	match(FLOAT);
	match(COMMA);
	float uy = curFloat();
	match(FLOAT);
	match(COMMA);
	float uz = curFloat();
	match(FLOAT);
	match(RPAREN);
	return cameraPtr(new camera(w, h, 0.1f, 100.f, fov, point3(px,py,pz),point3(lx,ly,lz),point3(ux,uy,uz)));
}

lightPtr sceneParser::li(){
	lightPtr lp;
	match(LIGHT);
	match(LANGLE);
	string type(currentToken);
	match(LIGHTTYPE);
	match(RANGLE);
	match(LPAREN);
	if(type == "point"){
		float px = curFloat();
		match(FLOAT);
		match(COMMA);
		float py = curFloat();
		match(FLOAT);
		match(COMMA);
		float pz = curFloat();
		match(FLOAT);
		match(COMMA);
		float r = curFloat();
		match(FLOAT);
		match(COMMA);
		float g = curFloat();
		match(FLOAT);
		match(COMMA);
		float b = curFloat();
		match(FLOAT);
		match(COMMA);
		float i = curFloat();
		match(FLOAT);
		lp.reset(new pointLight(point3(px,py,pz), rgbColor(r,g,b), i));
	}
	match(RPAREN);

	if(lp.get() != NULL){
		return lp;
	}
}

vector<primitivePtr> sceneParser::primitiveList(){
	vector<primitivePtr> prims;
	prims.push_back(prim());

	if(is(PRIMITIVE)){
		vector<primitivePtr> pr2 = primitiveList();
		for(size_t i=0; i<pr2.size(); i++){
			prims.push_back(pr2[i]);
		}
	}
	return prims;
}

primitivePtr sceneParser::prim(){
	primitivePtr p;
	if(is(TRIANGLE)){
		match(PRIMITIVE);
		match(LPAREN);

		float p1x = curFloat();
		match(FLOAT);
		match(COMMA);
		float p1y = curFloat();
		match(FLOAT);
		match(COMMA);
		float p1z = curFloat();
		match(FLOAT);
		match(COMMA);
		float p2x = curFloat();
		match(FLOAT);
		match(COMMA);
		float p2y = curFloat();
		match(FLOAT);
		match(COMMA);
		float p2z = curFloat();
		match(FLOAT);
		match(COMMA);
		float p3x = curFloat();
		match(FLOAT);
		match(COMMA);
		float p3y = curFloat();
		match(FLOAT);
		match(COMMA);
		float p3z = curFloat();
		match(FLOAT);
		match(RPAREN);

		p = primitivePtr(new triangle(point3(p1x,p1y,p1z), point3(p2x,p2y,p2z),point3(p3x,p3y,p3z)));
	}else if(is(SPHERE)){
		match(PRIMITIVE);
		match(LPAREN);

		float px = curFloat();
		match(FLOAT);
		match(COMMA);
		float py = curFloat();
		match(FLOAT);
		match(COMMA);
		float pz = curFloat();
		match(FLOAT);
		match(COMMA);
		float r = curFloat();
		match(FLOAT);
		match(RPAREN);

		p = primitivePtr(new sphere(point3(px, py, pz), r));
	}else if(is(PLANE)){
		match(PRIMITIVE);
		match(LPAREN);

		float px = curFloat();
		match(FLOAT);
		match(COMMA);
		float py = curFloat();
		match(FLOAT);
		match(COMMA);
		float pz = curFloat();
		match(FLOAT);
		match(COMMA);
		float d = curFloat();
		match(FLOAT);
		match(RPAREN);

		p = primitivePtr(new plane(vec3(px, py, pz), d));
	}
	return p;
}

float sceneParser::curFloat(){
	return strtof(currentToken.c_str(), NULL);
}

bool sceneParser::is(const regex& token){
	return regex_match(currentToken, token);
}

void sceneParser::match(const regex& token){
	if(regex_search(text, token, match_continuous)){
		text = regex_replace(text, token, "", format_first_only);

		match_results<const char*> m;
		const char* textC = text.c_str();

		if(regex_search(textC, m, FLOAT, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, PRIMITIVE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, BRDF, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, MATERIAL, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, ACCELTYPE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, CAMERA, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, SCENE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, LIGHTTYPE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, SHAPE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, LIGHT, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, LPAREN, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, RPAREN, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, LBRACE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, RBRACE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, SEMICOLON, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, COMMA, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, LANGLE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, RANGLE, match_continuous)){
			currentToken = m.str();
		}

		return;
	}

	throw ParseException(token.str());
}
