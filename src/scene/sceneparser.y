%skeleton "lalr1.cc"
%defines
%locations
%define namespace "Bison"
%define parser_class_name "Parser"

%parse-param    {scanner& scan}
%parse-param    {scene& scn}
%lex-param      {scanner& scan}

%code requires {
    #include <string>
    #include "scene/scene.hpp"
    #include "scene/objparser.hpp"

    #include "geometry/primitive.hpp"
    #include "geometry/triangle.hpp"
    #include "geometry/sphere.hpp"

    #include "mathlib/vector.hpp"
    #include "mathlib/point.hpp"

    #include "acceleration/bvh.hpp"
    #include "acceleration/octree.hpp"
    #include "acceleration/defaultaccelerator.hpp"

    #include "light/light.hpp"

    #include "materials/material.hpp"

    #include "datastructs/arraylist.hpp"

    // We want to return a string
    //#define YYSTYPE std::string

    class scanner;
}

%code{
    // Prototype for the yylex function
    static int yylex(Bison::Parser::semantic_type * yylval,
                     Bison::Parser::location_type * yylloc,
                     scanner &scan);
}

%union{
    float fval;
    char* sval;
    arraylist<primitive*>* listval;
    primitive* pval;
    camera* cval;
    light* lval;
    shape* shval;
    material* mval;
    bsdf* bval;
    bxdf* bxval;
    microfacetBxdf* mbxval;
    accelerator* aval;
}

%token SCENE SHAPE CAMERA LIGHT
%token TRIANGLE SPHERE OBJFILE
%token BVH OCTREE DEFAULT
%token MATERIAL BLINN PHONG LAMBERT BECKMANN ANISO SPECULAR SUBSTRATE PAIR EMISSIVE
%token DIELECTRIC CONDUCTOR
%token SMOOTH FLAT
%token AREATYPE POINTTYPE

%token <fval> FLOAT
%token <sval> FILEPATH
%type <listval> primitive_list
%type <pval> primitive
%type <shval> objfile
%type <pval> sphere
%type <pval> triangle
%type <cval> camera
%type <lval> light
%type <lval> arealight
%type <lval> pointlight
%type <mval> material
%type <bval> bsdf
%type <bval> specular
%type <bval> specular_dielectric
%type <bval> specular_conductor
%type <bval> substrate
%type <bval> pair
%type <bxval> bxdf
%type <mbxval> blinn
%type <mbxval> aniso
%type <bxval> phong
%type <mbxval> microfacet
%type <bxval> lambert
%type <aval> accelerator

%%

scene_file :
          SCENE '<' accelerator '>' '{' contents '}'
          { scn.setAccelerator(acceleratorPtr($3)); }
          ;

accelerator :
            BVH { $$ = new bvh(); } |
            OCTREE { $$ = new octree(); } |
            DEFAULT { $$ = new defaultAccelerator(); }
            ;

contents :
        camera contents |
        light contents |
        shape contents |
        shape | camera | light
        ;

camera :
       CAMERA '<' FLOAT ',' FLOAT '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
       {
           cameraPtr cam(new camera((float)$3, (float)$5, 0.1f, 100.f, $8, point3($10, $12, $14), point3($16, $18, $20), vec3($22, $24, $26)));
           scn.setCamera(cam);
       }
       ;

light :
      pointlight { scn.addLight($1); } |
      arealight { scn.addLight($1); }
      ;

pointlight :
           LIGHT '<' POINTTYPE '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
           { light* l = new pointLight(point3($6, $8, $10), $18, rgbColor($12, $14, $16)); $$ = l; }
           ;

arealight :
          LIGHT '<' AREATYPE '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
          { light* l = new areaLight(point3($6, $8, $10), $30, rgbColor($24, $26, $28), vec3($12, $14, $16), vec3($18, $20, $22)); $$ = l; }

shape :
       SHAPE '{' primitive_list material '}'
       {
           shapePtr shp(new shape());
           arraylist<primitive*>::iterator it;
           for(it = $3->begin(); it != $3->end(); ++it){
               shp->addPrimitive(*it);
           }
           shp->setMaterial($4);
           scn.addShape(shp);
       } |
       SHAPE '{' objfile material '}' { $3->setMaterial($4); scn.addShape($3); }
       ;

