/*
 *  This file is auto generated by oeedger8r. DO NOT EDIT.
 */
#include "switchless_t.h"

#include <openenclave/edger8r/enclave.h>

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

OE_EXTERNC_BEGIN

/**** ECALL functions. ****/

void ecall_enclave_add_N_switchless(
    uint8_t* input_buffer,
    size_t input_buffer_size,
    uint8_t* output_buffer,
    size_t output_buffer_size,
    size_t* output_bytes_written)
{
    oe_result_t _result = OE_FAILURE;

    /* Prepare parameters. */
    enclave_add_N_switchless_args_t* pargs_in = (enclave_add_N_switchless_args_t*)input_buffer;
    enclave_add_N_switchless_args_t* pargs_out = (enclave_add_N_switchless_args_t*)output_buffer;

    size_t input_buffer_offset = 0;
    size_t output_buffer_offset = 0;
    OE_ADD_SIZE(input_buffer_offset, sizeof(*pargs_in));
    OE_ADD_SIZE(output_buffer_offset, sizeof(*pargs_out));

    /* Make sure input and output buffers lie within the enclave. */
    if (!input_buffer || !oe_is_within_enclave(input_buffer, input_buffer_size))
        goto done;

    if (!output_buffer || !oe_is_within_enclave(output_buffer, output_buffer_size))
        goto done;

    /* Set in and in-out pointers. */
    if (pargs_in->m)
        OE_SET_IN_OUT_POINTER(m, sizeof(int), int*);

    /* Set out and in-out pointers. */
    /* In-out parameters are copied to output buffer. */
    if (pargs_in->m)
        OE_COPY_AND_SET_IN_OUT_POINTER(m, sizeof(int), int*);

    /* Check that in/in-out strings are null terminated. */
    /* There were no in nor in-out string parameters. */

    /* lfence after checks. */
    oe_lfence();

    /* Call user function. */
    enclave_add_N_switchless(
        pargs_in->m,
        pargs_in->n);

    /* Success. */
    _result = OE_OK;
    *output_bytes_written = output_buffer_offset;

done:
    if (pargs_out && output_buffer_size >= sizeof(*pargs_out))
        pargs_out->_result = _result;
}

void ecall_enclave_add_N_regular(
    uint8_t* input_buffer,
    size_t input_buffer_size,
    uint8_t* output_buffer,
    size_t output_buffer_size,
    size_t* output_bytes_written)
{
    oe_result_t _result = OE_FAILURE;

    /* Prepare parameters. */
    enclave_add_N_regular_args_t* pargs_in = (enclave_add_N_regular_args_t*)input_buffer;
    enclave_add_N_regular_args_t* pargs_out = (enclave_add_N_regular_args_t*)output_buffer;

    size_t input_buffer_offset = 0;
    size_t output_buffer_offset = 0;
    OE_ADD_SIZE(input_buffer_offset, sizeof(*pargs_in));
    OE_ADD_SIZE(output_buffer_offset, sizeof(*pargs_out));

    /* Make sure input and output buffers lie within the enclave. */
    if (!input_buffer || !oe_is_within_enclave(input_buffer, input_buffer_size))
        goto done;

    if (!output_buffer || !oe_is_within_enclave(output_buffer, output_buffer_size))
        goto done;

    /* Set in and in-out pointers. */
    if (pargs_in->m)
        OE_SET_IN_OUT_POINTER(m, sizeof(int), int*);

    /* Set out and in-out pointers. */
    /* In-out parameters are copied to output buffer. */
    if (pargs_in->m)
        OE_COPY_AND_SET_IN_OUT_POINTER(m, sizeof(int), int*);

    /* Check that in/in-out strings are null terminated. */
    /* There were no in nor in-out string parameters. */

    /* lfence after checks. */
    oe_lfence();

    /* Call user function. */
    enclave_add_N_regular(
        pargs_in->m,
        pargs_in->n);

    /* Success. */
    _result = OE_OK;
    *output_bytes_written = output_buffer_offset;

done:
    if (pargs_out && output_buffer_size >= sizeof(*pargs_out))
        pargs_out->_result = _result;
}

