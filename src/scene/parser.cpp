#include "parser.hpp"
#include "scene.hpp"
#include "objparser.hpp"

#include "geometry/primitive.hpp"
#include "geometry/shape.hpp"
#include "geometry/triangle.hpp"
#include "geometry/sphere.hpp"

#include "camera/camera.hpp"

#include "acceleration/defaultaccelerator.hpp"
#include "acceleration/bvh.hpp"

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
#ifndef RT_NO_EXCEPTIONS
	try{
#endif
		scn(s);
#ifndef RT_NO_EXCEPTIONS
	}catch(const ParseException& e){
		cerr << "Syntax error. Expected token: " << e.expectedToken << endl;
		cerr << "Actual token was: " << currentToken << endl;
	}
#endif
}

void sceneParser::scn(scene& s){
	match(SCENE);
	match(LPAREN);

	string accel(currentToken);
	match(ACCELTYPE);
	match(RPAREN);
	match(LBRACE);

    if(accel == "bvh"){
        s.setAccelerator(acceleratorPtr(new bvh));
    }else{
        s.setAccelerator(acceleratorPtr(new defaultAccelerator));
    }

	s.setCamera(cam());
    while(is(LIGHT)){
        s.addLight(li());
    }

	vector<shapePtr> shapes;
	shapes = shapeList();
	match(RBRACE);
	for(size_t i=0; i<shapes.size(); i++){
        s.addShape(shapes[i]);
	}
}

vector<shapePtr> sceneParser::shapeList(){
	vector<shapePtr> shapes;

	match(SHAPE);
	match(LBRACE);
	shapes.push_back(shp());
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

	p->setMaterial(mat());
	return p;
}

materialPtr sceneParser::mat(){
	match(MATERIAL);
	match(LPAREN);
    if(is(EMISSIVE)){
        match(EMISSIVE);
        match(LPAREN);
        float r = curFloat();
        match(FLOAT);
        match(COMMA);
        float g = curFloat();
        match(FLOAT);
        match(COMMA);
        float b = curFloat();
        match(FLOAT);
        match(COMMA);
        float intensity = curFloat();
        match(FLOAT);
        match(RPAREN);
        match(RPAREN);

        const materialPtr m(new material(rgbColor(r,g,b), intensity));
        return m;
    }else{
        brdfPtr br = bd();
        match(RPAREN);
        const materialPtr m(new material(br));
        return m;
    }
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

        const brdfPtr p(new lambertianBrdf(rgbColor(r,g,b)));
        return p;
	}else if(is(SPECULAR)){
		match(BRDF);
		match(LPAREN);
        string type(currentToken);
        match(SPECTYPE);
        float ior = 1.00029f;
        if(type == "dielectric"){
            match(COMMA);
            ior = curFloat();
            match(FLOAT);
        }
		match(RPAREN);
        if(type == "conductor"){
            const brdfPtr p(new specularBrdf(true, ior));
            return p;
        }else{
            const brdfPtr p(new specularBrdf(false, ior));
            return p;
        }
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

	const cameraPtr c(new camera(w, h, 0.1f, 100.f, fov, point3(px,py,pz),point3(lx,ly,lz),point3(ux,uy,uz)));
    return c;
}

lightPtr sceneParser::li(){
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
        match(RPAREN);
		const lightPtr l(new pointLight(point3(px,py,pz), rgbColor(r,g,b), i));
        return l;
	}
}

vector<primitivePtr> sceneParser::primitiveList(){
	vector<primitivePtr> prims;

    // primitiveList : primitive primitiveList
    if(is(PRIMITIVE)){
        prims.push_back(prim());
    // primitiveList : objfile primitiveList
    }else if(is(OBJFILE)){
        match(OBJFILE);
        match(LPAREN);

        // Strip quotes.
        string filename(currentToken.substr(1,currentToken.length() - 2));

        match(FILEPATH);
        match(RPAREN);
        const vector<primitivePtr> tris = objParser::parse(filename);
        for(unsigned int i=0; i<tris.size(); i++){
            prims.push_back(tris[i]);
        }
    }

    if(is(PRIMITIVE) || is(OBJFILE)){
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
	}
	return p;
}

float sceneParser::curFloat(){
	return strtof(currentToken.c_str(), NULL);
}

bool sceneParser::is(const regex& token){
	return regex_match(currentToken, token);
}

void sceneParser::match(const string& token){
    match(regex(token));
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
        }else if(regex_search(textC, m, SPECTYPE, match_continuous)){
            currentToken = m.str();
		}else if(regex_search(textC, m, BRDF, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, MATERIAL, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, ACCELTYPE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, EMISSIVE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, CAMERA, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, FILEPATH, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, OBJFILE, match_continuous)){
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
		}else if(regex_search(textC, m, COMMA, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, LANGLE, match_continuous)){
			currentToken = m.str();
		}else if(regex_search(textC, m, RANGLE, match_continuous)){
			currentToken = m.str();
		}

		return;
	}

#ifndef RT_NO_EXCEPTIONS
	throw ParseException(token.str());
#endif
}
