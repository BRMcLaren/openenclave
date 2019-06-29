// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <openenclave/bits/safecrt.h>
#include <openenclave/bits/safemath.h>
#include <openenclave/corelibc/stdio.h>
#include <openenclave/corelibc/stdlib.h>
#include <openenclave/corelibc/string.h>
#include <openenclave/edger8r/enclave.h>
#include <openenclave/enclave.h>
#include <openenclave/internal/calls.h>
#include <openenclave/internal/print.h>
#include <openenclave/internal/stack_alloc.h>

void* oe_host_calloc(size_t nmemb, size_t size)
{
    oe_calloc_args_t arg_in;
    uint64_t arg_out = 0;

    arg_in.nmemb = nmemb;
    arg_in.size = size;

    if (oe_ocall(
            OE_OCALL_CALLOC,
            (uint64_t)&arg_in,
            sizeof(arg_in),
            true,
            &arg_out,
            sizeof(arg_out)) != OE_OK)
    {
        return NULL;
    }

    if (arg_out && !oe_is_outside_enclave((void*)arg_out, size))
        oe_abort();

    return (void*)arg_out;
}

void* oe_host_realloc(void* ptr, size_t size)
{
    oe_realloc_args_t arg_in;
    uint64_t arg_out = 0;

    arg_in.ptr = ptr;
    arg_in.size = size;

    if (oe_ocall(
            OE_OCALL_REALLOC,
            (uint64_t)&arg_in,
            sizeof(arg_in),
            true,
            &arg_out,
            sizeof(arg_out)) != OE_OK)
    {
        return NULL;
    }

    if (arg_out && !oe_is_outside_enclave((void*)arg_out, size))
        oe_abort();

    return (void*)arg_out;
}

void* oe_host_memset(void* ptr, int value, size_t num)
{
    oe_memset_args_t arg_in;
    uint64_t arg_out = 0;

    arg_in.ptr = ptr;
    arg_in.value = value;
    arg_in.num = num;

    if (oe_ocall(
            OE_OCALL_MEMSET,
            (uint64_t)&arg_in,
            sizeof(arg_in),
            true,
            &arg_out,
            sizeof(arg_out)) != OE_OK)
    {
        return NULL;
    }

    if (arg_out && !oe_is_outside_enclave((void*)arg_out, num))
        oe_abort();

    return (void*)arg_out;
}

char* oe_host_strndup(const char* str, size_t n)
{
    oe_strndup_args_t* arg_in;
    size_t arg_in_sz;
    uint64_t arg_out;
    size_t len;

    if (!str)
        return NULL;

    len = oe_strlen(str);

    if (n < len)
        len = n;

    /* Would be an integer overflow in the next statement. */
    if (len == OE_SIZE_MAX)
        return NULL;

    if (oe_safe_add_sizet(len + 1, sizeof(oe_strndup_args_t), &arg_in_sz) !=
        OE_OK)
        return NULL;

    arg_in = (oe_strndup_args_t*)oe_calloc(1, arg_in_sz);
    if (!arg_in)
        return NULL;

    arg_in->n = len + 1;

    if (oe_memcpy_s(arg_in->str, len + 1, str, len) != OE_OK)
        goto done;

    arg_in->str[len] = '\0';

    if (oe_ocall(
            OE_OCALL_STRNDUP,
            (uint64_t)arg_in,
            arg_in_sz,
            true,
            &arg_out,
            sizeof(arg_out)) != OE_OK)
    {
        arg_out = 0;
        goto done;
    }

    if (arg_out && !oe_is_outside_enclave((void*)arg_out, len + 1))
        oe_abort();

done:
    oe_free(arg_in);
    return (void*)arg_out;
}

int oe_host_write(int device, const char* str, size_t len)
{
    oe_print_args_t* arg_in = NULL;
    size_t arg_in_sz;
    int ret = -1;

    /* Reject invalid arguments */
    if ((device != 0 && device != 1) || !str)
        goto done;

    /* Determine the length of the string */
    if (len == (size_t)-1)
        len = oe_strlen(str);

    /* Check for integer overflow and allocate space for the arguments followed
     * by null-terminated string */
    if (oe_safe_add_sizet(len + 1, sizeof(oe_print_args_t), &arg_in_sz) !=
        OE_OK)
        goto done;

    arg_in = (oe_print_args_t*)oe_calloc(1, arg_in_sz);
    if (!arg_in)
        goto done;

    /* Initialize the arguments */
    arg_in->device = device;

    if (oe_memcpy_s(arg_in->str, len + 1, str, len) != OE_OK)
        goto done;

    arg_in->str[len] = '\0';

    /* Perform OCALL */
    if (oe_ocall(OE_OCALL_WRITE, (uint64_t)arg_in, arg_in_sz, true, NULL, 0) !=
        OE_OK)
        goto done;

    ret = 0;

done:
    oe_free(arg_in);
    return ret;
}

// Function used by oeedger8r for allocating ocall buffers.
//
// TODO: These are allocated inside the TA and subsequently marshalled by
//       oe_ocall via libutee. This means that the arguments structure is
//       needlessly copied twice in OP-TEE.
void* oe_allocate_ocall_buffer(size_t size)
{
    return oe_malloc(size);
}

// Function used by oeedger8r for freeing ocall buffers.
void oe_free_ocall_buffer(void* buffer)
{
    oe_free(buffer);
}