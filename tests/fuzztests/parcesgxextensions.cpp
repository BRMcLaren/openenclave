#include <assert.h>
#include <dirent.h>
#include <openenclave/bits/report.h>
#include <openenclave/host.h>
#include <openenclave/internal/cert.h>
#include <openenclave/internal/raise.h>
#include <openenclave/internal/report.h>
#include <openenclave/internal/sgxcertextensions.h>
#include <openenclave/internal/tests.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>

std::vector<uint8_t> FileToBytes(const char* path)
{
    std::ifstream f(path, std::ios::binary);
    std::vector<uint8_t> bytes = std::vector<uint8_t>(
        std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());

    if (bytes.empty())
    {
        printf("File %s not found\n", path);
        exit(1);
    }
    bytes.push_back('\0');
    return bytes;
}

int main(int argc, char** argv)
{
    oe_result_t result;
    std::vector<uint8_t> buffer_parse;
    if (argc != 2)
    {
        fprintf(
            stderr, "Usage: %s << Path to the respective file >> \n", argv[0]);
        exit(1);
    }
    result = OE_FAILURE;
    size_t buffer_size = 1024;
    uint8_t* buffer;
    ParsedExtensionInfo parsed_extension_info = {{0}};
    oe_cert_t leaf_cert = {0};
    oe_cert_chain_t pck_cert_chain = {0};
    uint8_t* pem_pck_certificate;
    size_t pem_pck_certificate_size;
    buffer_parse = FileToBytes(argv[1]);

    pem_pck_certificate = &buffer_parse[0];
    pem_pck_certificate_size = (buffer_parse.size() - 1);
    oe_cert_chain_read_pem(
        &pck_cert_chain, pem_pck_certificate, pem_pck_certificate_size);
    oe_cert_chain_get_leaf_cert(&pck_cert_chain, &leaf_cert);
    buffer = (uint8_t*)malloc(buffer_size);
    result = ParseSGXExtensions(
        &leaf_cert, buffer, &buffer_size, &parsed_extension_info);
    free(buffer);
    printf(
        "\n the result of parse_sgx_extensions is %s \n",
        oe_result_str(result));
}
