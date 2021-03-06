%skeleton "lalr1.cc"
%defines
%locations
%define parse.error verbose
%define api.namespace {Bison}
%define parser_class_name {Parser}

%parse-param    {scanner& scan}
%parse-param    {scene& scn}
%lex-param      {scanner& scan}

%code requires {
    #include <string>
    #include <map>
    #include <vector>
    using std::map;
    using std::vector;

    #include "scene/scene.hpp"
    #include "scene/objparser.hpp"

    #include "geometry/primitive.hpp"
    #include "geometry/triangle.hpp"
    #include "geometry/sphere.hpp"

    #include "mathlib/vector.hpp"

    #include "acceleration/bvh.hpp"
    #include "acceleration/defaultaccelerator.hpp"

    #include "light/spherelight.hpp"
    #include "light/pointlight.hpp"
    #include "light/arealight.hpp"

    #include "materials/material.hpp"
    #include "materials/texture.hpp"

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
    vector<primitive*>* listval;
    primitive* pval;
    camera* cval;
    light* lval;
    shape* shval;
    material* mval;
    bsdf* bval;
    bxdf* bxval;
    texture2D* texval;
    textureSlot tsval;
    microfacetBrdf* mbrval;
    microfacetBtdf* mbtval;
    microfacetDistribution* mdistval;
    accelerator* aval;
    rayTracer* tval;
}

%token SCENE SHAPE CAMERA LIGHT
%token TRIANGLE SPHERE OBJFILE
%token BVH OCTREE DEFAULT
%token WHITTED PATH BIDIR
%token MATERIAL PHONG LAMBERT SPECULAR SUBSTRATE PAIR EMISSIVE WARD TEST
%token MICROFACET BLINN BECKMANN ANISO FROSTED
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
%type <bval> testBsdf
%type <bval> specular
%type <bval> frosted
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
%type <mbrval> microfacetBrdf
%type <mbtval> microfacetBtdf
%type <bxval> lambert
%type <bxval> ward
%type <aval> accelerator
%type <tval> tracer
%type <texval> texture
/*
%type <f_tmval> f_texture_list
%type <c_tmval> c_texture_list
%type <tsval> texture_slot
*/

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
           shape* shp = new shape();
           for(auto prim : *$3){
               shp->addPrimitive(prim);
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

primitive_list : primitive primitive_list { $2->push_back($1); $$ = $2; } |
               primitive { vector<primitive*>* l = new vector<primitive*>(); l->push_back($1); $$ = l; }
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
         MATERIAL '(' bsdf ')' { material* mat = new material($3); $$ = mat; } |
         MATERIAL '<' EMISSIVE '>' '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
         { $$ = new material(rgbColor($6, $8, $10), $12); }
         ;

bsdf :
    bxdf { bsdf* b = new bsdf(); b->addBxdf($1); $$ = b; } |
    specular { $$ = $1; } |
    frosted { $$ = $1; } |
    substrate { $$ = $1; } |
    pair { $$ = $1; } |
    testBsdf { $$ = $1; }
    ;

lambert :
        LAMBERT '(' FLOAT ',' FLOAT ',' FLOAT ')'
        { $$ = new lambertianBrdf(rgbColor($3, $5, $7)); } |
        LAMBERT '<' TEXTURED '>' '(' texture ')'
        { texture2DPtr p($6); $$ = new lambertianBrdf(p); }
        ;

phong :
      PHONG '(' FLOAT ',' FLOAT ',' FLOAT ',' FLOAT ')'
      { $$ = new phongBrdf(rgbColor($3, $5, $7), $9); }
      ;

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
                     p->addBxdf( new specularBrdf($12, 0.f, DIELECTRIC, rgbColor(1.f,1.f,1.f)));
                     p->addBxdf( new specularBtdf($12, rgbColor($6, $8, $10)));
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

frosted :
        FROSTED '(' microfacetBrdf ',' microfacetBtdf ')'
        {
            $$ = new frostedGlassBsdf($3, $5);
        }
        ;

microfacetDistrib :
           blinn { $$ = $1; } |
           aniso { $$ = $1; } |
           beckmann { $$ = $1; }
           ;

microfacetBrdf :
           MICROFACET '(' FLOAT ',' FLOAT ',' microfacetDistrib ')'
           {
               $$ = new microfacetBrdf($3, $5, $7);
           }
           ;

microfacetBtdf :
           MICROFACET '(' FLOAT ',' FLOAT ',' microfacetDistrib ')'
           {
               $$ = new microfacetBtdf($3, $5, $7);
           }
           ;

bxdf :
     microfacetBrdf { $$ = $1; } |
     microfacetBtdf { $$ = $1; } |
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

testBsdf :
         TEST '('')'
         {
             $$ = new testBsdf();
         }
         ;

texture :
        IMGTEX '(' FILEPATH ')' { $$ = new colorTexture2D(stripQuotes($3)); }
        ;

/*
texture_list :
            texture_slot ':' texture texture_list { (*$4)[$1] = $3; } |
            texture_slot ':' texture {
            map<textureSlot, texture2D*>* m = new map<textureSlot, texture2D>*>;
            for(int i=0; i < NUM_TEXTURE_SLOTS; ++i) (*m)[i] = NULL;
            (*m)[$1] = $3;
            $$ = m;
            }
            ;

texture_slot :
             DIFFUSETEX { $$ = DIFFUSE_COLOR; }
             ;
*/
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