objfile :
        OBJFILE '<' SMOOTH '>' '(' FILEPATH ')'
        { triangleMesh* tm = new triangleMesh(); objParser::parse(std::string($6).substr(1, std::string($6).length() - 2), tm); $$ = tm; } |
        OBJFILE '<' FLAT '>' '(' FILEPATH ')'
        { triangleMesh* tm = new triangleMesh(); objParser::parse(std::string($6).substr(1, std::string($6).length() - 2), tm, false); $$ = tm; }
        ;

primitive_list : primitive primitive_list { $2->add($1); $$ = $2; } |
               primitive { arraylist<primitive*>* l = new arraylist<primitive*>(); l->add($1); $$ = l; }
               ;

primitive :
          triangle { $$ = $1; } |
          sphere { $$ = $1; }
          ;

triangle:
        TRIANGLE '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
        { $$ = new triangle(point3($3, $5, $7), point3($9, $11, $13), point3($15, $17, $19)); }
        ;

sphere :
       SPHERE '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
       { $$ = new sphere(point3($3, $5, $7), $9); }
       ;

material :
         MATERIAL '(' bsdf ')' { bsdfPtr p($3); material* mat = new material(p); $$ = mat; } |
         MATERIAL '<' EMISSIVE '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
         { $$ = new material(rgbColor($6, $8, $10), $12); }
         ;

bsdf:
    lambert { bsdf* b = new bsdf(); b->addBxdf($1); $$ = b; }|
    phong { bsdf* b = new bsdf(); b->addBxdf($1); $$ = b; } |
    specular { $$ = $1; } |
    substrate { $$ = $1; } |
    pair { $$ = $1; }
    ;

lambert :
        LAMBERT '(' FLOAT ',' FLOAT ',' FLOAT ')'
        { $$ = new lambertianBrdf(rgbColor($3, $5, $7)); }
        ;

phong :
      PHONG '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
      { $$ = new phongBrdf(rgbColor($3, $5, $7), $9); }
      ;

blinn :
      BLINN '(' FLOAT ',' FLOAT ',' FLOAT ','  FLOAT ',' FLOAT ',' FLOAT ')'
      { $$ = new blinnMicrofacet(rgbColor($3, $5, $7), $9, $11, $13); }
      ;

aniso :
      ANISO '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
      { $$ = new asMicrofacet(rgbColor($3, $5, $7), $9, $11, $13, $15); }

specular :
         specular_dielectric { $$ = $1; }|
         specular_conductor { $$ = $1; }
         ;

specular_dielectric :
                 SPECULAR '<' DIELECTRIC '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
                 {
                     bsdf* p = new bsdf();
                     p->addBxdf(new specularBrdf($12, 0.f, DIELECTRIC, rgbColor(1.f,1.f,1.f)));
                     p->addBxdf(new specularBtdf($12, rgbColor($6, $8, $10)));
                     $$ = p;
                 }
                 ;

specular_conductor :
                   SPECULAR '<' CONDUCTOR '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
                   {
                       bsdf* p = new bsdf();
                       p->addBxdf(new specularBrdf($12, $14, CONDUCTOR, rgbColor($6, $8, $10)));
                       $$ = p;
                   }
                   ;

microfacet :
           blinn { $$ = $1; } |
           aniso { $$ = $1; }
           ;

bxdf :
     microfacet { $$ = $1; } |
     lambert { $$ = $1; } |
     phong { $$ = $1; }
     ;

substrate :
          SUBSTRATE '(' FLOAT ',' FLOAT ',' FLOAT ',' microfacet ')'
          {
              bsdf* p = new bsdf();
              p->addBxdf(new substrate(rgbColor($3, $5, $7), $9->rs(), $9));
              $$ = p;
          }
          ;

pair :
     PAIR '(' bxdf ',' bxdf ')'
     {
         bsdf* p = new bsdf();
         p->addBxdf($3);
         p->addBxdf($5);
         $$ = p;
     }
     ;
%%

void Bison::Parser::error(const Bison::Parser::location_type& loc, const std::string& msg){
    cerr << "Parse error at: " << loc << ": " << msg << endl;
}

#include "scanner.hpp"
static int yylex(Bison::Parser::semantic_type * yylval,
                 Bison::Parser::location_type * yylloc,
                 scanner &scan) {
    return scan.yylex(yylval, yylloc);
}
