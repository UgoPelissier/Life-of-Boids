FROM gcc:latest

# installing the dependent libraries for our project
RUN apt-get update -y && \
	apt-get install -y \
	cmake git python3-pip \
	libgtk2.0-dev libgl-dev libva-dev \
	libvdpau-dev libx11-xcb-dev libfontenc-dev \
	libxaw7-dev libxkbfile-dev libxmu-dev \
	libxmuu-dev libxpm-dev libxres-dev \
	libxss-dev libxtst-dev libxv-dev \
	libxvmc-dev libxxf86vm-dev xtrans-dev \
	libxcb-render-util0-dev libxcb-xkb-dev \
	libxcb-icccm4-dev libxcb-image0-dev \
	libxcb-keysyms1-dev libxcb-randr0-dev \
	libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev \
	libxcb-xinerama0-dev xkb-data libxcb-dri3-dev

# setting the environment variables
ENV VERSION 12
ENV MODE Release #Debug
ENV CXX g++
ENV CC gcc

# installing conan and cmake
RUN pip install conan && \
	conan profile new default --detect && \
	conan profile update settings.compiler=gcc default && \
	conan profile update settings.compiler.version="${VERSION}" default && \
	conan profile update settings.compiler.libcxx=libstdc++11 default

COPY conanfile.txt /tmp/conanfile.txt
RUN mkdir /tmp/installs/ && \
	cd /tmp/installs/ && \
	conan install .. --build=missing -c tools.system.package_manager:mode=install && \
	cd .. && rm -rf installs/

ENTRYPOINT /bin/bash
CMD echo -e '********************************\n\n\tLIFE-OF-BOIDS\n\n********************************\n\n'
