{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "addon.cpp", "audio_reader.cpp" ],
      "include_dirs": [
          "C:/Users/martin/Documents/GitHub/KeyFinderAPI/include/keyfinder/",
          "C:/Users/martin/Documents/GitHub/KeyFinderAPI/lib/libsndfile/include/"
      ],
      "libraries": [
    "-lsndfile",
    "C:/Users/martin/Documents/GitHub/KeyFinderAPI/lib/sndfile.lib"
],  
      "cflags": [ "-std=c++11", "-frtti" ],
      "cflags!": [ "-f-exceptions" ],
      "cflags_cc!": [ "-f-exceptions" ],
      "ExceptionHandling": 1,
      "defines": ["BUILDING_NODE_EXTENSION"]
    }
  ]  
}