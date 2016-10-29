all: debian_x64 debian_x86

debian_x64: ../../cpp/_nix_based/jssc.cpp
	g++ -march=x86-64 -m64 -I"../../cpp" -I"/usr/lib/jvm/default-java/include" -I"/usr/lib/jvm/default-java/include/linux" -fpic -o linux/libjSSC-2.9_x86_64.so -shared ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp

debian_x86: ../../cpp/_nix_based/jssc.cpp
	g++ -march=i386 -m32 -I"../../cpp" -I"/usr/lib/jvm/default-java/include" -I"/usr/lib/jvm/default-java/include/linux" -fpic -o linux/libjSSC-2.9_x86.so -shared ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp

# Compiled on an armhf machine (eg raspbery pi); not cross compiled
debian_armhf: _nix_based/jssc.cpp jssc_Common.cpp
	g++ -I"../../cpp" -I"/usr/lib/jvm/default-java/include" -I"/usr/lib/jvm/default-java/include/linux" -fpic -o linux/libjSSC-2.9_armhf.so -shared ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp

cleanall:
	rm linux/libjSSC-2.9_x86_64.so linux/libjSSC-2.9_x86.so linux/libjSSC-2.9_armhf.so
