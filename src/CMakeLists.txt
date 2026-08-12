# 1. Collect every source file recursively from the src directory
FILE(GLOB_RECURSE app_sources 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# 2. Filter out any files ending with "native.cpp" from the collected list
list(FILTER app_sources EXCLUDE REGEX ".*_native\\.cpp$")

# 3. Register the filtered list and include directories with ESP-IDF
idf_component_register(
    SRCS ${app_sources}
    INCLUDE_DIRS "." "engine" "engine/wifi" "filesystem"
)
