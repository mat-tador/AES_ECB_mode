#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <openssl/evp.h>
#include "cxxopts.hpp"
#include <fstream>
#include <opencv2/opencv.hpp>

// AES key (32 bytes for AES-256)
// Warning: For real applications, do NOT store secret keys in plaintext :)
const unsigned char SUPER_SECRET_KEY[32] = "0123456789abcdef0123456789abcdef";
//const unsigned char SUPER_SECRET_KEY[33] = "0123456789abcdef0123456789abcdef";
// this array size is wrong because end of the c string literal add automatically \0 .

// Encrypt a plaintext buffer using AES-256-ECB with PKCS#7 padding
void encrypt(const std::vector<unsigned char>& plaintext,
             std::vector<unsigned char>& ciphertext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, SUPER_SECRET_KEY, NULL);
    EVP_CIPHER_CTX_set_padding(ctx, 1); // Enable PKCS#7 padding

    int len = 0;
    ciphertext.resize(plaintext.size() + 16); // max possible size
    EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size());
    int ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
    ciphertext_len += len;
    ciphertext.resize(ciphertext_len); // shrink to actual size

    std::cout << "Cipher length: " << ciphertext_len;
  
    EVP_CIPHER_CTX_free(ctx);
}

// Decrypt a ciphertext buffer using AES-256-ECB with PKCS#7 padding
void decrypt(const std::vector<unsigned char>& ciphertext,
             std::vector<unsigned char>& decrypted) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, SUPER_SECRET_KEY, NULL);
    EVP_CIPHER_CTX_set_padding(ctx, 1); // Enable PKCS#7 padding

    int len = 0;
    decrypted.resize(ciphertext.size());
    EVP_DecryptUpdate(ctx, decrypted.data(), &len, ciphertext.data(), ciphertext.size());
    int decrypted_len = len;
    EVP_DecryptFinal_ex(ctx, decrypted.data() + len, &len);
    decrypted_len += len;
    decrypted.resize(decrypted_len);
  
    EVP_CIPHER_CTX_free(ctx);
}

// Encrypt/Decrypt helper using a custom key (used for test vector verification)
void encrypt_with_key(const std::vector<unsigned char>& plaintext,
                      std::vector<unsigned char>& ciphertext,
                      const unsigned char* key, size_t key_len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER* cipher = nullptr;
  
    if (key_len == 16) cipher = EVP_aes_128_ecb();
    else if (key_len == 24) cipher = EVP_aes_192_ecb();
    else cipher = EVP_aes_256_ecb();
  
    EVP_EncryptInit_ex(ctx, cipher, NULL, key, NULL);
    EVP_CIPHER_CTX_set_padding(ctx, 1);
  
    int len = 0;
    ciphertext.resize(plaintext.size() + 16);
    EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size());
  
    int ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
  
    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);
    EVP_CIPHER_CTX_free(ctx);
}

void decrypt_with_key(const std::vector<unsigned char>& ciphertext,
                      std::vector<unsigned char>& decrypted,
                      const unsigned char* key, size_t key_len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER* cipher = nullptr;
  
    if (key_len == 16) cipher = EVP_aes_128_ecb();
    else if (key_len == 24) cipher = EVP_aes_192_ecb();
    else cipher = EVP_aes_256_ecb();
  
    EVP_DecryptInit_ex(ctx, cipher, NULL, key, NULL);
    EVP_CIPHER_CTX_set_padding(ctx, 1);
  
    int len = 0;
    decrypted.resize(ciphertext.size());
    EVP_DecryptUpdate(ctx, decrypted.data(), &len, ciphertext.data(), ciphertext.size());
  
    int decrypted_len = len;
    EVP_DecryptFinal_ex(ctx, decrypted.data() + len, &len);
  
    decrypted_len += len;
    decrypted.resize(decrypted_len);
    EVP_CIPHER_CTX_free(ctx);
}

