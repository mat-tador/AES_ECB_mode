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
4. Start container (UBUNTU):
```bash
sudo docker run -it   --env DISPLAY=$DISPLAY   -v /tmp/.X11-unix:/tmp/.X11-unix   -v $(pwd):/AES_ECB_mode   aes_ecb_mode
```
Inside the container, all dependencies (g++, OpenSSL, OpenCV, cxxopts) are ready.
5. Start container (Windows/MacOS)
```
sudo docker run -it -p 8888:8888  -v $(pwd):/AES_ECB_mode     aes_ecb_mode
```

## Compile
```bash
bash compile.sh
```
Creates executable named AES_ECB.

## Run

1. Terminal input - Encrypt/decrypt a custom string:
```bash
./AES_ECB -i "Hello world!"
```
2. Text file - Encrypt/decrypt message.txt:
```bash
./AES_ECB -t txt
```
3. Image file - Encrypt/decrypt linux_logo.jpg:
```bash
./AES_ECB -t jpg
```

4. For Windows/mac to visualize image:
```
jupyter notebook --ip=0.0.0.0 --port=8888 --no-browser --allow-root
```
5. After the start of the jupyter notebook, run the show_results.ipynb 
