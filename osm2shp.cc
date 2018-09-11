#define OSMIUM_WITH_PBF_INPUT
#define OSMIUM_WITH_XML_INPUT


#include <osmium.hpp>
#include <osmium/input.hpp>

#include "osm/handler.hpp"

int process_file(const char *file, const char *dir)
{
    int iret = 0;
    try {
        Osmium::OSMFile  infile(file);
        osm::handler handler(dir);
        Osmium::Input::read(infile, handler);
        handler.node_stats();
        handler.way_stats();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        iret = 1;
    }
    std::cout << "Done processing " << iret << std::endl;

    return iret;
}

int main(int argc, char* argv[]) 
{
    int iret = 0;
    char *infile = argv[1];
    char *outdir = argv[2];
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " planet.osm(.gz|.bz2|.pbf) base-path" << std::endl;
        return 1;
    }
    iret = process_file(infile, outdir);

   return iret;
}
