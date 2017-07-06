rm sdl2example

c++ `sdl2-config --cflags` -I ../.. main.cpp imgui_impl_sdl.cpp ../../imgui*.cpp `sdl2-config --libs` -lGL -o sdl2example

./sdl2example