// Print a buffer in hexadecimal format with 32 bytes (256-bit) per line
void print_ciphertext(const std::vector<unsigned char>& buffer){
    const int block_size = 16;
    for (size_t i = 0; i < buffer.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(buffer[i]) ;
        if ((i + 1) % block_size == 0) std::cout << std::endl;
    }
    std::cout << std::dec << std::endl;
}
// Encrypt + Decrypt wrapper
void exec_encryption_input(const std::string& text) {
    std::vector<unsigned char> plaintext(text.begin(), text.end());

    std::vector<unsigned char> ciphertext;
    encrypt(plaintext, ciphertext);

    std::cout << "Ciphertext:" << std::endl;
    print_ciphertext(ciphertext);

    std::vector<unsigned char> decrypted;
    decrypt(ciphertext, decrypted);

    std::string decrypted_text(decrypted.begin(), decrypted.end());
    std::cout << "Payload text: " << decrypted_text << std::endl;
}

// Encrypt and decrypt a text file (message.txt)
void exec_encryption_text(){

    const std::string filename = "message.txt";

    //Open the file in binary and go straight to the end 
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> plaintext(size);
    if (!file.read(reinterpret_cast<char*>(plaintext.data()), size)) {
        throw std::runtime_error("Error reading file: " + filename);
    }


    std::vector<unsigned char> ciphertext; 

    encrypt(plaintext, ciphertext); 
    print_ciphertext(ciphertext); 

    std::vector<unsigned char> decrypted; 
    decrypt(ciphertext, decrypted); 
    std::string decrypted_text(decrypted.begin(), decrypted.end());
    std::cout << "Payload text: " << decrypted_text << std::endl;

}

// Encrypt and decrypt an image file (linux_logo.jpg)
void exec_encryption_image(){
    const std::string filename = "linux_logo.jpg";

    // Load image
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
    if (img.empty()) {
        throw std::runtime_error("Cannot open image: " + filename);
    }

    std::cout << "Original image size: " << img.cols << "x" << img.rows << std::endl;

    // Flatten image data to a single byte buffer for AES encryption
    std::vector<unsigned char> plaintext;
    if (img.isContinuous()) {
        plaintext.assign(img.data, img.data + img.total() * img.channels());
    } else {
        for (int i = 0; i < img.rows; ++i)
            plaintext.insert(plaintext.end(), img.ptr<unsigned char>(i), img.ptr<unsigned char>(i) + img.cols * img.channels());
    }

    // Pad rows to multiple of 16 for ECB
    //int row_bytes = img.cols * img.channels();
    
    std::vector<unsigned char> ciphertext;
    encrypt(plaintext, ciphertext);
  
// NOTE: AES-ECB encrypt data 16 bytes per block. 
// If data not 16 bytes multiple, PKCS7 padding add extra bytes. 
// So output can be little bigger than input. 
// This padding make sure each block always 16 bytes.
  
    size_t valid_size = std::min(ciphertext.size(), plaintext.size());

    // Rebuild the encrypted image (same dimensions as the original)
    cv::Mat ecb_img(img.rows, img.cols, img.type(), (void*)ciphertext.data());
    ecb_img = ecb_img.clone(); 
    // clone() is like taking a snapshot, so OpenCV doesn't get confused by extra padding bytes


    // Convert ciphertext back to image
    //cv::Mat ecb_img(img.rows, row_bytes / img.channels(), img.type(), ciphertext.data());
    // -- This line not need because ciphertext maybe bigger than original data. -- //

    // Save the ECB-encrypted image
    std::string filename_ecb = "ecb_leak.jpg";
    cv::imwrite(filename_ecb, ecb_img);
    std::cout << "ECB-encrypted image saved as " << filename_ecb << std::endl;

    // Decrypt back to original pixel data
    std::vector<unsigned char> decrypted;
    decrypt(ciphertext, decrypted);

    // Remove padding for proper image reconstruction
    decrypted.resize(plaintext.size());
  
    cv::Mat decrypted_img(img.rows, img.cols, img.type(), decrypted.data());

  // Save the decrypted image for verification
    std::string filename_decrypted = "decrypted.jpg";
    cv::imwrite(filename_decrypted, decrypted_img);
    std::cout << "Decrypted image saved as " << filename_decrypted << std::endl;

  // Display both original (decrypted) and ECB-encrypted images side by side
    cv::Mat canvas(img.rows, img.cols*2, img.type()); 
    decrypted_img.copyTo(canvas(cv::Rect(0, 0, img.cols, img.rows))); 
    ecb_img.copyTo(canvas(cv::Rect(img.cols, 0, img.cols, img.rows))); 
  
    cv::imshow("Plain (Left) vs ECB encripted (Right)", canvas); 
    cv::waitKey(0); 
    cv::destroyAllWindows(); 
}

