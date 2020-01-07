/*
 *  This file is auto generated by oeedger8r. DO NOT EDIT.
 */
#ifndef EDGER8R_DATASEALING_U_H
#define EDGER8R_DATASEALING_U_H

#include <openenclave/host.h>

#include "datasealing_args.h"

OE_EXTERNC_BEGIN

oe_result_t oe_create_datasealing_enclave(
    const char* path,
    oe_enclave_type_t type,
    uint32_t flags,
    const oe_enclave_setting_t* settings,
    uint32_t setting_count,
    oe_enclave_t** enclave);

/**** ECALL prototypes. ****/
oe_result_t seal_data(
    oe_enclave_t* enclave,
    int* _retval,
    int sealPolicy,
    unsigned char* opt_mgs,
    size_t opt_msg_len,
    unsigned char* data,
    size_t data_size,
    sealed_data_t** sealed_data,
    size_t* sealed_data_size);

oe_result_t unseal_data(
    oe_enclave_t* enclave,
    int* _retval,
    sealed_data_t* sealed_data,
    size_t sealed_data_size,
    unsigned char** data,
    size_t* data_size);

/**** OCALL prototypes. ****/
/* There were no ocalls. */

OE_EXTERNC_END

#endif // EDGER8R_DATASEALING_U_H
