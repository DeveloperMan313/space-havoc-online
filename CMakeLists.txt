cmake_minimum_required(VERSION 3.16)
project(AstroParty_Online)

set(CMAKE_CXX_STANDARD 14)
set(SFML_STATIC_LIBRARIES TRUE)

set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
add_executable(AstroParty_Online main.cpp Game.cpp Math.cpp Player.cpp Projectile.cpp RigidBody.cpp Server.cpp Client.cpp Textures.cpp PlayerWeak.cpp Powerup.cpp Powerup.h Laserbeam.cpp Laserbeam.h)

set(CMAKE_BUILD_TYPE RELEASE)
find_package(SFML 2 COMPONENTS graphics window system audio network REQUIRED)

include_directories(C:/SFML-2.5.1/include)
target_link_libraries(AstroParty_Online sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
