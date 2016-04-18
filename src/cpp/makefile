GPP=g++

error:
	@echo "Please choose one of the following targets: debian_armhf debian_x86_64"
	@exit 2

# Compiled on an armhf machine (eg raspbery pi); not cross compiled
debian_armhf: _nix_based/jssc.cpp jssc_Common.cpp
	$(GPP) -I. -I"/usr/lib/jvm/default-java/include" -fpic -o libjSSC-2.9_armhf.so -shared _nix_based/jssc.cpp jssc_Common.cpp

debian_x86_64: _nix_based/jssc.cpp jssc_Common.cpp
	$(GPP) -I. -I"/usr/lib/jvm/default-java/include" -m64 -fpic -o libjSSC-2.9_x86_64.so -shared _nix_based/jssc.cpp jssc_Common.cpp
