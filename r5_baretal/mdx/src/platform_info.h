#ifndef PLATFORM_INFO_H_
#define PLATFORM_INFO_H_


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


#include <openamp/remoteproc.h>
#include <openamp/virtio.h>
#include <openamp/rpmsg.h>

#if defined __cplusplus
extern "C" {
#endif

/* Cortex R5 memory attributes */
#define DEVICE_SHARED       0x00000001U /* device, shareable */
#define DEVICE_NONSHARED    0x00000010U /* device, non shareable */
#define NORM_NSHARED_NCACHE 0x00000008U /* Non cacheable  non shareable */
#define NORM_SHARED_NCACHE  0x0000000CU /* Non cacheable shareable */
#define PRIV_RW_USER_RW     (0x00000003U<<8U) /* Full Access */

/* Interrupt vectors */
#ifdef versal
#define IPI_IRQ_VECT_ID         63
#else
#define IPI_IRQ_VECT_ID         XPAR_XIPIPSU_0_INT_ID
#endif /* versal */

struct remoteproc_priv {
	const char *ipi_name; /**< IPI device name */
	const char *ipi_bus_name; /**< IPI bus name */
	struct metal_device *ipi_dev; /**< pointer to IPI device */
	struct metal_io_region *ipi_io; /**< pointer to IPI i/o region */
	unsigned int ipi_chn_mask; /**< IPI channel mask */
	atomic_int ipi_nokick;
};

/**
 * platform_init - initialize the platform
 *
 * It will initialize the platform.
 *
 * @argc: number of arguments
 * @argv: array of the input arguments
 * @platform: pointer to store the platform data pointer
 *
 * return 0 for success or negative value for failure
 */
int platform_init(int argc, char *argv[], void **platform);

/**
 * platform_create_rpmsg_vdev - create rpmsg vdev
 *
 * It will create rpmsg virtio device, and returns the rpmsg virtio
 * device pointer.
 *
 * @platform: pointer to the private data
 * @vdev_index: index of the virtio device, there can more than one vdev
 *              on the platform.
 * @role: virtio master or virtio slave of the vdev
 * @rst_cb: virtio device reset callback
 * @ns_bind_cb: rpmsg name service bind callback
 *
 * return pointer to the rpmsg virtio device
 */
struct rpmsg_device *
platform_create_rpmsg_vdev(void *platform, unsigned int vdev_index,
			   unsigned int role,
			   void (*rst_cb)(struct virtio_device *vdev),
			   rpmsg_ns_bind_cb ns_bind_cb);

/**
 * platform_poll - platform poll function
 *
 * @platform: pointer to the platform
 *
 * return negative value for errors, otherwise 0.
 */
int platform_poll(void *platform);

/**
 * platform_release_rpmsg_vdev - release rpmsg virtio device
 *
 * @rpdev: pointer to the rpmsg device
 */
void platform_release_rpmsg_vdev(struct rpmsg_device *rpdev);

/**
 * platform_cleanup - clean up the platform resource
 *
 * @platform: pointer to the platform
 */
void platform_cleanup(void *platform);

#if defined __cplusplus
}
#endif

#endif /* PLATFORM_INFO_H_ */
