add_library(sim
        progargs.cpp
        grid.cpp
        block.cpp
        math.cpp
        files.cpp
        particle.cpp
)
# Use this line only if you have dependencies from stim to GSL
target_link_libraries (sim PRIVATE Microsoft.GSL::GSL)