#pragma once

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
		void match(const string& token);
		bool is(const regex& token);

		void scn(scene& s);
		vector<shapePtr> shapeList(scene& s);
		shapePtr shp(scene& s);
		vector<primitivePtr> primitiveList(scene& s);
		primitivePtr prim(scene& s);
		materialPtr mat(scene& s);
		bsdfPtr bd(scene& s);
		lightPtr li(scene& s);
		cameraPtr cam(scene& s);
};

static const regex COMMENTLINE("//[^\\n\\r]*(\\n|\\r|\\r\\n)");
static const regex WHITESPACE("\\s+");
static const regex LPAREN("\\(");
static const regex RPAREN("\\)");
static const regex LBRACE("\\{");
static const regex RBRACE("\\}");
static const regex FILEPATH("\"[a-zA-Z0-9._/-]+\"");
static const regex SCENE("scene");
static const regex CAMERA("camera");
static const regex BRDF("lambert|specular|phong|anisotropic|blinn");
static const regex LAMBERT("lambert");
static const regex BLINN("blinn");
static const regex RE_SPECULAR("specular");
static const regex PHONG("phong");
static const regex ANISO("anisotropic");
static const regex EMISSIVE("emissive");
static const regex ACCELTYPE("default|grid|kdtree|bvh|bih");
static const regex PRIMITIVE("triangle|sphere");
static const regex TRIANGLE("triangle");
static const regex LIGHTTYPE("point|spot|area");
static const regex SPHERE("sphere");
static const regex SHAPE("shape");
static const regex LIGHT("light");
static const regex SPECTYPE("conductor|dielectric");
static const regex MATERIAL("material");
static const regex OBJFILE("objfile");
static const regex TRANSLATE("translate");
static const regex SCALE("scale");
static const regex FLOAT("-{0,1}[0-9]+(\\.[0-9]*){0,1}");
static const regex LANGLE("<");
static const regex RANGLE(">");
static const regex COMMA(",");

static const unsigned int numRegexes = 32;
static const regex regexes[numRegexes] = {
regex("//[^\\n\\r]*(\\n|\\r|\\r\\n)"),
regex("\\s+"),
regex("\\("),
regex("\\)"),
regex("\\{"),
regex("\\}"),
regex("\"[a-zA-Z0-9._/-]+\""),
regex("scene"),
regex("camera"),
regex("lambert|specular|phong|anisotropic|blinn"),
regex("lambert"),
regex("blinn"),
regex("specular"),
regex("phong"),
regex("anisotropic"),
regex("emissive"),
regex("default|grid|kdtree|bvh|bih"),
regex("triangle|sphere"),
regex("triangle"),
regex("point|spot|area"),
regex("sphere"),
regex("shape"),
regex("light"),
regex("conductor|dielectric"),
regex("material"),
regex("objfile"),
regex("translate"),
regex("scale"),
regex("-{0,1}[0-9]+(\\.[0-9]*){0,1}"),
regex("<"),
regex(">"),
regex(",")
};
