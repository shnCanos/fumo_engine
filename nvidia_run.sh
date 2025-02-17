uhh="build/"
project_name="${PWD##*/}"
EPIC="${uhh}${project_name}"
export DRI_PRIME=1
cmake --build build && ./$EPIC
