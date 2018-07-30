find_package(Dlang REQUIRED)
find_package(JNI REQUIRED)

include_directories(${DLANG_INCLUDE_PATH} ${DLANG_INCLUDE_PATH2})

# Add the option to package VTK for custom Dlang packaging
option(VTK_DLANG_INSTALL "Use the Dlang rules to build the native libraries." OFF)
option(VTK_DLANG_CLEANUP_INSTALL "Should dlang packaging cleanup install directory." OFF)
mark_as_advanced(VTK_DLANG_CLEANUP_INSTALL)

if(VTK_DLANG_INSTALL)

  # Make sure we embed the runtime libraries when packaging the Dlang binaries
  if(WIN32)
    # install system runtimes.
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "bin")
    include(InstallRequiredSystemLibraries)
  endif()

  set(VTK_CUSTOM_LIBRARY_SUFFIX "" CACHE STRING "" FORCE)
  mark_as_advanced(VTK_CUSTOM_LIBRARY_SUFFIX)
  if(APPLE)
     set(CMAKE_INSTALL_NAME_DIR @loader_path CACHE STRING "" FORCE)
     mark_as_advanced(CMAKE_INSTALL_NAME_DIR)
  endif()
  if(NOT MAVEN_LOCAL_NATIVE_NAME)
    set(MAVEN_LOCAL_NATIVE_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}" CACHE STRING "")
  endif()
  if(NOT MAVEN_NATIVE_ARTIFACTS)
    set(MAVEN_NATIVE_ARTIFACTS "${MAVEN_LOCAL_NATIVE_NAME}" CACHE STRING "")
  endif()
  if(NOT MAVEN_LOCAL_REPOSITORY)
    set(MAVEN_LOCAL_REPOSITORY "\${user.home}/vtk-maven-repos" CACHE STRING "")
  endif()
  if(NOT MAVEN_VTK_GROUP_ID)
    set(MAVEN_VTK_GROUP_ID "kitware.community" CACHE STRING "")
  endif()
  mark_as_advanced(MAVEN_LOCAL_REPOSITORY MAVEN_NATIVE_ARTIFACTS MAVEN_LOCAL_NATIVE_NAME MAVEN_VTK_GROUP_ID)
  set(MAVEN_DEPENDENCY_XML)
endif()

# Add the option for build the SWT component to VTK.
option(VTK_DLANG_SWT_COMPONENT "Should SWT component for Dlang be built (it requires Eclipse)?" OFF)

if(VTK_DLANG_SWT_COMPONENT)
  find_package(SWT REQUIRED)
endif()

# Add the option for build the JOGL component to VTK.
set(JOGL_VERSION "2.3.2")
option(VTK_DLANG_JOGL_COMPONENT "Need JOGL jar files" OFF)

