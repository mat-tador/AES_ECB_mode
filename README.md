# Crypto Assignment 1 
AES encryption/decryption in ECB mode using C++ with OpenSSL and OpenCV. Can handle terminal input, text, and image files.

## Docker install 

Follow the installation guide at: [Docker installation](https://docs.docker.com/desktop/?_gl=1*ibi48z*_gcl_au*MzcxNTczMjgwLjE3NjIwNDYwOTk.*_ga*MTMxNzU5MzY0LjE3NjIwNDYxMDA.*_ga_XJWPQMJYHQ*czE3NjIwNDYwOTkkbzEkZzEkdDE3NjIwNDYxMDkkajUwJGwwJGgw)

For ubuntu: [ubuntu installation](https://docs.docker.com/desktop/setup/install/linux/ubuntu/)


## Setup

1. Clone repo:  

```bash
git clone https://github.com/mat-tador/AES_ECB_mode.git
cd AES_ECB_mode
```
2. (ONLY for UBUNTU) Allow local Docker containers to access the host X11 display for GUI applications.
```
xhost +local:docker
```
3. Build Docker container:
```bash
sudo docker build -t aes_ecb .
```
for Windows CMD-PowerShell:
```bash
docker buildx build --load -t aes_ecb .
```

4. Start container 
(UBUNTU):
```bash
sudo docker run -it   --env DISPLAY=$DISPLAY   -v /tmp/.X11-unix:/tmp/.X11-unix   -v $(pwd):/AES_ECB_mode   aes_ecb
```
Inside the container, all dependencies (g++, OpenSSL, OpenCV, cxxopts) are ready.
(MacOS):
```
sudo docker run -it -p 8888:8888  -v $(pwd):/AES_ECB_mode     aes_ecb
```
(Windows CMD):
```
docker run -it -p 8888:8888 -v "%cd%":/AES_ECB_mode aes_ecb
```
(Windows Powershell):
```
docker run -it -p 8888:8888 -v ${PWD}:/AES_ECB_mode aes_ecb
```
(Windows CMD-Powershell)
```
cp /root/cpp-libs/cxxopts/include/cxxopts.hpp /AES_ECB_mode/
sed -i 's/\r$//' compile.sh
```
## Compile
```bash
bash compile.sh
```
Creates executable named AES_ECB.

## Run

1. Terminal input - Encrypt/decrypt a custom string:
```bash
./AES_ECB -i "Hello world!" -k <valid key size>
```
2. Text file - Encrypt/decrypt message.txt:
```bash
./AES_ECB -t txt -k <valid key size>
```
3. Image file - Encrypt/decrypt linux_logo.jpg:
```bash
./AES_ECB -t jpg -k <valid key size>
```
4. Test vector verification - AES-ECB correctness:
```bash
./AES_ECB -k <valid key size> -v
```
5. For Windows/MacOS to visualize image:
```
jupyter notebook --ip=0.0.0.0 --port=8888 --no-browser --allow-root
```
6. After the start of the jupyter notebook, run the show_results.ipynb 
