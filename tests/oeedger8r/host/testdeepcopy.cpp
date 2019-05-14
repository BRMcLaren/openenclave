// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "../edltestutils.h"

#include <openenclave/host.h>
#include <openenclave/internal/tests.h>
#include "all_u.h"

void test_deepcopy_edl_ecalls(oe_enclave_t* enclave)
{
    uint64_t data[8] = {0x1112131415161718,
                        0x2122232425262728,
                        0x3132333435363738,
                        0x4142434445464748,
                        0x5152535455565758,
                        0x6162636465666768,
                        0x7172737475767778,
                        0x8182838485868788};

    struct MyStruct s[] = {{7ULL, 64ULL, data}, {3ULL, 32ULL, &data[4]}};
    OE_TEST(s[0].ptr[0] == data[0]);
    OE_TEST(s[0].ptr[7] == data[7]);
    OE_TEST(s[1].ptr[0] == data[4]);
    OE_TEST(s[1].ptr[3] == data[7]);

    OE_TEST(deepcopy_value(enclave, s[0], data) == OE_OK);
    OE_TEST(deepcopy_shallow(enclave, &s[0], data) == OE_OK);

    printf("=== test_deepcopy_edl_ecalls passed\n");
}
