scene scn;

string filename = "../src/scene/test.scn";
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

sdlFramebuffer f(scn, 32);

#ifdef RT_MULTITHREADED
omp_set_num_threads(numThreads);
cerr << "Rendering on " << numThreads << " threads." << endl;
#endif

//cerr << scn.L(scn.getCamera().width()/2, scn.getCamera().height()/2) << endl;
//return 0;

SDL_EnableKeyRepeat(3,3);

struct timeval start, end;

SDL_Event e;
bool paused = false, dirty = false, showUpdates = false;

/*
 * Main interaction loop.
 */
while(true){
    if(paused){
        SDL_WaitEvent(&e);
    }else{
        f.render();

        dirty = true;

        // Consume any events that we aren't interested in.
        int ret;
        do{
            ret = SDL_PollEvent(&e);
        // Only interested in key events and quit events for now.
        }while(e.type != SDL_KEYDOWN && e.type != SDL_QUIT && ret != 0);
    }

    switch(e.type){
        case SDL_KEYDOWN:
            if(e.key.state == SDL_PRESSED || e.key.state == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case 'q':
                        if((e.key.keysym.mod & KMOD_CTRL) || (e.key.keysym.mod & KMOD_META)){
                            return 0;
                        }
                        break;
                    case SDLK_ESCAPE:
                        if(!paused){
                            cerr << "Pausing..." << endl;
                            paused = true;
                        }

                        break;
                    case 's':
                        if(dirty){
                            cerr << "Saving..." << endl;
                            dirty = false;
                        }
                        break;
                    case 'p':
                        if(paused){
                            cerr << "Resuming..." << endl;
                        }else{
                            cerr << "Pausing..." << endl;
                        }

                        paused = !paused;
                        break;
                    case 'u':
                        showUpdates = !showUpdates;
                        f.setShowUpdates(showUpdates);
                        break;
                    default:
                        break;
                }
            }
            break;
        case SDL_QUIT:
            return 0;
            break;
        default:
            break;
    }
}

return 0;