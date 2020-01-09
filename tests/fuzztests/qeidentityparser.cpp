// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <assert.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>

#include "../../common/sgx/tcbinfo.h"

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
    std::vector<uint8_t> qeIndentityInfo;

    if (argc != 2)
    {
        fprintf(
            stderr, "Usage: %s << Path to the respective file >> \n", argv[0]);
        exit(1);
    }

    qeIndentityInfo = FileToBytes(argv[1]);

    oe_parsed_qe_identity_info_t parsedinfo;
    oe_qe_identity_info_tcb_level_t platform_tcb_level = {{0}};

    result = oe_parse_qe_identity_info_json(
        (uint8_t*)&qeIndentityInfo[0],
        (uint32_t)qeIndentityInfo.size(),
        &platform_tcb_level,
        &parsedinfo);

    printf(
        "\n oe_parse_qe_identity_info_json "
        "return = %s\n",
        oe_result_str(result));
    return 0;
}
