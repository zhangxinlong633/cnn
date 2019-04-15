#ifndef __MNIST_FILE_H
#define __MNIST_FILE_H

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

int read_images(char *path, struct minist_image_info *images, int images_len);
int read_labels(char *path, struct minist_image_info *images, int images_len);
int read_datas(struct minist_image_info *images_train, int images_len, struct minist_image_info *images_test, int test_len);
int dump(struct minist_image_info *image);

#endif
