README.txt - 20161222

Build of this utility with msvc140.x64

Dependencies:

= shapelib - http://shapelib.maptools.org/ - X:\shapelib - hshaplibx.bat - installed X:/3rdParty.x64

= Boost - using Set BOOST_ROOT=C:/local/boost_1_62_0 - findPackage(Boost)

= osmium.hpp - From https://github.com/joto/osmium - clone X:\osmium - hosmium.bat

= OSM-binary - From https://github.com/scrosby/OSM-binary - hpsm-binaryx.bat - 

= Protobuf - From https://github.com/google/protobuf - hprotobufx.bat - installed X:/3rdParty.x64

And eventually, on LINK -
========================

1. external symbol __imp_XML_ParserCreate referenced in function "public: virtual void __cdecl Osmium::Input::XML<class osm::handler>::parse(void)"

Maybe need to at least add XML_STATIC, and find and link to libxml2??? That removed that...

2. database.obj : error LNK2019: unresolved external symbol sqlite3_close, sqlite3_open_v2, etc....

Hmmm, need to add find_package(SQlite3) ... Have a few samples X:\flightgear\CMakeModules\FindSqlite3.cmake, added one, and got past this

3. Protobuf library was compiled with the non-standard /MT[d] - rebuilt and installed with /MD[d], as are most other libraries.

4. Needed to find and include EXPAT

5. Needed to find and include SQLite3

6. Needed to find and include Protobuf

7. Needed to find and included osmpdb

8. Needed to build, install, and find ZLIB

*** AND IT COMPILED *** ;=))

I remember chopping some source using 'fork'! Need to provide say a pthreads substitute...

; eof
