# =============================================================================
# Build Source Targets
# =============================================================================

# Show a message to indicate that we are going through the ./src/ directory
message ("Going through ./src/")

# -----------------------------------------------------------------------------
# QtVTKFramework
# -----------------------------------------------------------------------------

# Set the source files for the QtVTKFramework target
add_executable(QtVTKFramework
    QtVTKFramework.cxx
    MainWindow.cxx
    MainWindow.h
    MainWindow.ui
)

# Link the QtVTKFramework target with the VTK libraries and the Qt5::Widgets
# library
target_link_libraries(QtVTKFramework
  PRIVATE
    clipp
    ${VTK_LIBRARIES}
    Qt5::Widgets
)

# Use the VTK_MODULE_AUTOINIT() function to automatically initialize the VTK
vtk_module_autoinit(
  TARGETS QtVTKFramework
  MODULES
    ${VTK_LIBRARIES}
)
