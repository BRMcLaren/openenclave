// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

extern int __ve_pid;

const char* ve_func_name(ve_func_t func)
{
    switch (func)
    {
        case VE_FUNC_RET:
            return "RET";
        case VE_FUNC_ERR:
            return "ERR";
        case VE_FUNC_INIT:
            return "INIT";
        case VE_FUNC_ADD_THREAD:
            return "ADD_THREAD";
        case VE_FUNC_TERMINATE_THREAD:
            return "TERMINATE_THREAD";
        case VE_FUNC_TERMINATE:
            return "TERMINATE";
        case VE_FUNC_PING:
            return "PING";
        case VE_FUNC_MALLOC:
            return "MALLOC";
        case VE_FUNC_CALLOC:
            return "CALLOC";
        case VE_FUNC_REALLOC:
            return "REALLOC";
        case VE_FUNC_MEMALIGN:
            return "MEMALIGN";
        case VE_FUNC_FREE:
            return "FREE";
        case VE_FUNC_GET_SETTINGS:
            return "GET_SETTINGS";
        case VE_FUNC_XOR:
            return "XOR";
        case VE_FUNC_INIT_ENCLAVE:
            return "INIT_ENCLAVE";
        case VE_FUNC_CALL_ENCLAVE_FUNCTION:
            return "VE_FUNC_CALL_ENCLAVE_FUNCTION";
        case VE_FUNC_CALL_HOST_FUNCTION:
            return "VE_FUNC_CALL_HOST_FUNCTION";
    }

    return "UNKNOWN";
}

int ve_call_send(
    int fd,
    uint64_t func,
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5,
    uint64_t arg6)
{
    int ret = -1;

    if (fd < 0)
        goto done;

    /* Send request. */
    {
        ve_call_buf_t buf;

        buf.func = func;
        buf.retval = 0;
        buf.arg1 = arg1;
        buf.arg2 = arg2;
        buf.arg3 = arg3;
        buf.arg4 = arg4;
        buf.arg5 = arg5;
        buf.arg6 = arg6;

        if (ve_writen(fd, &buf, sizeof(buf)) != 0)
            goto done;
    }

    ret = 0;

done:
    return ret;
}

int ve_call(
    int fd,
    uint64_t func,
    uint64_t* retval,
    uint64_t arg1,
    uint64_t arg2,
    uint64_t arg3,
    uint64_t arg4,
    uint64_t arg5,
    uint64_t arg6)
{
    int ret = -1;

    if (ve_call_send(fd, func, arg1, arg2, arg3, arg4, arg5, arg6) != 0)
        goto done;

    if (ve_call_recv(fd, retval) != 0)
        goto done;

    ret = 0;

done:
    return ret;
}