target_sources(assembler PRIVATE
    Lexer.cpp
    Parser.cpp
    OriginalSourceWriter.cpp
    Instructions.cpp
    Assembler.cpp
    SymbolResolvedWriter.cpp
    # Driver.cpp
)

target_link_libraries(assembler PRIVATE utils)