// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <assert.h>
#include <dirent.h>
#include <openenclave/bits/report.h>
#include <openenclave/host.h>
#include <openenclave/internal/report.h>
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
    std::vector<uint8_t> report;
    if (argc != 2)
    {
        fprintf(
            stderr, "Usage: %s << Path to the respective file >> \n", argv[0]);
        exit(1);
    }
    setenv("AZDCAP_BASE_CERT_URL", "https://americas.test.acccache.azure.net/sgx/certificates", 1);
    report = FileToBytes(argv[1]);
    result = oe_verify_remote_report( &report[0], report.size() - 1, NULL);
    printf("\n the string is %s \n", oe_result_str(result));

    return 0;
}
