README.md - 20161226
=========

osm2shp is a converter which converts from OpenStreetMap dumps to shapefiles. It can convert large files because
it uses a sqlite3 database for storage of the temporary nodes. The layer configuration is currently hard-coded.
Thanks to boost::iostreams osm2shp can read bz2 and gz streams directly.

Building
========

See some dependency and build notes in [build.x64/BUILDME.md](build.x64/BUILDME.md), but now this project is built using the [cmake](https://cmake.org/download/) configuraion and build file generation, so once all dependancies have been met, then it should be as simple as -

```
$ cd projects/dir
$ git clone git@github.com:geoffmcl/osm2shp.git
$ cd osm2shp/build.x64
$ cmake .. [options] [-DCMAKE_INSTALL_PREFIX=/as/desired] [-G "generator"], if other than the default desired
$ make (in linux), or
$ cmake --build . --config Release (in Windows)
```

Usage
=====

    osm2shp planet.osm.bz2 shape-path

Other included tools
====================

mapgen.sh - GRASS GIS shell script which calls osm2shp first,  processes the shapefiles with GRASS
            to reduce the geometrical complexity and exports shapefiles afterwards.

License
=======

The MIT License

Copyright (c) 2010 Daniel Mendler

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Enjoy, Geoff.

; eof
