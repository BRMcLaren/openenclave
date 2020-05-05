// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oe_err.h"

#if defined(WIN32)
#define HAS_ENGINE_SUPPORT 0
#else
#define HAS_ENGINE_SUPPORT 1
#endif

int oedump(const char*);
int oesign(
    const char* enclave,
    const char* conffile,
    const char* keyfile,
    const char* engine_id,
    const char* engine_load_path,
    const char* key_id);

static const char _usage_gen[] =
    "Usage: %s <command> [options]\n"
    "\n"
    "Commands:\n"
    "    sign  -  Sign the specified enclave.\n"
    "    dump  -  Print out the Open Enclave metadata for the specified "
    "enclave.\n"
    "\n"
    "For help with a specific command, enter \"%s <command> --help\"\n";

static const char _usage_sign[] =
    "Usage: %s sign {--enclave-image | -e} ENCLAVE_IMAGE "
    "{--config-file | -c} CONFIG_FILE {--key-file | -k} KEY_FILE\n"
#if HAS_ENGINE_SUPPORT
    "{{--engine| -n} ENGINE_NAME {--load-path | -p } ENGINE_LOAD_PATH "
    "{--key-id | -i } KEY_ID }\n"
#endif
    "\n"
    "Where:\n"
    "    ENCLAVE_IMAGE -- path of an enclave image file\n"
    "    CONFIG_FILE -- configuration file containing enclave properties\n"
    "    KEY_FILE -- private key file used to digitally sign the image\n"
#if HAS_ENGINE_SUPPORT
    "    ENGINE_NAME -- text name of the engine to use, for example 'pkcs-11'\n"
    "    ENGINE_LOADPATH -- absolute path to the shared object which "
    "implements the engine\n"
    "    KEY_ID -- text string specifying the desired key from the engine\n"
#endif
    "\n"
    "Description:\n"
    "    This utility (1) injects runtime properties into an enclave image "
    "and\n"
    "    (2) digitally signs that image.\n"
    "\n"
    "    The properties are read from the CONFIG_FILE. They override any\n"
    "    properties that were already defined inside the enclave image "
    "through\n"
    "    use of the OE_SET_ENCLAVE_SGX macro. These properties include:\n"
    "\n"
    "        Debug - whether enclave debug mode should be enabled (1) or not "
    "(0)\n"
    "        ProductID - the product identified number\n"
    "        SecurityVersion - the security version number\n"
    "        NumHeapPages - the number of heap pages for this enclave\n"
    "        NumStackPages - the number of stack pages for this enclave\n"
    "        NumTCS - the number of thread control structures for this "
    "enclave\n"
    "\n"
    "    The configuration file contains simple NAME=VALUE entries. For "
    "example:\n"
    "\n"
    "        Debug=1\n"
    "        NumHeapPages=1024\n"
    "\n"
    "    If specified, the key read from KEY_FILE and contains a private RSA "
    "key in PEM\n"
    "    format. The keyfile must contain the following header.\n"
    "\n"
    "        -----BEGIN RSA PRIVATE KEY-----\n"
    "\n"
    "    The resulting image is written to ENCLAVE_IMAGE.signed\n"
    "\n"
#if HAS_ENGINE_SUPPORT
    " Keys may also be received from an openssl engine specified by the "
    "string ENGINE_NAME\n"
    " If they are received from an engine, KEY_ID must be specified rather "
    "than KEY_FILE. \n"
#endif
    "\n";

static const char _usage_dump[] =
    "Usage: %s dump {--enclave-image | -e} ENCLAVE_IMAGE\n"
    "\n"
    "Where:\n"
    "    ENCLAVE_IMAGE -- path of an enclave image file\n"
    "\n"
    "Description:\n"
    "    This option dumps the oeinfo and signature information of an "
    "enclave\n";

