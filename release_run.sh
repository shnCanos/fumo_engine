uhh="release_build/"
project_name="${PWD##*/}"
EPIC="${uhh}${project_name}"
cmake --build release_build && ./$EPIC
