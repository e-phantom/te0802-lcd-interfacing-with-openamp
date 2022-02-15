/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * All rights reserved.
 *
 * Copyright (C) 2015 Xilinx, Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* This file populates resource table for BM remote
 * for use by the Linux Master */

#ifndef RSC_TABLE_H_
#define RSC_TABLE_H_



#include <atomic>
using std::atomic_flag_clear;
using std::atomic_flag_test_and_set;

using std::atomic_load;
using std::atomic_load_explicit;
using std::atomic_store_explicit;

using std::atomic_ushort;
using std::atomic_uchar;
using std::atomic_ulong;
using std::atomic_ullong;
using std::memory_order;
using std::atomic_uint;
typedef int atomic_int;

#define atomic_store(OBJ, VAL)						\
	do { *(OBJ) = (VAL); __sync_synchronize(); } while (0)

#define atomic_flag_test_and_set(FLAG)					\
	__sync_lock_test_and_set((FLAG), 1)

#define atomic_flag_clear(FLAG)						\
	__sync_lock_release((FLAG))

#define atomic_load(OBJ)						\
	({ __sync_synchronize(); *(OBJ); })



#include <stddef.h>
#include <openamp/open_amp.h>

#if defined __cplusplus
extern "C" {
#endif

#define NO_RESOURCE_ENTRIES         8

/* Resource table for the given remote */
struct remote_resource_table {
	unsigned int version;
	unsigned int num;
	unsigned int reserved[2];
	unsigned int offset[NO_RESOURCE_ENTRIES];
	/* rpmsg vdev entry */
	struct fw_rsc_vdev rpmsg_vdev;
	struct fw_rsc_vdev_vring rpmsg_vring0;
	struct fw_rsc_vdev_vring rpmsg_vring1;
}__attribute__((packed, aligned(0x100)));

void *get_resource_table (int rsc_id, int *len);

#if defined __cplusplus
}
#endif

#endif /* RSC_TABLE_H_ */
