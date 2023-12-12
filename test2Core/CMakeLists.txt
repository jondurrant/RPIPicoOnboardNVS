set(TEST_NAME "test2Core")
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# CppUTest
include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        master # or use release tag, eg. v4.0
)
# Set this to ON if you want to have the CppUTests in your project as well.
set(TESTS OFF CACHE BOOL "Switch off CppUTest Test build")
FetchContent_MakeAvailable(CppUTest)

include(../src/files.cmake)

add_executable(${TEST_NAME}
	test_main.cpp
	TestNVSMulticore.cpp
	
	${SRC_FILES}
)
        
target_include_directories(${TEST_NAME} PUBLIC
	"${CMAKE_CURRENT_LIST_DIR}/../src"
)

target_compile_definitions(${TEST_NAME} PRIVATE
	NVS_CORES=2
)

       

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(${TEST_NAME} PRIVATE
	pico_stdlib
	CppUTest::CppUTest
    CppUTest::CppUTestExt
    pico_multicore
)

target_compile_definitions(${TEST_NAME} PRIVATE
    PICO_CXX_DISABLE_ALLOCATION_OVERRIDES
    CPPUTEST_HAVE_EXCEPTIONS=1
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${TEST_NAME} 1)
pico_enable_stdio_uart(${TEST_NAME} 1)
target_compile_definitions(${TEST_NAME} PRIVATE
    PICO_DEFAULT_UART_RX_PIN=16
    PICO_DEFAULT_UART_TX_PIN=17
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${TEST_NAME})