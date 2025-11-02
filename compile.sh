g++ AES_ECB.cpp -o AES_ECB -lssl -lcrypto $(pkg-config --cflags --libs opencv4) -I ~/cpp-libs/cxxopts/include
chmod +x AES_ECB