int dump_parser(int argc, const char* argv[])
{
    int ret = 0;
    const char* enclave = NULL;

    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"enclave-image", required_argument, NULL, 'e'},
        {NULL, 0, NULL, 0},
    };
    const char short_options[] = "he:";

    int c;
    do
    {
        c = getopt_long(
            argc, (char* const*)argv, short_options, long_options, NULL);
        if (c == -1)
        {
            // all the command-line options are parsed
            break;
        }

        switch (c)
        {
            case 'h':
                fprintf(stderr, _usage_dump, argv[0]);
                goto done;
            case 'e':
                enclave = optarg;
                break;
            case ':':
                // Missing option argument
                ret = 1;
                goto done;
            case '?':
            default:
                // Invalid option
                ret = 1;
                goto done;
        }
    } while (1);

    if (enclave == NULL)
    {
        oe_err("--enclave-image option is missing");
        ret = 1;
    }
    if (!ret)
        /* dump oeinfo and signature information */
        ret = oedump(enclave);

done:

    return ret;
}

int sign_parser(int argc, const char* argv[])
{
    int ret = 0;
    const char* enclave = NULL;
    const char* conffile = NULL;
    const char* keyfile = NULL;
#if HAS_ENGINE_SUPPORT
    const char* engine_id = NULL;
    const char* engine_load_path = NULL;
    const char* key_id = NULL;
#endif

    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"enclave-image", required_argument, NULL, 'e'},
        {"config-file", required_argument, NULL, 'c'},
        {"key-file", required_argument, NULL, 'k'},
#if HAS_ENGINE_SUPPORT
        {"engine", required_argument, NULL, 'n'},
        {"load-path", required_argument, NULL, 'p'},
        {"key-id", required_argument, NULL, 'i'},
#endif
        {NULL, 0, NULL, 0},
    };
    const char short_options[] = "he:c:k:n:p:i:";

    int c;

    if (argc <= 2)
    {
        fprintf(stderr, _usage_sign, argv[0]);
        ret = 1;
        goto done;
    }

    do
    {
        c = getopt_long(
            argc, (char* const*)argv, short_options, long_options, NULL);
        if (c == -1)
        {
            // all the command-line options are parsed
            break;
        }

        switch (c)
        {
            case 'h':
                fprintf(stderr, _usage_sign, argv[0]);
                goto done;
            case 'e':
                enclave = optarg;
                break;
            case 'c':
                conffile = optarg;
                break;
            case 'k':
                keyfile = optarg;
                break;
#if HAS_ENGINE_SUPPORT
            case 'n':
                engine_id = optarg;
                break;
            case 'p':
                engine_load_path = optarg;
                break;
            case 'i':
                key_id = optarg;
                break;
#endif
            case ':':
                // Missing option argument
                ret = 1;
                goto done;
            case '?':
            default:
                // Invalid option
                ret = 1;
                goto done;
        }
    } while (1);

    if (conffile == NULL)
    {
        oe_err("--config-file option is missing");
        ret = 1;
    }

#if HAS_ENGINE_SUPPORT
    if (keyfile)
    {
        if (engine_id || engine_load_path || key_id)
        {
            oe_err("--key-file cannot be used with engine options");
            ret = 1;
            goto done;
        }
    }
    else
    {
        if (!engine_id || !key_id)
        {
            oe_err("Either --key-file or --key-id and its --engine must be "
                   "specified");
            ret = 1;
            goto done;
        }
    }
    if (!ret)
    {
        ret = oesign(
            enclave, conffile, keyfile, engine_id, engine_load_path, key_id);
    }
#else
    if (keyfile == NULL)
    {
        oe_err("--key-file option is missing");
        ret = 1;
        goto done;
    }
    if (!ret)
    {
        ret = oesign(enclave, conffile, keyfile, NULL, NULL, NULL);
    }
#endif

done:

    return ret;
}

int arg_handler(int argc, const char* argv[])
{
    int ret = 1;
    if ((strcmp(argv[1], "dump") == 0))
        ret = dump_parser(argc, argv);
    else if ((strcmp(argv[1], "sign") == 0))
        ret = sign_parser(argc, argv);
    else
    {
        fprintf(stderr, _usage_gen, argv[0], argv[0]);
        exit(1);
    }
    return ret;
}

int main(int argc, const char* argv[])
{
    oe_set_err_program_name(argv[0]);
    int ret = 1;

    if (argc < 2)
    {
        fprintf(stderr, _usage_gen, argv[0], argv[0]);
        exit(1);
    }

    ret = arg_handler(argc, argv);
    return ret;
}
