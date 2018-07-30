cmake_minimum_required(VERSION 3.3)

file(GLOB vtk_dlang_files "${VTK_BINARY_DIR}/dlang/vtk/*.dlang")
file(GLOB vtk_dlang_rendering_files "${VTK_BINARY_DIR}/dlang/vtk/rendering/*.dlang")
file(GLOB vtk_dlang_rendering_awt_files "${VTK_BINARY_DIR}/dlang/vtk/rendering/awt/*.dlang")
file(GLOB vtk_dlang_sample_files "${VTK_BINARY_DIR}/dlang/vtk/sample/*.dlang")

set(all_vtk_dlang_files vtk_dlang_files vtk_dlang_rendering_files vtk_dlang_rendering_awt_files vtk_dlang_sample_files)

if(VTK_DLANG_SWT_COMPONENT)
  file(GLOB vtk_dlang_swt_files "${VTK_BINARY_DIR}/dlang/vtk/rendering/swt/*.dlang")
  list(APPEND all_vtk_dlang_files vtk_dlang_swt_files)
endif()

if(VTK_DLANG_JOGL_COMPONENT)
  file(GLOB vtk_dlang_jogl_files "${VTK_BINARY_DIR}/dlang/vtk/rendering/jogl/*.dlang")
  list(APPEND vtk_dlang_jogl_files "${VTK_BINARY_DIR}/dlang/vtk/sample/rendering/JoglConeRendering.dlang")
  list(APPEND all_vtk_dlang_files vtk_dlang_jogl_files)
endif()

set(CMAKE_CONFIGURABLE_FILE_CONTENT)
foreach( dlang_file IN LISTS ${all_vtk_dlang_files} )
  set(CMAKE_CONFIGURABLE_FILE_CONTENT "${CMAKE_CONFIGURABLE_FILE_CONTENT}\"${dlang_file}\"\n")
endforeach()

configure_file(
  "${CMAKE_ROOT}/Modules/CMakeConfigurableFile.in"
  "${RSP_FILENAME}"
  @ONLY)
