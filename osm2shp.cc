/*\
* Part of the osm2shp project - https://github.com/geoffmcl/osm2shp -
*
* osm2shp.cc
*
* Copyright (c) 2018 - Geoff R. McLane
* Licence: GNU GPL version 2
* Original MIT License
* Copyright (c) 2010 Daniel Mendler
*
\*/

/* TODO: Offer more 'options' */

#define OSMIUM_WITH_PBF_INPUT
#define OSMIUM_WITH_XML_INPUT


#include <osmium.hpp>
#include <osmium/input.hpp>

#include "osm/handler.hpp"

#define def_usr_opts (uo_show_skipped_ways | uo_show_skipped_keys | uo_show_skipped_tags | uo_show_skipped_nodes | uo_show_saved_nodes)

static const char *module = "osm2shp";
static const char *version = "0.0.9 2018.09.12";

static const char *usr_input = 0;
static const char *usr_out_dir = 0;
static uint64_t usr_options = def_usr_opts;

int process_file(const char *file, const char *dir)
{
    int iret = 0;
    try {
        Osmium::OSMFile  infile(file);
        osm::handler handler(dir, usr_options);
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

#if 0 // 0000000000000000000000000000000000000000000000000000000
int old_main(int argc, char* argv[]) 
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
#endif // 0000000000000000000000000000000000000000000000000000000
void show_version()
{
    printf("%s: version %s\n", module, version);
}
void give_help(char *name)
{
    show_version();
    printf("%s: usage: [options] usr_input.osm[.gz|bz2|pbf] out-dir\n", module);
    printf("Options:\n");
    printf(" --help  (-h or -?) = This help and exit(0)\n");
    printf(" --version          = Version, and exit(0)\n");
    // TODO: More help
    printf("\n");
    printf(" Read 'input' as osm nodes, ways, and outputs landuse shapefiles to the 'out-dir'.\n");

}

int parse_args(int argc, char **argv)
{
    int i, i2, c;
    char *arg, *sarg;
    for (i = 1; i < argc; i++) {
        arg = argv[i];
        i2 = i + 1;
        if (strcmp(arg, "--version") == 0) {
            show_version();
            return 2;
        }
        if (*arg == '-') {
            sarg = &arg[1];
            while (*sarg == '-')
                sarg++;
            c = *sarg;
            switch (c) {
            case 'h':
            case '?':
                give_help(argv[0]);
                return 2;
                break;
                // TODO: Other arguments
            default:
                printf("%s: Unknown argument '%s'. Try -? for help...\n", module, arg);
                return 1;
            }
        }
        else {
            // bear argument
            if (usr_input) {
                if (usr_out_dir) {
                    printf("%s: Already have input '%s' and '%s' out! What is this '%s'?\n", module, usr_input, usr_out_dir, arg);
                    return 1;
                }
                else {
                    // second bear argument - output dir for shp
                    usr_out_dir = strdup(arg);
                }
            }
            else {
                // first bear argument - input osm file
                usr_input = strdup(arg);
            }
        }
    }
    if (!usr_input) {
        printf("%s: No user input found in command!\n", module);
        return 1;
    }
    if (!usr_out_dir) {
        printf("%s: No user output dir found in command!\n", module);
        return 1;
    }
    return 0;
}

// main() OS entry
int main(int argc, char **argv)
{
    int iret = 0;
    iret = parse_args(argc, argv);
    if (iret) {
        if (iret == 2)
            iret = 0;
        return iret;
    }

    iret = process_file(usr_input, usr_out_dir);

    return iret;
}

/* eof - osm2shp.cc */

