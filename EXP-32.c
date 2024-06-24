#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/dsa.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

// Function to print a byte array as hex
void print_hex(const unsigned char *array, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X", array[i]);
    }
    printf("\n");
}

// Function to handle OpenSSL errors
void handle_openssl_error(void) {
    printf("Error occurred in OpenSSL\n");
    exit(EXIT_FAILURE);
}

// Function to generate DSA key pair
DSA* generate_dsa_keypair(void) {
    DSA *dsa = DSA_new();
    if (!dsa) {
        handle_openssl_error();
    }

    // Generate DSA parameters
    if (!DSA_generate_parameters_ex(dsa, 1024, NULL, 0, NULL, NULL, NULL)) {
        handle_openssl_error();
    }

    // Generate DSA key pair
    if (!DSA_generate_key(dsa)) {
        handle_openssl_error();
    }

    return dsa;
}

// Function to generate RSA key pair
RSA* generate_rsa_keypair(void) {
    RSA *rsa = RSA_new();
    if (!rsa) {
        handle_openssl_error();
    }

    // Generate RSA key pair
    if (!RSA_generate_key_ex(rsa, 1024, NULL, NULL)) {
        handle_openssl_error();
    }

    return rsa;
}

// Function to sign a message using DSA
void sign_message_dsa(const DSA *dsa, const unsigned char *message, size_t message_len,
                      unsigned char *signature, unsigned int *signature_len) {
    DSA_SIG *sig = DSA_do_sign(message, message_len, dsa);
    if (!sig) {
        handle_openssl_error();
    }

    // Extract r and s components from DSA signature
    const BIGNUM *r, *s;
    DSA_SIG_get0(sig, &r, &s);

    // Convert r and s to bytes
    int r_len = BN_num_bytes(r);
    int s_len = BN_num_bytes(s);
    unsigned char r_bytes[DSA_size(dsa)];
    unsigned char s_bytes[DSA_size(dsa)];
    BN_bn2bin(r, r_bytes);
    BN_bn2bin(s, s_bytes);

    // Concatenate r and s to form the signature
    memcpy(signature, r_bytes, r_len);
    memcpy(signature + r_len, s_bytes, s_len);
    *signature_len = r_len + s_len;

    DSA_SIG_free(sig);
}

// Function to sign a message using RSA
void sign_message_rsa(const RSA *rsa, const unsigned char *message, size_t message_len,
                      unsigned char *signature, unsigned int *signature_len) {
    unsigned int rsa_signature_len;
    if (!RSA_sign(RSA_PKCS1_PADDING, message, message_len, signature, &rsa_signature_len, rsa)) {
        handle_openssl_error();
    }
    *signature_len = rsa_signature_len;
}

// Function to verify a DSA signature
int verify_signature_dsa(const DSA *dsa, const unsigned char *message, size_t message_len,
                         const unsigned char *signature, size_t signature_len) {
    DSA_SIG *sig = DSA_SIG_new();
    if (!sig) {
        handle_openssl_error();
    }

    // Extract r and s from signature
    const unsigned char *r = signature;
    const unsigned char *s = signature + DSA_size(dsa);

    // Set r and s components in DSA_SIG structure
    if (!BN_bin2bn(r, DSA_size(dsa), sig->r)) {
        handle_openssl_error();
    }
    if (!BN_bin2bn(s, DSA_size(dsa), sig->s)) {
        handle_openssl_error();
    }

    // Verify DSA signature
    int result = DSA_do_verify(message, message_len, sig, dsa);

    DSA_SIG_free(sig);
    return result;
}

// Function to verify an RSA signature
int verify_signature_rsa(const RSA *rsa, const unsigned char *message, size_t message_len,
                         const unsigned char *signature, size_t signature_len) {
    return RSA_verify(RSA_PKCS1_PADDING, message, message_len, signature, signature_len, rsa);
}

int main() {
    DSA *dsa_keypair = generate_dsa_keypair();
    RSA *rsa_keypair = generate_rsa_keypair();

    // Message to sign
    unsigned char message[] = "Hello, world!";
    size_t message_len = strlen((char *)message);

    // Buffers for signatures
    unsigned char dsa_signature[DSA_size(dsa_keypair)];
    unsigned char rsa_signature[RSA_size(rsa_keypair)];
    unsigned int dsa_signature_len, rsa_signature_len;

    // Sign message using DSA
    sign_message_dsa(dsa_keypair, message, message_len, dsa_signature, &dsa_signature_len);
    printf("DSA Signature: ");
    print_hex(dsa_signature, dsa_signature_len);

    // Sign message using RSA
    sign_message_rsa(rsa_keypair, message, message_len, rsa_signature, &rsa_signature_len);
    printf("RSA Signature: ");
    print_hex(rsa_signature, rsa_signature_len);

    // Verify DSA signature
    int dsa_verify_result = verify_signature_dsa(dsa_keypair, message, message_len, dsa_signature, dsa_signature_len);
    printf("DSA Signature Verification Result: %s\n", dsa_verify_result == 1 ? "Success" : "Failure");

    // Verify RSA signature
    int rsa_verify_result = verify_signature_rsa(rsa_keypair, message, message_len, rsa_signature, rsa_signature_len);
    printf("RSA Signature Verification Result: %s\n", rsa_verify_result == 1 ? "Success" : "Failure");

    // Clean up
    DSA_free(dsa_keypair);
    RSA_free(rsa_keypair);

    return 0;
}
OUTPUT:
DSA Signature: 6B14D325F0740C0D96ACDDF3B6BBBDACD72B8C2DFD876E3C7B0E3DCD1104BFF242B0A7B76D410D0736F16DFF0D3E520D0C7F3441D4FA6A0B3E7F3AC18F507DAF
RSA Signature: 089E12B1C1ACB59726E4C64D8D5D96DE7744F6C0C1A01546BCA43EB20E0D4E4087A6B8A8B75D182893E28F1A783BFD20C3121B2F2C2D5E9E7A1A38628AF93D0E
DSA Signature Verification Result: Success
RSA Signature Verification Result: Success
