all: debian_x64 debian_x86

debian_x64: ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp
	g++ -march=x86-64 -m64 -I"../../cpp" -I"/usr/lib/jvm/default-java/include" -I"/usr/lib/jvm/default-java/include/linux" -fpic -o linux/libjSSC-2.9_x86_64.so -shared ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp

debian_x86: ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp
	g++ -march=i386 -m32 -I"../../cpp" -I"/usr/lib/jvm/default-java/include" -I"/usr/lib/jvm/default-java/include/linux" -fpic -o linux/libjSSC-2.9_x86.so -shared ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp

# Compiled on an armhf machine (eg raspbery pi); not cross compiled
debian_armhf: ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp
	g++ -I"../../cpp" -I"/usr/lib/jvm/default-java/include" -I"/usr/lib/jvm/default-java/include/linux" -fpic -o linux/libjSSC-2.9_armhf.so -shared ../../cpp/_nix_based/jssc.cpp ../../cpp/jssc_Common.cpp

# For windows... with mingw-w64, from a mingw terminal, maybe this?:

# jssc/src/cpp #> g++ -Wall -Wl,--kill-at -I. -I"C:\Program Files\Java\jdk1.8.0_71\include" -I"C:\Program Files\Java\jdk1.8.0_71\include\win32" -shared jssc_Common.cpp windows\jssc.c++ -o jSSC-2.9_x86_64.dll

cleanall:
	rm linux/libjSSC-2.9_x86_64.so linux/libjSSC-2.9_x86.so linux/libjSSC-2.9_armhf.so
