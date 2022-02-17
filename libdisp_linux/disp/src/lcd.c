/*
 * Sample demo application that showcases inter processor
 * communication from linux userspace to a remote software
 * context. The application generates random matrices and
 * transmits them to the remote context over rpmsg. The
 * remote application performs multiplication of matrices
 * and transmits the results back to this application.
 */
#include "lcd.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <linux/rpmsg.h>
#include <strings.h>

#define WIDTH 320
#define HEI 240

#define RPMSG_BUS_SYS "/sys/bus/rpmsg"
#define RPMSG_GET_KFIFO_SIZE 1
#define RPMSG_GET_FREE_SPACE 3


static int charfd = -1, fd, compute_flag;

void clearDisplay(){
	char  args[50];
	sprintf(args,"clear_display-%ld;0",clrBlack);
	function_call(args);
}
void 		displayOn(bool fEn){
	char  args[50];
	sprintf(args,"display_on-%s;0",fEn);
	function_call(args);
}

void 		setBacklight(int val){
	char  args[50];
	sprintf(args,"set_backlight-%d;0",val);
	function_call(args);
}
//HDS			displayHds();
void 		setPen(unsigned int pen){
	char  args[50];
	sprintf(args,"set_pen-%d;0",pen);
	function_call(args);
}
void 		setForeground(unsigned int clr){
	char  args[50];
	sprintf(args,"set_foreground-%d;0",clr);
	function_call(args);
}
void 		setBackground(unsigned int clr){
	char  args[50];
	sprintf(args,"set_background-%d;0",clr);
	function_call(args);
}
void 		setTransparency(bool fTrans, unsigned int clr){
	char  args[50];
	sprintf(args,"set_transparency-%;%d",fTrans,clr);
	function_call(args);
}
void 		setIntensity(int ity){
	char  args[50];
	sprintf(args,"set_intensity-%d;0",ity);
	function_call(args);
}

void 		setFont(int idFont){
	char  args[50];
	sprintf(args,"set_font-%d;0",idFont);
	function_call(args);
}
unsigned int		loadImage(char * name){
	char  args[50];
	sprintf(args,"load_image-%s;0",name);
	function_call(args);
	return -1;
}
void 		deleteImage(unsigned int hbmp){
	char  args[50];
	sprintf(args,"delete_image-%d;0",hbmp);
	function_call(args);

}

/* Basic graphics drawing functions.
*/
void 		setPixel(int x, int y, unsigned int clr){
	char  args[50];
	sprintf(args,"set_pixel-%d;%d;%ld",x,y,clr);
	function_call(args);

}
unsigned int	getPixel(int x, int y){
	char  args[50];
	sprintf(args,"get_pixel-%d;%d",x,y);
	function_call(args);
	return -1;
}
void 		drawLine(int x1, int y1, int x2, int y2){
	char  args[50];
	sprintf(args,"draw_line-%d;%d;%d;%d",x1,y1,x2,y2);
	function_call(args);

}
void 		drawArc(int x1, int y1, int x2, int y2,	int xr1, int yr1, int xr2, int yr2){
	char  args[50];
	sprintf(args,"draw_arc-%d;%d,%d,%d,%d;%d,%d,%d",x1,y1,x2,y2,xr1, yr1,xr2,yr2);
	function_call(args);

}
void 		drawRectangle(bool fill, int x1, int y1, int x2, int y2){
	char  args[50];
	sprintf(args,"draw_rectangle-%d;%d;%d;%d;%d",fill,x1,y1,x2,y2);
	function_call(args);
}
void 		drawEllipse(bool fill, int x1, int y1, int x2, int y2){
	char  args[50];
	sprintf(args,"draw_ellipse-%d;%d;%d;%d;%d",fill,x1,y1,x2,y2);
	function_call(args);
}
void 		drawImage(char * name, int x, int y){
	char  args[50];
	sprintf(args,"draw_image-%s;%d;%d",name,x,y);
	function_call(args);

}
//void 		drawImage(HBMP hbmp, int x, int y);
void 		drawText(char * txt, int x, int y){
	char  args[50];
	sprintf(args,"draw_text-%s;%d;%d",txt,x,y);
	function_call(args);

}