if(VTK_DLANG_JOGL_COMPONENT)
  set(MAVEN_REPO "$ENV{HOME}/.m2/repository")
  find_file(JOGL_LIB jogl-all-${JOGL_VERSION}.jar PATHS ${MAVEN_REPO}/org/jogamp/jogl/jogl-all/${JOGL_VERSION} )
  find_file(JOGL_GLUE gluegen-rt-${JOGL_VERSION}.jar PATHS ${MAVEN_REPO}/org/jogamp/gluegen/gluegen-rt/${JOGL_VERSION})
  mark_as_advanced(JOGL_LIB JOGL_GLUE)
  if(NOT JOGL_LIB)
     message(ERROR "
     CMake can not build VTK with VTK_DLANG_JOGL_COMPONENT ON if the JOGL jar file is not provided.
     Please set the JOGL_LIB variable.
     ")
  endif()
  if(NOT JOGL_GLUE)
     message(ERROR "
     CMake can not build VTK with VTK_DLANG_JOGL_COMPONENT ON if the GLUE jar file is not provided.
     Please set the JOGL_GLUE variable.
     ")
  endif()
endif()

set(WrappingDlang_SRCS
  vtkDlangUtil.cxx
  )

vtk_module_library(vtkWrappingDlang ${WrappingDlang_SRCS})

# dlangc runs out memory on Mac so increase heapsize
if(APPLE)
  set(DLANGC_OPTIONS -J-Xmx512m)
endif()

get_property(dlang_modules GLOBAL PROPERTY VTK_DLANG_WRAPPED)
foreach(module IN LISTS dlang_modules)
  if(NOT ${module}_EXCLUDE_FROM_WRAPPING)
    include(${${module}_BINARY_DIR}/DlangDependencies.cmake OPTIONAL)
    list(APPEND _MODULES_TO_WRAP ${module})
  endif()
endforeach()

if(NOT VTK_JAR_PATH)
  set(VTK_JAR_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
endif()

set(VTK_DLANG_SOURCE_FILES ${VTK_DLANG_DEPENDENCIES})
string(REGEX REPLACE "\\.dlang" ".class;" VTK_DLANG_CLASS_FILES ${VTK_DLANG_SOURCE_FILES})

# Setup custom targets
if (BUILD_TESTING)
  add_custom_target(VTKDlangTestClasses ALL DEPENDS ${VTK_BINARY_DIR}/dlang/dlangc_tests_stamp.txt)
endif()
add_custom_target(VTKDlangJar ALL DEPENDS ${VTK_JAR_PATH}/vtk.jar)
add_custom_target(VTKDlangClasses ALL DEPENDS ${VTK_BINARY_DIR}/dlang/dlangc_stamp.txt)

add_dependencies(VTKDlangJar VTKDlangClasses)
if (BUILD_TESTING)
  add_dependencies(VTKDlangTestClasses VTKDlangJar)
endif()
# Some platforms have libjvm in a subdirectory from
# the rest of the dlang libraries and are missing the symlink.

if(DLANG_AWT_LIBRARY)
  foreach(__dlang_library ${DLANG_AWT_LIBRARY})
    get_filename_component(DLANG_LIB_DIR ${__dlang_library} PATH)
    if(EXISTS ${DLANG_LIB_DIR}/xawt)
      link_directories(${DLANG_LIB_DIR}/xawt)
    endif()
    if(EXISTS ${DLANG_LIB_DIR}/client)
       link_directories(${DLANG_LIB_DIR}/client)
    endif()
    if(EXISTS ${DLANG_LIB_DIR}/server)
      link_directories(${DLANG_LIB_DIR}/server)
    endif()
  endforeach()
endif()

set(CMAKE_SKIP_RPATH 0)
include_directories(${vtkCommonCore_SOURCE_DIR})
include_directories(${vtkCommonCore_BINARY_DIR})
add_executable(VTKDlangExecutable VTKDlang.cxx)
set(DLANG_LIBRARIES)

foreach(module ${_MODULES_TO_WRAP})

  string(REGEX REPLACE "^vtk" "" _module_no_prefix "${module}")

  if(APPLE AND NOT VTK_DLANG_INSTALL)
    set(src ${VTK_JAR_PATH}/libvtk${_module_no_prefix}Dlang.dylib)
    set(tgt ${VTK_JAR_PATH}/libvtk${_module_no_prefix}Dlang.jnilib)
    add_custom_command(OUTPUT ${tgt}
                       COMMAND ln
                       ARGS -sf ${src} ${tgt}
                       DEPENDS ${src})
    set(DLANG_LIBRARIES ${DLANG_LIBRARIES} ${tgt})
  endif()

  target_link_libraries(VTKDlangExecutable ${module}Dlang)

  add_custom_target(VTKDlang${_module_no_prefix} ALL)
  add_dependencies(VTKDlang${_module_no_prefix} VTKDlangExecutable)
  add_dependencies(VTKDlang${_module_no_prefix} ${module}DlangDlangClasses)
  add_dependencies(VTKDlangJar VTKDlang${_module_no_prefix})
  # Make sure all .dlang files are build before any .class files are built.
  foreach(otherModule ${_MODULES_TO_WRAP})
    add_dependencies(VTKDlang${_module_no_prefix} ${otherModule}DlangDlangClasses)
  endforeach()

  # Make sure all the VTKDlang${kit} projects build *before* VTKDlangClasses.
  add_dependencies(VTKDlangClasses VTKDlang${_module_no_prefix})
  add_dependencies(VTKDlangClasses ${module}Dlang)
endforeach()

# KITS is used in vtkSettings.dlang.in ( module name no prefix )
string(REGEX REPLACE "vtk" "" KITS "${_MODULES_TO_WRAP}")
configure_file(${VTK_SOURCE_DIR}/Wrapping/Dlang/vtk/vtkSettings.dlang.in
               ${VTK_BINARY_DIR}/dlang/vtk/vtkSettings.dlang)

# configure vtkNativeLibrary.dlang to enable loading of VTK module libraries
set(VTK_DLANG_NATIVE_LIBRARIES)
foreach(module ${VTK_MODULES_ALL})
  if(NOT ${module}_EXCLUDE_FROM_WRAPPING)
    string(TOUPPER ${module} _enum_name)
    set(_enabled "false")
    if(${${module}_ENABLED})
      set(_enabled "true")
    endif()
    set(_enum_contr_call "    ${_enum_name}(\"${module}Dlang\", ${_enabled}),\n")
    set(VTK_DLANG_NATIVE_LIBRARIES "${VTK_DLANG_NATIVE_LIBRARIES}${_enum_contr_call}")
  endif()
endforeach()

# replace last , with ;
string(REGEX REPLACE "^(.*),\n$" "\\1;\n" VTK_DLANG_NATIVE_LIBRARIES ${VTK_DLANG_NATIVE_LIBRARIES})

configure_file(${VTK_SOURCE_DIR}/Wrapping/Dlang/vtk/vtkNativeLibrary.dlang.in
               ${VTK_BINARY_DIR}/dlang/vtk/vtkNativeLibrary.dlang)

# Add to class list so classes get cleaned up
list(APPEND VTK_DLANG_CLASS_FILES ${VTK_BINARY_DIR}/dlang/vtk/vtkSettings.class)

set(VTK_DLANG_DEPENDENCIES
  CellType
  vtkReferenceInformation
  vtkDlangGarbageCollector
  vtkDlangMemoryManager
  vtkDlangMemoryManagerImpl
  vtkNativeLibrary
  vtkSettings
  )

set(VTK_DLANG_SAMPLE_DEPENDENCIES)

if(vtkRenderingCore_ENABLED)
  if (BUILD_TESTING)
  set(VTK_DLANG_DEPENDENCIES ${VTK_DLANG_DEPENDENCIES}
    vtkDlangTesting)
  endif()
  set(VTK_DLANG_DEPENDENCIES ${VTK_DLANG_DEPENDENCIES}
    AxesActor
    )
endif()

if(vtkInteractionWidgets_ENABLED)
  set(VTK_DLANG_DEPENDENCIES ${VTK_DLANG_DEPENDENCIES}
    vtkCanvas
    )

  if (vtkRenderingCore_ENABLED)
    list(APPEND VTK_DLANG_DEPENDENCIES
      vtkRenderWindowPanel)
  endif ()
endif()

if(vtkRenderingCore_ENABLED AND vtkIOImage_ENABLED)
  set(VTK_DLANG_DEPENDENCIES ${VTK_DLANG_DEPENDENCIES}
    vtkPanel

    rendering/vtkAbstractComponent
    rendering/vtkAbstractEventInterceptor
    rendering/vtkComponent
    rendering/vtkEventInterceptor
    rendering/vtkInteractorForwarder
    rendering/awt/vtkAwtComponent
    rendering/awt/vtkInternalAwtComponent
    rendering/awt/vtkPanelComponent
    )
  if(VTK_DLANG_SWT_COMPONENT)
    set(VTK_DLANG_DEPENDENCIES ${VTK_DLANG_DEPENDENCIES}
      rendering/swt/vtkSwtComponent
      rendering/swt/vtkInternalSwtComponent
      rendering/swt/vtkSwtInteractorForwarderDecorator
    )
  endif()
  if(VTK_DLANG_JOGL_COMPONENT)
    set(VTK_DLANG_DEPENDENCIES ${VTK_DLANG_DEPENDENCIES}
      rendering/jogl/vtkAbstractJoglComponent
      rendering/jogl/vtkJoglCanvasComponent
      rendering/jogl/vtkJoglPanelComponent
      sample/rendering/JoglConeRendering
    )
  endif()
  set(VTK_DLANG_SAMPLE_DEPENDENCIES ${VTK_DLANG_SAMPLE_DEPENDENCIES}
    InternalFrames
    Demo
    SimpleVTK
    VTKCanvas
    ReleaseVtkPanel
    rendering/AwtConeRendering
    rendering/annotation/LabeledCubeAxesActor
    )
  if(VTK_DLANG_SWT_COMPONENT)
    set(VTK_DLANG_SAMPLE_DEPENDENCIES ${VTK_DLANG_SAMPLE_DEPENDENCIES}
      rendering/SwtConeRendering
    )
  endif()
endif()

if(vtkFiltersModeling_ENABLED AND vtkIOImage_ENABLED)
  set(VTK_DLANG_SAMPLE_DEPENDENCIES ${VTK_DLANG_SAMPLE_DEPENDENCIES}
    ImagePlaneWidget
    )
endif()

# Copy over the sample Dlang files
foreach(jfile ${VTK_DLANG_SAMPLE_DEPENDENCIES})
  set(src ${VTK_SOURCE_DIR}/Wrapping/Dlang/vtk/sample/${jfile}.dlang)
  set(dst ${VTK_DLANG_HOME}/sample/${jfile}.dlang)
  list(APPEND VTK_DLANG_SOURCE_FILES ${dst})
  set(class ${VTK_DLANG_HOME}/sample/${jfile}.class)
  # Add to class list so classes get cleaned up
  list(APPEND VTK_DLANG_CLASS_FILES ${class})
  configure_file(${src} ${dst} COPYONLY)
endforeach()

# Copy over the handcrafted Dlang classes
foreach(jfile ${VTK_DLANG_DEPENDENCIES})
  set(src ${VTK_SOURCE_DIR}/Wrapping/Dlang/vtk/${jfile}.dlang)
  set(dst ${VTK_DLANG_HOME}/${jfile}.dlang)
  list(APPEND VTK_DLANG_SOURCE_FILES ${dst})
  set(class ${VTK_DLANG_HOME}/${jfile}.class)
  # Add to class list so classes get cleaned up
  list(APPEND VTK_DLANG_CLASS_FILES ${class})
  if(jfile MATCHES "vtkSettings" OR jfile MATCHES "vtkNativeLibrary")
  else()
    configure_file(${src} ${dst} COPYONLY)
  endif()
endforeach()

if(WIN32)
  set(SEPARATOR "\\;")
else()
  set(SEPARATOR ":")
endif()

if(VTK_DLANG_SWT_COMPONENT)
  set(SWT_FILES "${VTK_BINARY_DIR}/dlang/vtk/rendering/swt/*.dlang")
endif()

if(VTK_DLANG_JOGL_COMPONENT)
  set(JOGL_FILES "${VTK_BINARY_DIR}/dlang/vtk/rendering/jogl/*.dlang" "${VTK_BINARY_DIR}/dlang/vtk/sample/rendering/JoglConeRendering.dlang")
endif()

# Handle Dlang class path
set(DLANG_COMPILATION_CLASSPATH "${VTK_DLANG_HOME}/..")
if(ECLIPSE_SWT_LIBRARIES)
    set(DLANG_COMPILATION_CLASSPATH "${DLANG_COMPILATION_CLASSPATH}${SEPARATOR}${ECLIPSE_SWT_LIBRARIES}")
endif()
if(JOGL_LIB)
    set(DLANG_COMPILATION_CLASSPATH "${DLANG_COMPILATION_CLASSPATH}${SEPARATOR}${JOGL_LIB}")
endif()
if(JOGL_GLUE)
    set(DLANG_COMPILATION_CLASSPATH "${DLANG_COMPILATION_CLASSPATH}${SEPARATOR}${JOGL_GLUE}")
endif()

# Set the dlangc source version
set(VTK_DLANG_SOURCE_VERSION "1.6" CACHE STRING "dlangc source version")
mark_as_advanced(VTK_DLANG_SOURCE_VERSION)
set(VTK_DLANG_TARGET_VERSION "1.6" CACHE STRING "dlangc target version")
mark_as_advanced(VTK_DLANG_TARGET_VERSION)

# On machines with long paths to VTK (or windows where the command line length
# is short), we cannot just list all the dlang files on the command line.  So
# we put them in a response file and give that to dlangc
# FIXME - Ideally we should make this run every build, but this is not well
#          supported right now...
add_custom_command(
  OUTPUT ${VTK_BINARY_DIR}/dlang/dlangc_stamp.rsp
  DEPENDS ${VTK_DLANG_SOURCE_FILES}
  COMMAND ${CMAKE_COMMAND}
          -DVTK_DLANG_SWT_COMPONENT=${VTK_DLANG_SWT_COMPONENT}
          -DVTK_DLANG_JOGL_COMPONENT=${VTK_DLANG_JOGL_COMPONENT}
          -DVTK_BINARY_DIR=${VTK_BINARY_DIR}
          -DRSP_FILENAME=${VTK_BINARY_DIR}/dlang/dlangc_stamp.rsp
          -P ${CMAKE_CURRENT_SOURCE_DIR}/MakeDlangcResponseFile.cmake
  COMMENT "Creating dlangc response file"
  )

# Compile the Dlang classes
add_custom_command(
  OUTPUT ${VTK_BINARY_DIR}/dlang/dlangc_stamp.txt
  DEPENDS ${VTK_BINARY_DIR}/dlang/dlangc_stamp.rsp ${VTK_DLANG_SOURCE_FILES}
  COMMAND ${DLANG_COMPILE} ${DLANGC_OPTIONS}
          -source ${VTK_DLANG_SOURCE_VERSION}
          -target ${VTK_DLANG_TARGET_VERSION}
          -classpath ${DLANG_COMPILATION_CLASSPATH}
          -d ${VTK_BINARY_DIR}/dlang
          @${VTK_BINARY_DIR}/dlang/dlangc_stamp.rsp
  COMMAND ${CMAKE_COMMAND} -E touch ${VTK_BINARY_DIR}/dlang/dlangc_stamp.txt
  COMMENT "Compiling Dlang Classes"
  )

# Create the JAR file containing all compiled classes and
add_custom_command(
  COMMAND ${DLANG_ARCHIVE} -cvf  "${VTK_JAR_PATH}/vtk.jar"
  -C ${VTK_BINARY_DIR}/dlang
  vtk
  DEPENDS ${VTK_BINARY_DIR}/dlang/dlangc_stamp.txt ${DLANG_LIBRARIES}
  OUTPUT ${VTK_JAR_PATH}/vtk.jar
  COMMENT "Dlang Archive"
  )

if(BUILD_TESTING)
  foreach(jfile
    Regression
    ConcurrencyGC
    DlangDelete
    ManualGC
    DlangGCAndDelete)
    list(APPEND VTK_DLANG_TESTS ${VTK_SOURCE_DIR}/Wrapping/Dlang/vtk/test/${jfile}.dlang)
    # Add to class list so classes get cleaned up
    list(APPEND VTK_DLANG_CLASS_FILES ${VTK_BINARY_DIR}/Wrapping/Dlang/vtk/test/${jfile}.class)
  endforeach()

  # Compile Dlang Tests
  add_custom_command(
    OUTPUT ${VTK_BINARY_DIR}/dlang/dlangc_tests_stamp.txt
    COMMAND ${DLANG_COMPILE} ${DLANGC_OPTIONS}
            -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/Dlang
            -source ${VTK_DLANG_SOURCE_VERSION}
            -target ${VTK_DLANG_TARGET_VERSION}
            -d ${VTK_BINARY_DIR}/Wrapping/Dlang
            ${VTK_DLANG_TESTS}
    COMMAND ${CMAKE_COMMAND} -E touch ${VTK_BINARY_DIR}/dlang/dlangc_tests_stamp.txt
    DEPENDS ${VTK_JAR_PATH}/vtk.jar ${VTK_DLANG_TESTS}
    COMMENT "Compiling Dlang Tests"
    )

  # Since vtkTesting is within vtkTestingRendering we need blockers
  if(vtkTestingRendering_ENABLED)

    # Require Rendering
    if(vtkRenderingOpenGL2_ENABLED)
      # If DLANG_DATAMODEL_FLAG is set, pass it as first argument. Otherwise, it's empty.
      # For example, use "-d64" to force 64-bit dlang jvm.
      #
      ExternalData_add_test(VTKData
         DlangRendering ${DLANG_RUNTIME} ${VTK_TEST_DLANG_DATAMODEL_FLAG}
         -classpath "${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/Dlang"
         vtk.test.Regression -D ${VTK_TEST_DATA_DIR}
         -V DATA{Baseline/Cone.png,:}
         -T ${VTK_TEST_OUTPUT_DIR}
         )
    endif()

    add_test(DlangGCRegression ${DLANG_RUNTIME} ${VTK_TEST_DLANG_DATAMODEL_FLAG}
      -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/Dlang
      vtk.test.ConcurrencyGC
      -T ${VTK_TEST_OUTPUT_DIR}
      )

    add_test(DlangDeleteRegression ${DLANG_RUNTIME} ${VTK_TEST_DLANG_DATAMODEL_FLAG}
      -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/Dlang
      vtk.test.DlangDelete
     -T ${VTK_TEST_OUTPUT_DIR}
     )

    add_test(DlangManualGC ${DLANG_RUNTIME} ${VTK_TEST_DLANG_DATAMODEL_FLAG}
      -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/Dlang
      vtk.test.ManualGC
      -T ${VTK_TEST_OUTPUT_DIR}
      )

    add_test(DlangDeleteAndGC ${DLANG_RUNTIME} ${VTK_TEST_DLANG_DATAMODEL_FLAG}
      -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/Dlang
      vtk.test.DlangGCAndDelete
     -T ${VTK_TEST_OUTPUT_DIR}
     )
  endif()
endif()

# Make sure .class files are cleaned up
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${VTK_DLANG_CLASS_FILES}")
install(FILES
  ${VTK_JAR_PATH}/vtk.jar
  DESTINATION ${VTK_INSTALL_LIBRARY_DIR}
  COMPONENT RuntimeLibraries)

#-----------------------------------------------------------------------------
# Post-Install script for Custom Dlang Packaging
# This install rule MUST stay at the bottom of that CMakeLists file as
# it has to be the last install rule that get executed
if(VTK_DLANG_INSTALL)
  set(MAVEN_NATIVE_ARTIFACT_XML)
  foreach(native_name ${MAVEN_NATIVE_ARTIFACTS})
     set(MAVEN_NATIVE_ARTIFACT_XML "${MAVEN_NATIVE_ARTIFACT_XML}\n                                <artifact><file>vtk-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}-natives-${native_name}.jar</file><classifier>natives-${native_name}</classifier><type>jar</type></artifact>")
  endforeach()
  if(VTK_DLANG_JOGL_COMPONENT)
    set(MAVEN_DEPENDENCY_XML "${MAVEN_DEPENDENCY_XML}\n      <dependency><groupId>org.jogamp.jogl</groupId><artifactId>jogl-all-main</artifactId><version>${JOGL_VERSION}</version></dependency>\n      <dependency><groupId>org.jogamp.gluegen</groupId><artifactId>gluegen-rt-main</artifactId><version>${JOGL_VERSION}</version></dependency>")
  endif()
  configure_file(DlangInstall.cmake.in
               ${VTK_BINARY_DIR}/DlangInstall.cmake @ONLY)
  configure_file(Maven/pom.xml.in
               ${VTK_BINARY_DIR}/pom.xml @ONLY)
  configure_file(Maven/README.txt.in
               ${VTK_BINARY_DIR}/MAVEN-README.txt @ONLY)
  install(SCRIPT ${VTK_BINARY_DIR}/DlangInstall.cmake)
endif()
