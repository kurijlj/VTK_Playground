# =============================================================================
# Build Source Targets
# =============================================================================

# Show a message to indicate that we are going through the ./src/ directory
message ("Going through ./src/")

# -----------------------------------------------------------------------------
# QtVTKFramework
# -----------------------------------------------------------------------------

# Show message that we are building the QtVTKFramework target
message (STATUS "Building the `QtVTKFramework` target")

# Set the source files for the `QtVTKFramework` target
add_executable(QtVTKFramework
    QtVTKFramework.cxx
    MainWindow.cxx
    MainWindow.h
    MainWindow.ui
)

# Link the `QtVTKFramework` target with the VTK libraries and the Qt5::Widgets
# library
target_link_libraries(QtVTKFramework
  PRIVATE
    clipp
    ${VTK_LIBRARIES}
    Qt5::Widgets
)

# Use this to copy individual files to the binary directory
add_custom_command(
    TARGET QtVTKFramework POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            ${CMAKE_BINARY_DIR}/bin/images
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/images/about.png
            ${CMAKE_BINARY_DIR}/bin/images/about.png
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/images/logout.png
            ${CMAKE_BINARY_DIR}/bin/images/logout.png
)

# Use this to copy the images directory to the binary directory
# add_custom_command(
#     TARGET QtVTKFramework POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#             ${CMAKE_SOURCE_DIR}/src/images
#             ${CMAKE_BINARY_DIR}/bin/images
# )

# Use the VTK_MODULE_AUTOINIT() function to automatically initialize the VTK
vtk_module_autoinit(
  TARGETS QtVTKFramework
  MODULES
    ${VTK_LIBRARIES}
)

# Use this to include individual resources in the binary directory
qt5_add_resources(QtVTKFramework imageresources
    PREFIX "/images"
    FILES
        about.png
        logout.png
)

# Use this to include all the resources in the binary directory
# qt5_add_resources(QtVTKFramework imageresources
#     PREFIX "/images"
#     FILES
#         add.png
#         approval.png
#         bulleted-list.png
#         checked-checkbox.png
#         checklist.png
#         checkmark.png
#         close.png
#         close-window.png
#         content.png
#         copy.png
#         create.png
#         cut.png
#         decrease.png
#         done.png
#         download.png
#         downloads.png
#         edit.png
#         enter.png
#         error.png
#         expired.png
#         help.png
#         hierarchy.png
#         high-priority.png
#         increase.png
#         info.png
#         logout.png
#         maximize-window.png
#         menu.png
#         minimize-window.png
#         natural-user-interface-2.png
#         notification.png
#         open-email.png
#         paste.png
#         private.png
#         process.png
#         redo.png
#         remove.png
#         restore-window.png
#         ruler.png
#         save-as.png
#         screenshot.png
#         thumbnails.png
#         tools.png
#         trash-can.png
#         tree-structure.png
#         unchecked-checkbox.png
#         undo.png
#         upload.png
#         view-details.png
#         workflow.png
#         zoom-in.png
#         zoom-out.png
# )