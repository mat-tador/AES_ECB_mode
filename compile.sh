g++ AES_ECB.cpp -o AES_ECB -lssl -lcrypto $(pkg-config --cflags --libs opencv4) -I /root/cpp-libs/cxxopts/include

