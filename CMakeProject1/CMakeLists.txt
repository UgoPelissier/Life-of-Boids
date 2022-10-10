# CMakeList.txt : projet CMake pour CMakeProject1, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

project ("CMakeProject1")

# Ajoutez une source à l'exécutable de ce projet.
add_executable (CMakeProject1 "CMakeProject1.cpp" "CMakeProject1.h" "fonction.hpp")

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
