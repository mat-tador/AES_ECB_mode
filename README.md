# Crypto Assignment 1 

## Running the code 
The code has three ways it can be run: 
- Give an input form terminal 
```
./executable -i Hello\ world! 
```
- Test with a text file named message.txt \
```
./executable -t txt
```
- test with a text image named linux_logo.jpeg 
```
./executable -t jpg
```
## Prerequisites 
You need to install the following software: 
- OpenSSL 
- g++ 
- openCV
- cxxopts


To install the necesseray software (for Ubuntu) you need to run the following command: 
```
sudo apt install g++ libssl-dev libopencv-dev   
mkdir -p ~/cpp-libs/cxxopts/include
cd ~/cpp-libs/cxxopts
wget https://raw.githubusercontent.com/jarro2783/cxxopts/master/include/cxxopts.hpp -O include/cxxopts.hpp


```

It is possible to check that the installation is done correctly by running the following commands: 
```
openssl version 
g++ --version
```
## Compiling the code 
In order to compile the code you must run: 
```
bash compile.sh
```