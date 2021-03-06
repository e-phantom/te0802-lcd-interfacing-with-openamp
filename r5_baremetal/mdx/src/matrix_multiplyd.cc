/* This is a sample demonstration application that showcases usage of remoteproc
and rpmsg APIs on the remote core. This application is meant to run on the remote CPU
running baremetal code. This applicationr receives two matrices from the master,
multiplies them and returns the result to the master core. */

#include "xil_printf.h"
#include <stdlib.h>
#include <string>

#include "MyDisp.h"
#include "mtds.h"
#include "display_functions.h"

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


#include <openamp/open_amp.h>
#include "matrix_multiply.h"
#include "platform_info.h"

#define	MAX_SIZE		6
#define NUM_MATRIX		2

#define SHUTDOWN_MSG	0xEF56A55A

#define LPRINTF(format, ...) xil_printf(format, ##__VA_ARGS__)
#define LPERROR(format, ...) LPRINTF("ERROR: " format, ##__VA_ARGS__)

typedef struct _matrix {
	unsigned int size;
	unsigned int elements[MAX_SIZE][MAX_SIZE];
} matrix;

/* Local variables */
static struct rpmsg_endpoint lept;
static int shutdown_req = 0;

/* External functions */
extern int init_system(void);
extern void cleanup_system(void);


/*-----------------------------------------------------------------------------*
 *  RPMSG callbacks setup by remoteproc_resource_init()
 *-----------------------------------------------------------------------------*/
static int rpmsg_endpoint_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
			     uint32_t src, void *priv)
{

	(void)priv;
	(void)src;
	bool * ret;

	if ((*(unsigned int *)data) == SHUTDOWN_MSG) {
		LPRINTF("shutdown message is received.\n");
		shutdown_req = 1;
		return RPMSG_SUCCESS;
	}

	/* Process received data and multiple matrices. */

	char* new_data = (char *) data;
	std::string args (new_data);
	std::vector <std::string> out;

	if(strlen(new_data)>0){

		tokenize_function(args, out);
		function_call(out[0], out[1], ret);

	}else{
		LPRINTF("\nwrong display string");
	}
	/* Send the result of matrix multiplication back to master. */
	if (rpmsg_send(ept, ret, sizeof(ret)) < 0) {
		LPERROR("rpmsg_send failed\n");
	}
	return RPMSG_SUCCESS;
}

static void rpmsg_service_unbind(struct rpmsg_endpoint *ept)
{
	(void)ept;
	LPERROR("Endpoint is destroyed\n");
	shutdown_req = 1;
}

/*-----------------------------------------------------------------------------*
 *  Application
 *-----------------------------------------------------------------------------*/
int app(struct rpmsg_device *rdev, void *priv)
{
	int ret;

	ret = rpmsg_create_ept(&lept, rdev, RPMSG_SERVICE_NAME,
				   0, RPMSG_ADDR_ANY, rpmsg_endpoint_cb,
				   rpmsg_service_unbind);
	if (ret) {
		LPERROR("Failed to create endpoint.\n");
		return -1;
	}

	LPRINTF("Waiting for events...\n");
	while(1) {
		platform_poll(priv);
		/* we got a shutdown request, exit */
		if (shutdown_req) {
			break;
		}
	}
	rpmsg_destroy_ept(&lept);

	return 0;
}

/*-----------------------------------------------------------------------------*
 *  Application entry point
 *-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	void *platform;
	struct rpmsg_device *rpdev;
	int ret;

	LPRINTF("Starting application v6--------------------...\n");

	/* Initialize platform */
	ret = platform_init(argc, argv, &platform);

	if (function_map() == false){
		LPRINTF("function call not successful");
	}

	if (ret) {
		LPERROR("Failed to initialize platform.\n");
		ret = -1;
	} else {
		rpdev = platform_create_rpmsg_vdev(platform, 0,
						   VIRTIO_DEV_SLAVE,
						   NULL, NULL);
		if (!rpdev) {
			LPERROR("Failed to create rpmsg virtio device.\n");
			ret = -1;
		} else {
			app(rpdev, platform);
			platform_release_rpmsg_vdev(rpdev);
			ret = 0;
		}
	}

	LPRINTF("Stopping application...\n");
	platform_cleanup(platform);

	return ret;
}
