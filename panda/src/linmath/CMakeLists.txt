building_dll(CORE)
set(sources linmath_composite1.cxx linmath_composite2.cxx)

set(scan_headers
  aa_luse.h compose_matrix.h config_linmath.h coordinateSystem.h dbl2fltnames.h
  dblnames.h deg_2_rad.h flt2dblnames.h fltnames.h lcast_to.h lmatrix.h
  lorientation.h lpoint2.h lpoint3.h lpoint4.h lquaternion.h lrotation.h luse.h
  lvec2_ops.h lvec3_ops.h lvec4_ops.h lvecBase2.h lvecBase3.h lvecBase4.h
  lvector2.h lvector3.h lvector4.h mathNumbers.h vector_Colorf.h
  vector_LPoint2f.h vector_LVecBase3f.h vector_Normalf.h vector_TexCoordf.h
  vector_Vertexf.h
)

set(install_headers
  aa_luse.h compose_matrix.h compose_matrix_src.I compose_matrix_src.h
  config_linmath.h coordinateSystem.h dbl2fltnames.h dblnames.h deg_2_rad.h
  flt2dblnames.h fltnames.h lcast_to.h lcast_to_src.I lcast_to_src.h lmat_ops.h
  lmat_ops_src.I lmat_ops_src.h lmatrix.h lmatrix3_src.I lmatrix3_src.h
  lmatrix4_src.I lmatrix4_src.h lorientation.h lorientation_src.I
  lorientation_src.h lpoint2.h lpoint2_src.I lpoint2_src.h lpoint3.h
  lpoint3_src.I lpoint3_src.h lpoint4.h lpoint4_src.I lpoint4_src.h
  lquaternion.h lquaternion_src.I lquaternion_src.h lrotation.h lrotation_src.I
  lrotation_src.h luse.I luse.h lvec2_ops.h lvec2_ops_src.I lvec2_ops_src.h
  lvec3_ops.h lvec3_ops_src.I lvec3_ops_src.h lvec4_ops.h lvec4_ops_src.I
  lvec4_ops_src.h lvecBase2.h lvecBase2_src.I lvecBase2_src.h lvecBase3.h
  lvecBase3_src.I lvecBase3_src.h lvecBase4.h lvecBase4_src.I lvecBase4_src.h
  lvector2.h lvector2_src.I lvector2_src.h lvector3.h lvector3_src.I
  lvector3_src.h lvector4.h lvector4_src.I lvector4_src.h mathNumbers.h
  mathNumbers.I vector_Colorf.h vector_LPoint2f.h vector_LVecBase3f.h
  vector_Normalf.h vector_TexCoordf.h vector_Vertexf.h
)

add_library(p3linmath ${sources})
interrogate(p3linmath core ${scan_headers} ${sources})

install_headers(${install_headers})

