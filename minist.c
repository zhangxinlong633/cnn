#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define MINIST_IMAGS_PIXS		28
#define MINIST_IMAGS_NUM_TRAIN	60000
#define MINIST_IMAGS_NUM_TEST	10000

#define MINIST_IMAGES_PATH "./minist_data/train-images-idx3-ubyte"
#define MINIST_IMAGES_LABEL "./minist_data/train-labels-idx1-ubyte"
#define MINIST_TEST_PATH "./minist_data/t10k-images-idx3-ubyte"
#define MINIST_TEST_LABEL "./minist_data/t10k-labels-idx1-ubyte"

struct minist_image_info {
	uint8_t label; /* 0-9 */
	uint8_t image[MINIST_IMAGS_PIXS][MINIST_IMAGS_PIXS]; /* 0-9 images */
};

// test 
int read_images(char *path, struct minist_image_info *images, int images_len)
{
	int ret = 0;
	uint32_t magic  = 0;
	uint32_t len = 0;
	uint8_t image[MINIST_IMAGS_PIXS][MINIST_IMAGS_PIXS]; 
	uint32_t pixs;

	int fd = open(path, O_RDONLY);
	if (fd <= 0) {
		goto exit;
	}

	//magic
	ret = read(fd, &magic, sizeof(magic));
	if (ret != 4) {
		goto exit;
	}

	//len
	ret = read(fd, &len, sizeof(magic));
	if (ret != 4) {
		goto exit;
	}
	len = ntohl(len);

	len = images_len > len ? len : images_len;
		
	ret = read(fd, &pixs, sizeof(pixs));
	ret = read(fd, &pixs, sizeof(pixs));

	// images
	for (int i = 0; i < len; i ++) {
		ret = read(fd, image, MINIST_IMAGS_PIXS * MINIST_IMAGS_PIXS);
		if (ret != MINIST_IMAGS_PIXS * MINIST_IMAGS_PIXS) {
			goto exit;
		}
		memcpy(images[i].image, image, MINIST_IMAGS_PIXS * MINIST_IMAGS_PIXS);
	}

	ret = 0;

exit:
	return ret;
}

int read_labels(char *path, struct minist_image_info *images, int images_len)
{
	int ret = 0;
	uint32_t magic  = 0;
	uint32_t len = 0;
	uint8_t label = 0;

	int fd = open(path, O_RDONLY);
	if (fd <= 0) {
		goto exit;
	}

	//magic
	ret = read(fd, &magic, sizeof(magic));
	if (ret != 4) {
		goto exit;
	}

	//len
	ret = read(fd, &len, sizeof(magic));
	if (ret != 4) {
		goto exit;
	}
	len = ntohl(len);

	len = images_len > len ? len : images_len;

	// images
	for (int i = 0; i < len; i ++) {
		ret = read(fd, &label, sizeof(uint8_t));
		if (ret != sizeof(uint8_t)) {
			goto exit;
		}
		images[i].label = label;
	}

	ret = 0;

exit:
	return ret;
}

int dump(struct minist_image_info *image) 
{
	printf("###dump####\n");
	printf("label is: %3d\n", 	image->label);
	printf("image is:\n");
	for (int i = 0; i < MINIST_IMAGS_PIXS; i ++) {
		for (int j = 0; j < MINIST_IMAGS_PIXS; j ++) {
			printf("%2x ", image->image[i][j]);
		}
		printf("\n");
	}

	return 0;
}

int read_datas(struct minist_image_info *images_train, int images_len, struct minist_image_info *images_test, int test_len)
{
	int ret = EINVAL;

	ret = read_images(MINIST_IMAGES_PATH, images_train, images_len);
	if (ret != 0) {
		printf("read images fail, ret: %d.\n", ret);
		goto exit;
	}

	ret = read_labels(MINIST_IMAGES_LABEL, images_train, images_len);
	if (ret != 0) {
		printf("read images label fail, ret: %d.\n", ret);
		goto exit;
	}

	ret = read_images(MINIST_TEST_PATH, images_test, test_len);
	if (ret != 0) {
		printf("read images test fail, ret: %d.\n", ret);
		goto exit;
	}

	ret = read_labels(MINIST_TEST_LABEL, images_test, test_len);
	if (ret != 0) {
		printf("read images test label fail, ret: %d.\n", ret);
		goto exit;
	}

exit:
	return ret;
}

/*
   1. read image / label file -> set(image, label)
   2. read test / lable file -> set(image, label)
   3. conv
   4. test result
   5. compare result
*/

int main(int argc, char **argv)
{
	int ret = EINVAL;
	int i = 0;
	
	struct minist_image_info *images_train = malloc(MINIST_IMAGS_NUM_TRAIN * sizeof(struct minist_image_info));
	if (images_train == NULL) {
		printf("no memory for images_train.\n");
		goto exit;
	}

	struct minist_image_info *images_test = malloc(MINIST_IMAGS_NUM_TEST * sizeof(struct minist_image_info));
	if (images_test == NULL) {
		printf("no memory for images_test.\n");
		goto exit;
	}

	ret = read_datas(images_train, MINIST_IMAGS_NUM_TRAIN, images_test, MINIST_IMAGS_NUM_TEST);
	
#define DUMP
#ifdef DUMP
	for (i = 0; i < MINIST_IMAGS_NUM_TRAIN; i ++) {
		dump(&images_train[i]);
	}

	for (i = 0; i < MINIST_IMAGS_NUM_TEST; i ++) {
		dump(&images_test[i]);
	}
#endif

exit:
	return ret;
}

