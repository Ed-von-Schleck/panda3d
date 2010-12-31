building_dll(CORE)
set(sources mathutil_composite1.cxx mathutil_composite2.cxx)

set(scan_headers
  boundingHexahedron.h boundingLine.h boundingBox.h boundingPlane.h
  boundingSphere.h boundingVolume.h config_mathutil.h fftCompressor.h
  finiteBoundingVolume.h frustum.h geometricBoundingVolume.h linmath_events.h
  look_at.h linmath_events.h mersenne.h omniBoundingVolume.h parabola.h
  perlinNoise.h perlinNoise2.h perlinNoise3.h plane.h pta_LMatrix4f.h
  pta_LMatrix3f.h pta_LVecBase3f.h pta_LVecBase4f.h pta_LVecBase2f.h
  randomizer.h rotate_to.h stackedPerlinNoise2.h stackedPerlinNoise3.h
  triangulator.h
)

set(install_headers
  boundingHexahedron.I boundingHexahedron.h boundingLine.I boundingLine.h
  boundingBox.I boundingBox.h boundingPlane.I boundingPlane.h boundingSphere.I
  boundingSphere.h boundingVolume.I boundingVolume.h config_mathutil.h
  fftCompressor.h pta_LMatrix4f.h pta_LMatrix3f.h pta_LVecBase3f.h
  pta_LVecBase4f.h pta_LVecBase2f.h finiteBoundingVolume.h frustum.h
  frustum_src.I frustum_src.h geometricBoundingVolume.I
  geometricBoundingVolume.h look_at.h look_at_src.I look_at_src.h
  linmath_events.h mersenne.h omniBoundingVolume.I omniBoundingVolume.h
  parabola.h parabola_src.I parabola_src.cxx parabola_src.h perlinNoise.h
  perlinNoise.I perlinNoise2.h perlinNoise2.I perlinNoise3.h perlinNoise3.I
  plane.h plane_src.I plane_src.cxx plane_src.h randomizer.h randomizer.I
  rotate_to.h rotate_to_src.cxx stackedPerlinNoise2.h stackedPerlinNoise2.I
  stackedPerlinNoise3.h stackedPerlinNoise3.I triangulator.h triangulator.I
)

add_library(p3mathutil ${sources})
interrogate(p3mathutil core ${scan_headers} ${sources})

install_headers(${install_headers})

