#ifndef OSM2SHP_OSM_HANDLER_HPP
#define OSM2SHP_OSM_HANDLER_HPP

#include "point_database.hpp"
#include "layer.hpp"

#include <map>
#include <vector>
#include <iostream>

#include <osmium/osm/way.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/handler.hpp>

// for nodes
#define uo_show_saved_nodes     0x00000001
#define uo_show_skipped_nodes   0x00000002
#define uo_show_noname_nodes    0x00000004

// for ways
#define uo_show_saved_ways      0x00000008
#define uo_show_skipped_ways    0x00000010
#define uo_show_skipped_keys    0x00000020
#define uo_show_skipped_tags    0x00000040

#define all_usr_opts (uo_show_skipped_ways | uo_show_skipped_keys | uo_show_skipped_tags | \
    uo_show_skipped_nodes | uo_show_saved_nodes | uo_show_saved_ways | uo_show_noname_nodes )

typedef struct tagSHOWOPTS {
    size_t bit;
    const char *desc;
}SHOWOPTS, *PSHOWOPTS;

extern std::string get_opts_stg(size_t flag);
extern std::string get_opts_help();



namespace osm {

class shape_file;

class handler: public Osmium::Handler::Base {
        typedef std::map<std::string, shape_file*> shape_map;
        typedef std::map<std::string, std::string> tag_map;

public:

        handler(const std::string& base, uint64_t opts);
        ~handler();

        void node(const shared_ptr<Osmium::OSM::Node const>& node);
        void way(const shared_ptr<Osmium::OSM::Way>& way);
        void node_stats();
        void way_stats();


private:

        void add_shape(const std::string& name, int type);
        void add_layer(const std::string& name, const std::string& type, const std::string& subtype);
        bool is_area(const shared_ptr<Osmium::OSM::Way>& way);

        uint64_t             options_;
        point_database       tmp_nodes_;
        int64_t              processed_nodes_;
        int64_t              processed_ways_;
        int64_t              exported_nodes_;
        int64_t              nodes_no_id_, nodes_no_name_, nodes_skipped_;
        int64_t              exported_ways_;
        int64_t              way_skipped_, ways_skipped_;
        std::vector<layer>   layers_;
        std::string          base_path_;
        shape_map            shapes_;
};

}

#endif
