
#include <sstream>      // std::stringstream
#include "handler.hpp"
#include "shapefile.hpp"

#include <shapefil.h>
#include <sys/stat.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#ifdef _MSC_VER
#include <process.h>    // for _getpid
#include <iostream>
#include <direct.h> // for _mkdir
#define getpid _getpid
#define mkdir(a,b)  _mkdir(a)
#endif // _MSC_VER

#define foreach BOOST_FOREACH

static SHOWOPTS ShowOpts[] = {
    { uo_show_saved_nodes , "nodes saved" },
    { uo_show_skipped_nodes, "nodes skipped" },
    { uo_show_noname_nodes, "nodes noname" },
    { uo_show_saved_ways , "ways saved"},
    { uo_show_skipped_ways, "ways skipped"},
    { uo_show_skipped_keys, "ways skip keys"},
    { uo_show_skipped_tags, "ways skip tags"},

    /* MUST BE LAST */
    { 0, 0 }
};

std::string get_opts_stg(size_t flag)
{
    std::stringstream ss;
    PSHOWOPTS pso = ShowOpts;
    size_t cnt = 0;
    if (flag) {
        while (pso->desc) {
            if (pso->bit & flag) {
                if (cnt)
                    ss << ", ";
                ss << pso->desc;
                cnt++;
                //flag &= ~pso->bit;
                //if (!flag)
                //    break;
            }
            pso++;
        }
    }
    else {
        ss << "none(0)";
    }
    return ss.str();
}

std::string get_opts_help()
{
    std::stringstream ss;
    PSHOWOPTS pso = ShowOpts;
    while (pso->desc) {
        ss << " " << pso->desc << "=" << pso->bit << std::endl;

        pso++;
    }
    return ss.str();
}


typedef std::vector<Osmium::OSM::Tag> vTAGS;
typedef std::vector<std::string> vSTG;
typedef std::vector<int> vINT;

static vSTG vKeysSkipped;
bool Is_in_KeySkipped(std::string & key)
{
    size_t ii, max = vKeysSkipped.size();
    for (ii = 0; ii < max; ii++) {
        if (vKeysSkipped[ii] == key)
            return true;
    }
    return false;

}
void show_skipped_keys()
{
    size_t ii, max = vKeysSkipped.size();
    std::cout << "Way keys skipped " << max << std::endl;
    for (ii = 0; ii < max; ii++) {
        std::string key = vKeysSkipped[ii];
        std::cout << " " << key;
    }
    std::cout << std::endl;
}

static vTAGS vSkipped;

bool Is_in_skipped(Osmium::OSM::Tag &tag) 
{
    size_t ii, max = vSkipped.size();
    for (ii = 0; ii < max; ii++) {
        if (vSkipped[ii] == tag)
            return true;
    }
    return false;
}

static size_t max_line = 120;
void show_skipped_tags2()
{
    size_t len, ii, max = vSkipped.size();
    std::cout << "Way tags skipped " << max << ", may have multiple 'values', but only first shown..." << std::endl;
    std::stringstream ss;
    std::string s;
    for (ii = 0; ii < max; ii++) {
        Osmium::OSM::Tag tag(vSkipped[ii]);
        ss << " " << tag.key() << "=" << tag.value();
        s = ss.str();
        len = s.size();
        if (len > max_line) {
            std::cout << s << std::endl;
            ss.str(""); // clear stringstream
        }
    }
    s = ss.str();
    std::cout << s << std::endl;
}

typedef struct tagWAYTAGS {
    std::string key;
    vSTG values;
    vINT counts;
}WAYTAGS, * PWAYTAGS;

typedef std::vector<PWAYTAGS> vWAYTAGS;

vWAYTAGS vWayTags;

void add_to_waytags(std::string &k, std::string &v)
{
    PWAYTAGS pwt;
    int i;
    size_t ii, jj, max2, max = vWayTags.size();
    for (ii = 0; ii < max; ii++)
    {
        pwt = vWayTags[ii];
        if (pwt->key == k) {
            max2 = pwt->values.size();
            for (jj = 0; jj < max2; jj++) {
                if (v == pwt->values[jj]) {
                    i = pwt->counts[jj];
                    i++;
                    pwt->counts[jj] = i; // bumpt the count
                    return;
                }
            }
            pwt->values.push_back(v);
            pwt->counts.push_back(1);
            return;
        }
    }
    pwt = new WAYTAGS;
    pwt->key = k;
    pwt->values.push_back(v);
    pwt->counts.push_back(1);
    vWayTags.push_back(pwt);
}

