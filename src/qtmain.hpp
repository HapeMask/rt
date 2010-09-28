scene scn;

string filename = "/Users/hape/hape-rt/rt/src/scene/test.scn";
int numThreads = 4;
int c;

while( (c = getopt(argc, args, "n:")) != -1 ){
    switch(c){
        case 'n':
            numThreads = atoi(optarg);
            break;
        case '?':
            if(optopt == 'n'){
                cerr << "-n requires an argument" << endl;
            }else{
                cerr << "Unknown option: -" << optopt << endl;
            }
            break;
        default:
            return 1;
    }
}

if(optind < argc){
    filename = string(args[optind]);
}

ifstream in(filename.c_str());
if(!in.is_open()){
    cerr << "Error opening file: " << filename << endl;
    return 1;
}

sceneloader::load(in, scn);
in.close();
scn.build();
QApplication app(argc, args);

qtOpenGLFramebuffer fb(scn);

//app.setActiveWindow(&fb);
fb.resize(scn.getCamera().width(), scn.getCamera().height());
fb.setWindowTitle("Qt/OpenGL Framebuffer");
fb.show();

/*
QHboxLayout* hbox = new QHboxLayout(&app);
hbox->add
*/

return app.exec();
