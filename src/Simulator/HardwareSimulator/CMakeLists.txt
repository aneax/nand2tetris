target_sources(hardware_simulator PRIVATE
    Computer.cpp
    Memory.cpp
    Gates.cpp
    Parser.cpp
    Helper.cpp
)

target_link_libraries(hardware_simulator PUBLIC utils)