#!/bin/sh

# First prepare glmConfig.cmake
mkdir ext/glm/build && cd ext/glm/build
cmake ..
cd ../../../


declare -a CMAKE_PLATFORM_FLAGS
if [[ ${HOST} =~ .*linux.* ]]; then
    CMAKE_PLATFORM_FLAGS+=(-DCMAKE_TOOLCHAIN_FILE="${RECIPE_DIR}/cross-linux.cmake")
fi

mkdir build && cd build

# We want to link against the anaconda-provided openGL, so we have to
# set the preference to legacy.
cmake ..					\
      -DCMAKE_INSTALL_PREFIX=$PREFIX		\
      -DCMAKE_INSTALL_LIBDIR=$PREFIX/lib	\
      -DCMAKE_BUILD_TYPE=Release		\
      ${CMAKE_PLATFORM_FLAGS[@]}		\
      -DOpenGL_GL_PREFERENCE="LEGACY"           \
      -Dglm_DIR="ext/glm/build"

      # -DGLFW_BUILD_EXAMPLES=OFF			\
      # -DGLFW_BUILD_TESTS=OFF			\
      # -DGLFW_BUILD_DOCS=OFF			\


make -j $CPU_COUNT VERBOSE=1

# Install binary
install -m 755 recast3d $PREFIX/bin/
