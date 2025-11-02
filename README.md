# Crypto Assignment 1 
AES encryption/decryption in ECB mode using C++ with OpenSSL and OpenCV. Can handle terminal input, text, and image files.

## Docker install 

Follow the installation guide at: https://docs.docker.com/desktop/?_gl=1*ibi48z*_gcl_au*MzcxNTczMjgwLjE3NjIwNDYwOTk.*_ga*MTMxNzU5MzY0LjE3NjIwNDYxMDA.*_ga_XJWPQMJYHQ*czE3NjIwNDYwOTkkbzEkZzEkdDE3NjIwNDYxMDkkajUwJGwwJGgw 
For ubuntu: https://docs.docker.com/desktop/setup/install/linux/ubuntu/

```
sudo docker -t build aes_ecb_mode .
# to run it in an interactive way   ae
sudo docker run -it aes_ecb_mode bash
```
## Setup

1. Clone repo:  

```bash
git clone https://github.com/mat-tador/AES_ECB_mode.git
cd AES_ECB_mode
```

2. Build Docker container:
```bash
docker build -t aes_ecb_env .
```
4. Start container:
```bash
docker run -it aes_ecb_env
```
Inside the container, all dependencies (g++, OpenSSL, OpenCV, cxxopts) are ready.

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
3. Text file - Encrypt/decrypt message.txt:
```bash
./AES_ECB -t txt
```
4. Image file - Encrypt/decrypt linux_logo.jpg:
```bash
./AES_ECB -t jpg
```
5. Test vector verification - Check AES correctness:
```bash
./AES_ECB -v
```
## Notes
- For text and image file tests, make sure message.txt or linux_logo.jpg exist in the repo directory.
- Terminal input mode (-i) allows arbitrary strings.
- AES-256-ECB uses PKCS#7 padding, so plaintext of any length works.
- Custom test vector is included to verify encryption correctness.
- You must run the executable manually inside Docker.
