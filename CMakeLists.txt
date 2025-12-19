cmake_minimum_required(VERSION 3.14)
project(EcofunctionalPerceptron)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)

add_executable(EcofunctionalPerceptron
    main.cpp
    dataset.cpp
    dataset.h
    perceptron.cpp
    perceptron.h
    domain.cpp
    domain.h
    services.cpp
    services.h
    data_loader.cpp
    data_loader.h
)

target_link_libraries(EcofunctionalPerceptron PRIVATE nlohmann_json::nlohmann_json)
