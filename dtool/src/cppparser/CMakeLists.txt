if(HAVE_INTERROGATE)

set(sources cppParser_composite1.cxx cppParser_composite2.cxx cppBison.yxx)

panda_add_sources(interrogate ${sources})

endif(HAVE_INTERROGATE)
