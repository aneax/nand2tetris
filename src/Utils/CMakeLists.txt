target_sources(utils PRIVATE
    Errors.cpp
    Iterator.cpp
    Reader.cpp
    Characters.cpp
    # Driver.cpp
)

target_link_libraries(utils PUBLIC fmt::fmt-header-only)