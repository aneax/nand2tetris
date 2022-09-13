target_sources(vasm PRIVATE
    ASMGenerator.cpp
    VMParser.cpp
    VMCommand.cpp
)


target_link_libraries(vasm PUBLIC utils)