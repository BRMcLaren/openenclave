// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef _VE_SHM_H
#define _VE_SHM_H

#include <openenclave/bits/defs.h>
#include <openenclave/bits/types.h>

#define VE_SHM_RND 020000

void* ve_shmat(int shmid, const void* shmaddr, int shmflg);

int ve_shmdt(const void* shmaddr);

#endif /* _VE_SHM_H */
