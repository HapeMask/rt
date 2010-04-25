#ifndef __RT_PARSER__
#define __RT_PARSER__

#include "scene.hpp"
#include "geometry/primitive.hpp"
#include "geometry/shape.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

class ParseException : public runtime_error {
	public:
		ParseException(const string& s) : runtime_error("ParseException"), expectedToken(s) {}
		string expectedToken;

		~ParseException() throw() {}
};

class sceneParser {

	public:
		sceneParser();
		void parse(scene& s, istream& in);

	private:
		string text;
		string currentToken;

		float curFloat();
		void match(const regex& token);
		bool is(const regex& token);

		void scn(scene& s);
		vector<shapePtr> shapeList();
		shapePtr shp();
		vector<primitivePtr> primitiveList();
		primitivePtr prim();
		materialPtr mat();
		brdfPtr bd();
		lightPtr li();
		cameraPtr cam();
};

static const regex WHITESPACE("\\s+");
static const regex LPAREN("\\(");
static const regex RPAREN("\\)");
static const regex LBRACE("\\{");
static const regex RBRACE("\\}");
static const regex SCENE("scene");
static const regex CAMERA("camera");
static const regex BRDF("lambert|specular|phong|microfacet");
static const regex LAMBERT("lambert");
static const regex SPECULAR("specular");
static const regex PHONG("phong");
static const regex MICROFACET("microfacet");
static const regex ACCELTYPE("default|grid|kdtree|bvh|bih");
static const regex PRIMITIVE("triangle|plane|sphere");
static const regex TRIANGLE("triangle");
static const regex LIGHTTYPE("point|spot|area");
static const regex SPHERE("sphere");
static const regex PLANE("plane");
static const regex SHAPE("shape");
static const regex LIGHT("light");
static const regex SPECTYPE("conductor|dielectric");
static const regex MATERIAL("material");
static const regex FLOAT("-{0,1}[0-9]+(\\.[0-9]*){0,1}");
static const regex SEMICOLON(";");
static const regex LANGLE("<");
static const regex RANGLE(">");
static const regex COMMA(",");
#endif