void show_skipped_tags()
{
    PWAYTAGS pwt;
    int i, tot;
    size_t ii, jj, max2, max = vWayTags.size();
    if (!max)
        return;
    std::stringstream ss;
    std::string s;
    std::cout << "Way tags skipped " << max << ", may have multiple 'values' as shown..." << std::endl;
    for (ii = 0; ii < max; ii++)
    {
        pwt = vWayTags[ii];
        max2 = pwt->values.size();
        ss << pwt->key;
        if (max2 > 1)
            ss << " (" << max2 << ")";
        ss << " = ";
        tot = 0;
        for (jj = 0; jj < max2; jj++) {
            if (jj)
                ss << ", ";
            ss << pwt->values[jj];
            i = pwt->counts[jj];
            tot += i;
            if (i > 1) {
                ss << " (" << i << ")";
            }
        }
        if (tot > 1) {
            ss << " t=" << tot;
        }
        s = ss.str();
        std::cout << s << std::endl;

        ss.str(""); // clear
        pwt->values.clear();
        pwt->counts.clear();
        delete pwt;

    }
    vWayTags.clear();


}


namespace osm {

template<typename T, class K>
inline bool has_key(const T& map, const K& key) {
		const char *v = map.get_value_by_key(key);
		return (v ? true : false);
}

template<typename T, class K, class V>
inline bool has_key_value(const T& map, const K& key, const V& value) {
		const char *v = map.get_value_by_key(key);
		return v && !strcmp(v, value);
}

handler::handler(const std::string& base, uint64_t opts)
        : tmp_nodes_(boost::str(boost::format("tmpnodes-%1%.sqlite") % getpid())),
          processed_nodes_(0), processed_ways_(0),
          exported_nodes_(0), exported_ways_(0),
          base_path_(base), options_(opts),
    way_skipped_(0), ways_skipped_(0),
    nodes_no_id_(0), nodes_no_name_(0), nodes_skipped_(0) {

        mkdir(base.c_str(), 0755);

        add_shape("roadbig_line",     SHPT_ARC);
        add_shape("roadmedium_line",  SHPT_ARC);
        add_shape("roadsmall_line",   SHPT_ARC);
        add_shape("railway_line",     SHPT_ARC);
        add_shape("city_point",       SHPT_POINT);
        add_shape("town_point",       SHPT_POINT);
        add_shape("suburb_point",     SHPT_POINT);
        add_shape("village_point",    SHPT_POINT);
        add_shape("water_line",       SHPT_ARC);
        add_shape("water_area",       SHPT_POLYGON);
        add_shape("coast_line",       SHPT_ARC);

        add_layer("roadbig_line", "highway", "tertiary"); // Add highway=tertiary
        add_layer("roadbig_line",     "highway",  "motorway");
        add_layer("roadbig_line",     "highway",  "trunk");
        add_layer("roadmedium_line",  "highway",  "primary");

        // expand small roads
        add_layer("roadsmall_line" ,  "highway",  "secondary");
        add_layer("roadsmall_line", "highway", "residential");  // 20180925 - add residential roads
        add_layer("roadsmall_line", "highway", "service");  // 20180925 - add service roads
        add_layer("roadsmall_line", "highway", "track");  // 20180925 - add track roads
        add_layer("roadsmall_line", "highway", "unclassified");  // 20180925 - add unclassified roads

        add_layer("railway_line",     "railway",  "rail");
        add_layer("city_point",       "place",    "city");
        add_layer("town_point",       "place",    "town");
        add_layer("suburb_point",     "place",    "suburb");
        add_layer("village_point",    "place",    "village");

        add_layer("water_line",       "waterway", "river");
        add_layer("water_line",       "waterway", "canal");
        add_layer("water_line", "waterway", "stream");  // 20180925 - add streams

        add_layer("water_area",       "natural",  "water");
        add_layer("coast_line", "natural", "coastline");    // Add coastline shp

        if (opts)
        {
            std::string s = get_opts_stg(opts);
            int i = (int)opts;
            std::cout << "Show option " << i << " = " << s << std::endl;
        }
}

handler::~handler() {
        std::cout << "Total exported nodes: "   << exported_nodes_
                  << "\nTotal exported ways:  " << exported_ways_ << std::endl;

        foreach (shape_map::value_type& value, shapes_)
                delete value.second;

        vKeysSkipped.clear();
        vSkipped.clear();

}

void handler::add_shape(const std::string& name, int type) {
        shapes_[name] = new shape_file(base_path_ + "/" + name, type);
        if (type == SHPT_POINT)
                shapes_[name]->add_field("name");
}

void handler::add_layer(const std::string& name, const std::string& type, const std::string& subtype) {
        shape_file* shape = shapes_[name];
        assert(shape);
        layers_.push_back(layer(shape, type, subtype));
}

void handler::node(const shared_ptr<Osmium::OSM::Node const>& node) {
		int64_t id_ = node->id();
		double x_  = node->position().lon();
		double y_  = node->position().lat();

        if (++processed_nodes_ % 100000 == 0)
                std::cout << processed_nodes_ << " nodes processed, " << exported_nodes_ << " nodes exported" << std::endl;

        if (id_ <= 0) {
            nodes_no_id_++;
            return;
        }

        tmp_nodes_.set(id_, x_, y_);

        const char* name = node->tags().get_value_by_key("name");
        if (!name) {
            nodes_no_name_++;
            if (options_ & uo_show_noname_nodes) {
                std::cerr << "Node " << id_ <<
                    ", x=" << x_ << ", y=" << y_ << " - noname" << std::endl;
            }
            return;
        }
        bool fnd = false;
        foreach (const layer& lay, layers_) {
                if (lay.shape()->type() == SHPT_POINT ) {
                    const char *typ = lay.type().c_str();
                    const char *val = lay.subtype().c_str();
                    if (has_key_value(node->tags(), typ, val)) {
                        lay.shape()->point(x_, y_);
                        lay.shape()->add_attribute(0, name);
                        ++exported_nodes_;
                        fnd = true;
                        if (options_ & uo_show_saved_nodes) {
                            std::cerr << "Node " << id_ << 
                                ", x=" << x_ << ", y=" << y_ << 
                                ", k=" << typ << ", v=" << val << 
                                ", name " << name << " exported" << std::endl;
                        }
                        break;
                    }
                }
        }
        if (!fnd) {
            if (options_ & uo_show_skipped_nodes) {
                Osmium::OSM::TagList n_tags = node->tags();
                size_t ii, len = n_tags.size();
                Osmium::OSM::Tag *tag;
                std::cerr << "Node " << id_ <<
                    ", x=" << x_ << ", y=" << y_ <<
                    ", with " << len << " tags, name=" << name << " excluded" << std::endl;
                for (ii = 0; ii < len; ii++) {
                    tag = &n_tags[(int)ii];   // get a pointer to the OSM tag - key=val
                    const char *key = tag->key();
                    const char *val = tag->value();
                    std::cerr << "  " << key << "=" << val << std::endl;
                }
            }
            nodes_skipped_++;
        }
}

void handler::way(const shared_ptr<Osmium::OSM::Way>& way) {
        if (++processed_ways_ % 10000 == 0)
                std::cout << processed_ways_ << " ways processed, " << exported_ways_ << " ways exported" << std::endl;

        size_t wns = way->nodes().size();
        int type = is_area(way) ? SHPT_POLYGON : SHPT_ARC;
        if ((type == SHPT_POLYGON && wns < 3) || wns < 2) {
            way_skipped_++;
            return;
        }
        const char *ctyp = (type == SHPT_POLYGON) ? "poly" : "arc";

        bool added = false;

        foreach (const layer& lay, layers_) {
            const char *typ = lay.type().c_str();
            const char *val = lay.subtype().c_str();
            if (lay.shape()->type() == type) {
                if (has_key_value(way->tags(), typ, val)) {
#ifdef _MSC_VER
                    size_t s = wns; //  way->nodes().size();
                    double *x = 0;
                    double *y = 0;
                    if (s) {
                        x = (double *)malloc(s * sizeof(double));
                        if (!x) {
                            std::cerr << "MEMORY FAILED for double *x!" << std::endl;
                            way_skipped_++;
                            return;
                        }
                        y = (double *)malloc(s * sizeof(double));
                        if (!y) {
                            free(x);
                            std::cerr << "MEMORY FAILED for double *y!" << std::endl;
                            way_skipped_++;
                            return;
                        }
                    }
#else
                    double x[way->nodes().size()], y[way->nodes().size()];
#endif
                    if (tmp_nodes_.get(way->nodes(), x, y)) {
                        lay.shape()->multipoint(type, wns, x, y);
                        ++exported_ways_;
                        added = true;
                        if (options_ & uo_show_saved_ways) {
                            std::cerr << "Way " << ctyp << ", with " << wns << " nodes, kv " << 
                                typ << "=" << val << 
                                " exported" << std::endl;
                            const Osmium::OSM::WayNodeList& ids = way->nodes();
                            std::vector<Osmium::OSM::WayNode>::const_iterator ii = ids.begin();
                            size_t jj;
                            for (jj = 0; ii != ids.end() && jj < wns; ii++, jj++) {
                                size_t id = ii->ref();
                                double xx = x[jj];   // ii->lon();
                                double yy = y[jj];  // ii->lat();
                                std::cerr << " id " << id << " at " << xx << "," << yy << std::endl;
                                
                            }

                        }
                    }
                    else {
                        std::cerr << "FAILED Export way, with " << wns << " nodes!" << std::endl;
                        way_skipped_++;
                    }
#ifdef _MSC_VER
                    if (x)
                        free(x);
                    if (y)
                        free(y);
                    x = 0;
                    y = 0;
#endif
                    break;
                }
            }
        }

        if (!added) {
            if (options_ & uo_show_skipped_ways) {
                bool skip_keys = (options_ & uo_show_skipped_keys) ? true : false;
                bool skip_tags = (options_ & uo_show_skipped_tags) ? true : false;

                // Osmium::OSM::TagList
                Osmium::OSM::Tag *tag;
                Osmium::OSM::TagList &mtags = way->tags();
                size_t ii, len = mtags.size();
                //for (const_iterator it = mtags.begin(); it != mtags.end(); ++it) {
                //    if (!strcmp(it->key(), key)) {
                //        return it->value();
                //    }
                //}
                bool dnhdr = false;
                std::cerr << "Way " << ctyp << ", with " << wns << " nodes, " << len << " tags - skipped" << std::endl;
                dnhdr = true;
                for (ii = 0; ii < len; ii++) {
                    tag = &mtags[(int)ii];   // get a pointer to the OSM tag - key=val
                    const char *key = tag->key();
                    const char *val = tag->value();
                    std::string k(key);
                    std::string v(val);
                    add_to_waytags(k, v);
                    if (skip_keys && Is_in_KeySkipped(k)) {
                        // have already shown this k
                    }
                    else {
                        vKeysSkipped.push_back(k);
                        Osmium::OSM::Tag t(key, val);   // create new tag
                        if (skip_tags && Is_in_skipped(t)) {
                            // have already shown this k=v
                        }
                        else {
                            if (!dnhdr) {
                                std::cerr << "Way " << ctyp << ", with " << wns << " nodes, " << len << " tags" << std::endl;
                                dnhdr = true;
                            }
                            std::cerr << "  " << key << "=" << val << std::endl;
                            vSkipped.push_back(t);
                        }
                    }
                }
            }
            ways_skipped_++;
        }
}

bool handler::is_area(const shared_ptr<Osmium::OSM::Way>& way) {
        return has_key_value(way->tags(), "area", "yes")      ||
               has_key(way->tags(), "landuse")                ||
               has_key_value(way->tags(), "natural", "land")  ||
               has_key_value(way->tags(), "natural", "water") ||
               has_key_value(way->tags(), "natural", "woord");
}

void handler::node_stats() 
{
    std::cout << processed_nodes_ << " nodes processed, " <<
        nodes_no_id_ << " no id, " <<
        nodes_no_name_ << " no nm, " <<
        nodes_skipped_ << " skipped, " <<
        exported_nodes_ << " nodes exported" << std::endl;
}

void handler::way_stats()
{
    std::cout << processed_ways_ << " ways processed, " <<
        way_skipped_ << " ways akipped, " <<
        ways_skipped_ << " way missed, " <<
        exported_ways_ << " ways exported" << std::endl;

    if (options_ & uo_show_skipped_ways) {
        // show_skipped_keys();
        show_skipped_tags();
    }
}



}