/* User interface management fuctions.
*/
void 		createButton(int id, char * name, int x, int y){
	char  args[50];
	sprintf(args,"create_button-%d;%s;%d;%d",id,name,x,y);
	function_call(args);
}
//void 		createButton(int id, char * name1, char * name2, int x, int y);
//void 		createButton(int id, HBMP hbmp1, HBMP hbmp2, int x, int y);
void 		deleteButton(int id){
	char  args[50];
	sprintf(args,"delete_button-%d;0",id);
	function_call(args);
}
void 		enableButton(int id, bool fEn){
	char  args[50];
	sprintf(args,"enable_button-%d;%d",id,fEn);
	function_call(args);
}
void 		drawButton(int id, int st){
	char  args[50];
	sprintf(args,"delete_button-%d;%d",id,st);
	function_call(args);
}
void 		isEnabled(int id){
	char  args[50];
	sprintf(args,"is_enabled-%d;0",id);
	function_call(args);
}
void 		isTouched(int id){
	char  args[50];
	sprintf(args,"is_touched-%d;0",id);
	function_call(args);
}
//void 		getFinger(int num, MDFNG * fng);
void 		getButton();
void		checkTouch();

int get_response()
{
	int bytes_rcvd;
	char buff[64] = "";
	short count = 0;
	while (compute_flag == 1 && count < 2) {
		bytes_rcvd = read(fd, buff, sizeof(buff));
		if (bytes_rcvd>0){
			printf("\r\n Received results! - %d bytes from ", bytes_rcvd);
			printf("\r\n Received results! - %s from ", buff);
			printf("rpmsg device (transmitted from remote context) \r\n");
			compute_flag =0;
		}else{
			count ++;
			usleep(100);
		}
	}
	compute_flag =0;
	return 0;
}
int send_message(char * args){
	int size = strlen(args)+1;
	write(fd, args, size);
	compute_flag = 1;

	return 0;
}
int function_call(char * args){

    // write message to the remote processor
	int send = send_message(args);

	// wait for the reply
	int receive = get_response();

	if(send ==0 && receive ==0){
		return 0;
	}else{
		return -1;
	}

}

void setup(){
	char  args[50];
	sprintf(args,"begin-0;0");
	int fStat;
	   while (1) {

		  fStat = function_call(args);
	      if (fStat==0) {
	         printf("mydisp.begin() succeeded\n\r");
	         break;
	      }
	      else {
	         printf("mydisp.begin() failed\n\r");
	         sleep(1);
	      }
	   }
	   printf("\n function call successful");
}


void loop(){
	static int x=HEI/2;
	int y=0;

	short phase = 0;

	while(y<WIDTH){

		switch (phase){
			case 1:
				// the increment phase
				x+=23;
				if(x>HEI)
					phase =2;
			break;
			case 2:
				// the decrement phase
				x-=3;
				if(x<0)
					phase = 1;
			break;
			default:
				phase=1;
			break;
		}
		y++;

		char  args[50];
		sprintf(args,"set_pixel-%d;%d;%ld",x,y,clrCyan);
		printf("args %s",args);
		function_call(args);

		usleep(10000);
	}
}

int rpmsg_create_ept(int rpfd, struct rpmsg_endpoint_info *eptinfo)
{
	int ret;

	ret = ioctl(rpfd, RPMSG_CREATE_EPT_IOCTL, eptinfo);
	if (ret)
		perror("Failed to create endpoint.\n");
	return ret;
}

static char *get_rpmsg_ept_dev_name(const char *rpmsg_char_name,
				    const char *ept_name,
				    char *ept_dev_name)
{
	char sys_rpmsg_ept_name_path[64];
	char svc_name[64];
	char *sys_rpmsg_path = "/sys/class/rpmsg";
	FILE *fp;
	int i;
	int ept_name_len;

	for (i = 0; i < 128; i++) {
		sprintf(sys_rpmsg_ept_name_path, "%s/%s/rpmsg%d/name",
			sys_rpmsg_path, rpmsg_char_name, i);
		printf("checking %s\n", sys_rpmsg_ept_name_path);
		if (access(sys_rpmsg_ept_name_path, F_OK) < 0)
			continue;
		fp = fopen(sys_rpmsg_ept_name_path, "r");
		if (!fp) {
			printf("failed to open %s\n", sys_rpmsg_ept_name_path);
			break;
		}
		fgets(svc_name, sizeof(svc_name), fp);
		fclose(fp);
		printf("svc_name: %s.\n",svc_name);
		ept_name_len = strlen(ept_name);
		if (ept_name_len > sizeof(svc_name))
			ept_name_len = sizeof(svc_name);
		if (!strncmp(svc_name, ept_name, ept_name_len)) {
			sprintf(ept_dev_name, "rpmsg%d", i);
			return ept_dev_name;
		}
	}

	printf("Not able to RPMsg endpoint file for %s:%s.\n",
	       rpmsg_char_name, ept_name);
	return NULL;
}

