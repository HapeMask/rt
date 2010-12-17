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
    #include <map>
    using std::map;

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

    #include "light/spherelight.hpp"
    #include "light/pointlight.hpp"
    #include "light/arealight.hpp"

    #include "materials/material.hpp"
    #include "materials/texture.hpp"

    #include "datastructs/arraylist.hpp"

    #include "tracer/tracer.hpp"

    // We want to return a string
    //#define YYSTYPE std::string

    class scanner;

    inline std::string stripQuotes(char* p) {
        return std::string(p).substr(1, std::string(p).length() - 2);
    }
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
    texture2D* texval;
    map<textureSlot, texture2D*>* tmval;
    textureSlot tsval;
    microfacetBrdf* mbxval;
    microfacetDistribution* mdistval;
    accelerator* aval;
    rayTracer* tval;
}

%token SCENE SHAPE CAMERA LIGHT
%token TRIANGLE SPHERE OBJFILE
%token BVH OCTREE DEFAULT
%token WHITTED PATH BIDIR
%token MATERIAL BLINN PHONG LAMBERT BECKMANN ANISO SPECULAR SUBSTRATE PAIR EMISSIVE MICROFACET WARD
%token IMGTEX TEXTURED DIFFUSETEX
%token DIELECTRIC CONDUCTOR
%token SMOOTH FLAT
%token AREA POINT

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
%type <lval> spherelight
%type <lval> pointlight
%type <mval> material
%type <bval> bsdf
%type <bval> specular
%type <bval> specular_dielectric
%type <bval> specular_conductor
%type <bval> substrate
%type <bval> pair
%type <bxval> bxdf
%type <mdistval> blinn
%type <mdistval> aniso
%type <mdistval> beckmann
%type <mdistval> microfacetDistrib
%type <bxval> phong
%type <mbxval> microfacet
%type <bxval> lambert
%type <bxval> ward
%type <aval> accelerator
%type <tval> tracer
%type <texval> texture
%type <tmval> texture_list
%type <tsval> texture_slot

%%
scene_file :
            SCENE '<' accelerator ',' tracer '>' '{' contents '}'
            {
                scn.setAccelerator(acceleratorPtr($3));
                scn.setTracer(rayTracerPtr($5));
            }
            ;

accelerator :
            BVH { $$ = new bvh(); } |
            OCTREE { $$ = new octree(); } |
            DEFAULT { $$ = new defaultAccelerator(); }
            ;

tracer :
       WHITTED { $$ = new whittedRayTracer(scn); } |
       PATH { $$ = new pathTracer(scn); } |
       BIDIR { $$ = new bdpt(scn); }
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
      arealight { scn.addLight($1); } |
      spherelight { scn.addLight($1); }
      ;

pointlight :
           LIGHT '<' POINT '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
           { light* l = new pointLight(point3($6, $8, $10), $12, rgbColor($14, $16, $18)); $$ = l; }
           ;

arealight :
          LIGHT '<' AREA '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
          { light* l = new areaLight(point3($6, $8, $10), $12, rgbColor($14, $16, $18), vec3($20, $22, $24), vec3($26, $28, $30)); $$ = l; }

spherelight :
          LIGHT '<' SPHERE '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
          { light* l = new sphereLight(point3($6, $8, $10), $12, rgbColor($14, $16, $18), $20); $$ = l;}

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
        { triangleMesh* p = new triangleMesh(); objParser::parse(stripQuotes($6), p); $$ = p; } |
        OBJFILE '<' FLAT '>' '(' FILEPATH ')'
        { triangleMesh* tm = new triangleMesh(); objParser::parse(stripQuotes($6), tm, false); $$ = tm; }
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
    lambert { bsdf* b = new bsdf(); b->addBxdf($1); $$ = b; } |
    phong { bsdf* b = new bsdf(); b->addBxdf($1); $$ = b; } |
    specular { $$ = $1; } |
    substrate { $$ = $1; } |
    pair { $$ = $1; }
    ;

lambert :
        LAMBERT '(' FLOAT ',' FLOAT ',' FLOAT ')'
        { $$ = new lambertianBrdf(rgbColor($3, $5, $7)); } |
        LAMBERT '<' TEXTURED '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' texture')'
        { texture2DPtr p($12);
        $$ = new lambertianBrdf(rgbColor($6, $8, $10), p); }
        ;

phong :
      PHONG '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
      { $$ = new phongBrdf(rgbColor($3, $5, $7), $9); }
      ;

// NOTE the 1.f/{alpha, beta} below. This makes the input format for the brdf
// the same as others (0-BIG_NUMBER instead of 0-1).
ward :
	 WARD '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
     { $$ = new newWard(rgbColor($3, $5, $7), $9, $11); }
	 ;

blinn :
      BLINN '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
      { $$ = new blinn(rgbColor($3, $5, $7), $9); }
      ;

beckmann :
      BECKMANN '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
      { $$ = new beckmann(rgbColor($3, $5, $7), $9); }
      ;

aniso :
      ANISO '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
      { $$ = new aniso(rgbColor($3, $5, $7), $9, $11); }

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

microfacetDistrib :
           blinn { $$ = $1; } |
           aniso { $$ = $1; } |
           beckmann { $$ = $1; }
           ;

microfacet :
           MICROFACET '(' FLOAT ',' FLOAT ',' microfacetDistrib ')'
           {
               $$ = new microfacetBrdf($3, $5, $7);
           }
           ;

bxdf :
     microfacet { $$ = $1; } |
     lambert { $$ = $1; } |
     phong { $$ = $1; } |
     ward { $$ = $1; }
     ;

substrate :
          SUBSTRATE '(' FLOAT ',' FLOAT ',' FLOAT ',' microfacetDistrib ')'
          {
              bsdf* p = new bsdf();
              p->addBxdf(new substrate(rgbColor($3, $5, $7), $9->rho, $9));
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

texture_list :
            texture_slot ':' texture texture_list { (*$4)[$1] = $3; } |
            texture_slot ':' texture {
            map<textureSlot, texture2D*>* m = new map<textureSlot, texture2D*>;
            for(int i=0; i < NUM_TEXTURE_SLOTS; ++i) (*m)[i] = NULL;
            (*m)[$1] = $3;
            $$ = m;
            }
            ;

texture :
        IMGTEX '(' FILEPATH ')' { $$ = new colorTexture2D(stripQuotes($3)); }
        ;

texture_slot :
             DIFFUSETEX { $$ = DIFFUSE_COLOR; }
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
