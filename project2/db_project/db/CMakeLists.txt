# Sources
set(DB_SOURCE_DIR src)
set(DB_SOURCES
  ${DB_SOURCE_DIR}/file.cc
  # Add your sources here
  # ${DB_SOURCE_DIR}/foo/bar/your_source.cc
  ${DB_SOURCE_DIR}/page.cc
  ${DB_SOURCE_DIR}/table.cc
  ${DB_SOURCE_DIR}/tree.cc
  )

# Headers
set(DB_HEADER_DIR include)
set(DB_HEADERS
  ${DB_HEADER_DIR}/file.h
  # Add your headers here
  # ${DB_HEADER_DIR}/foo/bar/your_header.h
  ${DB_HEADER_DIR}/table.h
  ${DB_HEADER_DIR}/errors.h
  ${DB_HEADER_DIR}/page.h
  ${DB_HEADER_DIR}/types.h
  ${DB_HEADER_DIR}/tree.h
  )

add_library(db STATIC ${DB_HEADERS} ${DB_SOURCES})

target_include_directories(db
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${DB_HEADER_DIR}"
  )

