target_include_directories(utils PUBLIC Utils)
target_include_directories(assembler PUBLIC Assembler)
target_include_directories(vasm PUBLIC ASMTranslator)

# target_include_directories(utils PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/Utils>
#         $<INSTALL_INTERFACE:${NAND2TETRIS_INSTALL_INCLUDE_DIR}/nand2tetris/Utils>
# )

# target_include_directories(assembler PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Assembler>
#         $<INSTALL_INTERFACE:${NAND2TETRIS_INSTALL_INCLUDE_DIR}/nand2tetris/Assembler>
# )

# target_include_directories(vasm PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ASMTranslator>
#         $<INSTALL_INTERFACE:${NAND2TETRIS_INSTALL_INCLUDE_DIR}/nand2tetris/ASMTranslator>
# )
