# Find doxygen package
find_package(Doxygen REQUIRED dot)
message("[INFO] Found Doxygen")
set(DOXYGEN_HTML_OUTPUT            ${PROJECT_BINARY_DIR}/docs/html)
set(DOXYGEN_JAVADOC_AUTOBRIEF      YES)
set(DOXYGEN_GENERATE_HTML          YES)
set(DOXYGEN_HAVE_DOT               YES)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)  # requires a README.md

SET(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include)
SET(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
SET(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
SET(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
SET(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Generate DoxyFile from the input file.
CONFIGURE_FILE(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Create Output directory.
FILE(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
# Command for generating doc from Doxygen config file.
ADD_CUSTOM_COMMAND(OUTPUT ${DOXYGEN_INDEX_FILE}
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
		MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
		COMMENT "Generating Doxygen documentation"
		VERBATIM)
# Create CMake Target for generating doc.
ADD_CUSTOM_TARGET(docs ALL DEPENDS ${DOXYGEN_INDEX_FILE})

doxygen_add_docs(docs_target
  ${PROJECT_SOURCE_DIR}
  ALL  # does not seem to work
  COMMENT "Generate HTML documentation"
)



