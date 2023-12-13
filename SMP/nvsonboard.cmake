# Add library cpp files
if (DEFINED (NOT NVS_ONBOARD_PATH))
	SET(NVS_ONBOARD_PATH "${CMAKE_CURRENT_LIST_DIR}" CACHE STRING "Lib")
endif ()
add_library(nvs_onboard STATIC)
target_sources(nvs_onboard PUBLIC
    ${NVS_ONBOARD_PATH}/src/NVSOnboard.cpp
)

# Add include directory
target_include_directories(nvs_onboard PUBLIC 
	${NVS_ONBOARD_PATH}/src
)

# Add the standard library to the build
target_link_libraries(nvs_onboard PUBLIC 
	pico_stdlib
	FreeRTOS-Kernel
	hardware_flash
)