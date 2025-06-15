cmake_minimum_required(VERSION 3.16...3.27)
project(B1)

# Find Geant4 with UI and visualization components
find_package(Geant4 REQUIRED ui_all vis_all)

# Find HDF5 with C++ components
find_package(HDF5 REQUIRED COMPONENTS CXX)

# Locate sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add executable with all sources and headers (headers included for IDEs)
add_executable(exampleB1 exampleB1.cc ${sources} ${headers})

# Include directories for Geant4 and HDF5
target_include_directories(exampleB1 PRIVATE
  include
  ${HDF5_INCLUDE_DIRS}
)

# Link Geant4 and HDF5 libraries
target_link_libraries(exampleB1 PRIVATE
  ${Geant4_LIBRARIES}
  ${HDF5_CXX_LIBRARIES}
  ${HDF5_LIBRARIES}
)

# Copy macro scripts to build directory
set(EXAMPLEB1_SCRIPTS
  init_vis.mac
  run.mac
  vis.mac
)

foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
  )
endforeach()

