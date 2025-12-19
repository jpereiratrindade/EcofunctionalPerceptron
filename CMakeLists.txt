cmake_minimum_required(VERSION 3.14)
project(EcofunctionalPerceptron)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)

add_executable(EcofunctionalPerceptron
    src/main.cpp
    src/dataset.cpp
    src/perceptron.cpp
    src/domain.cpp
    src/services.cpp
    src/data_loader.cpp
)

target_include_directories(EcofunctionalPerceptron PRIVATE include)
target_link_libraries(EcofunctionalPerceptron PRIVATE nlohmann_json::nlohmann_json)