/**** ECALL function table. ****/

oe_ecall_func_t __oe_ecalls_table[] = {
    (oe_ecall_func_t) ecall_enclave_add_N_switchless,
    (oe_ecall_func_t) ecall_enclave_add_N_regular
};

size_t __oe_ecalls_table_size = OE_COUNTOF(__oe_ecalls_table);

/**** OCALL function wrappers. ****/

oe_result_t host_increment_switchless(int* m)
{
    oe_result_t _result = OE_FAILURE;

    /* If the enclave is in crashing/crashed status, new OCALL should fail
       immediately. */
    if (oe_get_enclave_status() != OE_OK)
        return oe_get_enclave_status();

    /* Marshalling struct. */
    host_increment_switchless_args_t _args, *_pargs_in = NULL, *_pargs_out = NULL;
    /* No pointers to save for deep copy. */

    /* Marshalling buffer and sizes. */
    size_t _input_buffer_size = 0;
    size_t _output_buffer_size = 0;
    size_t _total_buffer_size = 0;
    uint8_t* _buffer = NULL;
    uint8_t* _input_buffer = NULL;
    uint8_t* _output_buffer = NULL;
    size_t _input_buffer_offset = 0;
    size_t _output_buffer_offset = 0;
    size_t _output_bytes_written = 0;

    /* Fill marshalling struct. */
    memset(&_args, 0, sizeof(_args));
    _args.m = (int*)m;

    /* Compute input buffer size. Include in and in-out parameters. */
    OE_ADD_SIZE(_input_buffer_size, sizeof(host_increment_switchless_args_t));
    if (m)
        OE_ADD_SIZE(_input_buffer_size, sizeof(int));
    
    /* Compute output buffer size. Include out and in-out parameters. */
    OE_ADD_SIZE(_output_buffer_size, sizeof(host_increment_switchless_args_t));
    if (m)
        OE_ADD_SIZE(_output_buffer_size, sizeof(int));
    
    /* Allocate marshalling buffer. */
    _total_buffer_size = _input_buffer_size;
    OE_ADD_SIZE(_total_buffer_size, _output_buffer_size);
    _buffer = (uint8_t*)oe_allocate_switchless_ocall_buffer(_total_buffer_size);
    _input_buffer = _buffer;
    _output_buffer = _buffer + _input_buffer_size;
    if (_buffer == NULL)
    {
        _result = OE_OUT_OF_MEMORY;
        goto done;
    }
    
    /* Serialize buffer inputs (in and in-out parameters). */
    _pargs_in = (host_increment_switchless_args_t*)_input_buffer;
    OE_ADD_SIZE(_input_buffer_offset, sizeof(*_pargs_in));
    if (m)
        OE_WRITE_IN_OUT_PARAM(m, sizeof(int), int*);
    
    /* Copy args structure (now filled) to input buffer. */
    memcpy(_pargs_in, &_args, sizeof(*_pargs_in));

    /* Call host function. */
    if ((_result = oe_switchless_call_host_function(
             switchless_fcn_id_host_increment_switchless,
             _input_buffer,
             _input_buffer_size,
             _output_buffer,
             _output_buffer_size,
             &_output_bytes_written)) != OE_OK)
        goto done;

    /* Setup output arg struct pointer. */
    _pargs_out = (host_increment_switchless_args_t*)_output_buffer;
    OE_ADD_SIZE(_output_buffer_offset, sizeof(*_pargs_out));
    
    /* Check if the call succeeded. */
    if ((_result = _pargs_out->_result) != OE_OK)
        goto done;
    
    /* Currently exactly _output_buffer_size bytes must be written. */
    if (_output_bytes_written != _output_buffer_size)
    {
        _result = OE_FAILURE;
        goto done;
    }
    
    /* Unmarshal return value and out, in-out parameters. */
    /* No return value. */
    /* No pointers to restore for deep copy. */
    OE_READ_IN_OUT_PARAM(m, (size_t)(sizeof(int)));

    /* Retrieve propagated errno from OCALL. */
    /* Errno propagation not enabled. */

    _result = OE_OK;

done:
    if (_buffer)
        oe_free_switchless_ocall_buffer(_buffer);
    return _result;
}