// Custom AES-256-ECB Test Vector -- self generated
void verify_custom_aes_vector() {
    std::cout << " ** AES-256-ECB custom test vector verification ** \n";

    // Custom AES-256 key (32 bytes)
    const unsigned char test_key[32] = {
        0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
        0x88,0x99,0x6d,0x65,0x6c,0x69,0x6b,0x65,
        0x61,0x79,0x73,0x65,0x6e,0x75,0x72,0x20,
        0x79,0x69,0x6c,0x64,0x69,0x72,0x69,0x6d
    };

    // Custom plaintext (16 bytes)
    std::vector<unsigned char> pt = {
    'S','E','C','U','R','E','_','T','E','S','T','_','B','L','O','C','K'
    };

    // Expected ciphertext (calculated via OpenSSL)
    const unsigned char expected_ct[16] = {
    0x8a,0x7e,0x12,0x59,0xd2,0x4b,0x4c,0xa8,0x01,0x94,0xa3,0x31,0xb3,0x5c,0x2a,0x9f
    };

    std::vector<unsigned char> ct;
    encrypt_with_key(pt, ct, test_key, sizeof(test_key));

    bool match = true;
    for (int i = 0; i < 16; ++i)
        if (ct[i] != expected_ct[i]) {
         match = false; 
         break;
        }

    if (match)
        std::cout << "Custom AES-256-ECB vector matched successfully\n";
    else
        std::cout << "Custom test vector mismatch\n";
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("AES_ECB", "Encryption and Decryption using AES in ECB mode");
    options.add_options()
        ("i,input", "Input text to encrypt", cxxopts::value<std::string>())
        ("t,test", "Specify <txt> or <jpg> to run the encryption/decryption on either of the file: message.txt, linux_logo.jpg", cxxopts::value<std::string>())
        ("v,verify", "Run custom AES-256 test vector verification") //new
        ("h,help", "HELP!");

    try {
        auto result = options.parse(argc, argv);
        if (argc == 1) throw cxxopts::exceptions::exception("No input provided");

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("input")) {
            std::string input_string = result["input"].as<std::string>();
            //std::cout << "Payload text: " << input_string << std::endl;
            // not only text input --
            exec_encryption_input(input_string);
        }

        if (result.count("test")){
            std::string input_string = result["test"].as<std::string>(); 
            
            if (input_string == "txt"){
                exec_encryption_text(); 
            }
            else if (input_string == "jpg"){
                exec_encryption_image(); 
            }
            else {
                throw cxxopts::exceptions::no_such_option("Invalid test mode. Please use 'txt' or 'jpg'.");
            }
        }
        if (result.count("verify")){
            verify_custom_aes_vector();
        }
    } catch (const cxxopts::exceptions::no_such_option& e){
        std::cerr << "Error: " << e.what() << "\n\n";
        std::cout << options.help() << std::endl;
        return 1;

    } // catch (const cxxopts::exceptions::exception& e) {
      // std::cerr << "Error: " << e.what() << "\n\n";
      //  std::cout << options.help() << std::endl;
      //  return 1;
   // } 

    return 0;
}
