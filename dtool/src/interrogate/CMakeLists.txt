if(HAVE_INTERROGATE)

panda_add_sources(interrogate
  interrogate_composite1.cxx
  interrogate_composite2.cxx)

panda_add_sources(interrogate_module
  interrogate_module.cxx)

panda_add_sources(parse_file
  parse_file.cxx)

endif(HAVE_INTERROGATE)