oe_result_t host_increment_regular(int* m)
{
    oe_result_t _result = OE_FAILURE;

    /* If the enclave is in crashing/crashed status, new OCALL should fail
       immediately. */
    if (oe_get_enclave_status() != OE_OK)
        return oe_get_enclave_status();

    /* Marshalling struct. */
    host_increment_regular_args_t _args, *_pargs_in = NULL, *_pargs_out = NULL;
    /* No pointers to save for deep copy. */

    /* Marshalling buffer and sizes. */
    size_t _input_buffer_size = 0;
    size_t _output_buffer_size = 0;
    size_t _total_buffer_size = 0;
    uint8_t* _buffer = NULL;
    uint8_t* _input_buffer = NULL;
    uint8_t* _output_buffer = NULL;
    size_t _input_buffer_offset = 0;
    size_t _output_buffer_offset = 0;
    size_t _output_bytes_written = 0;

    /* Fill marshalling struct. */
    memset(&_args, 0, sizeof(_args));
    _args.m = (int*)m;

    /* Compute input buffer size. Include in and in-out parameters. */
    OE_ADD_SIZE(_input_buffer_size, sizeof(host_increment_regular_args_t));
    if (m)
        OE_ADD_SIZE(_input_buffer_size, sizeof(int));
    
    /* Compute output buffer size. Include out and in-out parameters. */
    OE_ADD_SIZE(_output_buffer_size, sizeof(host_increment_regular_args_t));
    if (m)
        OE_ADD_SIZE(_output_buffer_size, sizeof(int));
    
    /* Allocate marshalling buffer. */
    _total_buffer_size = _input_buffer_size;
    OE_ADD_SIZE(_total_buffer_size, _output_buffer_size);
    _buffer = (uint8_t*)oe_allocate_ocall_buffer(_total_buffer_size);
    _input_buffer = _buffer;
    _output_buffer = _buffer + _input_buffer_size;
    if (_buffer == NULL)
    {
        _result = OE_OUT_OF_MEMORY;
        goto done;
    }
    
    /* Serialize buffer inputs (in and in-out parameters). */
    _pargs_in = (host_increment_regular_args_t*)_input_buffer;
    OE_ADD_SIZE(_input_buffer_offset, sizeof(*_pargs_in));
    if (m)
        OE_WRITE_IN_OUT_PARAM(m, sizeof(int), int*);
    
    /* Copy args structure (now filled) to input buffer. */
    memcpy(_pargs_in, &_args, sizeof(*_pargs_in));

    /* Call host function. */
    if ((_result = oe_call_host_function(
             switchless_fcn_id_host_increment_regular,
             _input_buffer,
             _input_buffer_size,
             _output_buffer,
             _output_buffer_size,
             &_output_bytes_written)) != OE_OK)
        goto done;

    /* Setup output arg struct pointer. */
    _pargs_out = (host_increment_regular_args_t*)_output_buffer;
    OE_ADD_SIZE(_output_buffer_offset, sizeof(*_pargs_out));
    
    /* Check if the call succeeded. */
    if ((_result = _pargs_out->_result) != OE_OK)
        goto done;
    
    /* Currently exactly _output_buffer_size bytes must be written. */
    if (_output_bytes_written != _output_buffer_size)
    {
        _result = OE_FAILURE;
        goto done;
    }
    
    /* Unmarshal return value and out, in-out parameters. */
    /* No return value. */
    /* No pointers to restore for deep copy. */
    OE_READ_IN_OUT_PARAM(m, (size_t)(sizeof(int)));

    /* Retrieve propagated errno from OCALL. */
    /* Errno propagation not enabled. */

    _result = OE_OK;

done:
    if (_buffer)
        oe_free_ocall_buffer(_buffer);
    return _result;
}

OE_EXTERNC_END