static int bind_rpmsg_chrdev(const char *rpmsg_dev_name)
{
	char fpath[256];
	char *rpmsg_chdrv = "rpmsg_chrdev";
	int fd;
	int ret;

	/* rpmsg dev overrides path */
	sprintf(fpath, "%s/devices/%s/driver_override",
		RPMSG_BUS_SYS, rpmsg_dev_name);
	fd = open(fpath, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open %s, %s\n",
			fpath, strerror(errno));
		return -EINVAL;
	}
	ret = write(fd, rpmsg_chdrv, strlen(rpmsg_chdrv) + 1);
	if (ret < 0) {
		fprintf(stderr, "Failed to write %s to %s, %s\n",
			rpmsg_chdrv, fpath, strerror(errno));
		return -EINVAL;
	}
	close(fd);

	/* bind the rpmsg device to rpmsg char driver */
	sprintf(fpath, "%s/drivers/%s/bind", RPMSG_BUS_SYS, rpmsg_chdrv);
	fd = open(fpath, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open %s, %s\n",
			fpath, strerror(errno));
		return -EINVAL;
	}
	ret = write(fd, rpmsg_dev_name, strlen(rpmsg_dev_name) + 1);
	if (ret < 0) {
		fprintf(stderr, "Failed to write %s to %s, %s\n",
			rpmsg_dev_name, fpath, strerror(errno));
		return -EINVAL;
	}
	close(fd);
	return 0;
}

static int get_rpmsg_chrdev_fd(const char *rpmsg_dev_name,
			       char *rpmsg_ctrl_name)
{
	char dpath[256];
	char fpath[256];
	char *rpmsg_ctrl_prefix = "rpmsg_ctrl";
	DIR *dir;
	struct dirent *ent;
	int fd;

	sprintf(dpath, "%s/devices/%s/rpmsg", RPMSG_BUS_SYS, rpmsg_dev_name);
	dir = opendir(dpath);
	if (dir == NULL) {
		fprintf(stderr, "Failed to open dir %s\n", dpath);
		return -EINVAL;
	}
	while ((ent = readdir(dir)) != NULL) {
		if (!strncmp(ent->d_name, rpmsg_ctrl_prefix,
			    strlen(rpmsg_ctrl_prefix))) {
			printf("Opening file %s.\n", ent->d_name);
			sprintf(fpath, "/dev/%s", ent->d_name);
			fd = open(fpath, O_RDWR | O_NONBLOCK);
			if (fd < 0) {
				fprintf(stderr,
					"Failed to open rpmsg char dev %s,%s\n",
					fpath, strerror(errno));
				return fd;
			}
			sprintf(rpmsg_ctrl_name, "%s", ent->d_name);
			return fd;
		}
	}

	fprintf(stderr, "No rpmsg char dev file is found\n");
	return -EINVAL;
}

int start_app()
{
	unsigned int size;
	int opt, ret;
	char *rpmsg_dev="virtio0.rpmsg-openamp-demo-channel.-1.0";
	char rpmsg_char_name[16];
	char fpath[256];
	struct rpmsg_endpoint_info eptinfo;
	char ept_dev_name[16];
	char ept_dev_path[32];


	/* Load rpmsg_char driver */
	printf("\r\nMaster>probe rpmsg_char\r\n");
	ret = system("modprobe rpmsg_char");
	if (ret < 0) {
		perror("Failed to load rpmsg_char driver.\n");
		return -EINVAL;
	}

	printf("\r\n Open rpmsg dev %s! \r\n", rpmsg_dev);
	sprintf(fpath, "%s/devices/%s", RPMSG_BUS_SYS, rpmsg_dev);
	if (access(fpath, F_OK)) {
		fprintf(stderr, "Not able to access rpmsg device %s, %s\n",
			fpath, strerror(errno));
		return -EINVAL;
	}
	ret = bind_rpmsg_chrdev(rpmsg_dev);
	if (ret < 0)
		return ret;
	charfd = get_rpmsg_chrdev_fd(rpmsg_dev, rpmsg_char_name);
	if (charfd < 0)
		return charfd;

	/* Create endpoint from rpmsg char driver */
	strcpy(eptinfo.name, "rpmsg-openamp-demo-channel");
	eptinfo.src = 0;
	eptinfo.dst = 0xFFFFFFFF;
	ret = rpmsg_create_ept(charfd, &eptinfo);
	if (ret) {
		printf("failed to create RPMsg endpoint.\n");
		return -EINVAL;
	}
	if (!get_rpmsg_ept_dev_name(rpmsg_char_name, eptinfo.name,
				    ept_dev_name))
		return -EINVAL;
	sprintf(ept_dev_path, "/dev/%s", ept_dev_name);
	fd = open(ept_dev_path, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		perror("Failed to open rpmsg device.");
		close(charfd);
		return -1;
	}

	return 0;
}

void exit_app(){
	close(fd);
	if (charfd >= 0)
		close(charfd);

	printf("\r\n Quitting application .. \r\n");
	printf(" LCD application end \r\n");
}
