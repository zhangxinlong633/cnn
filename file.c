#include "file.h"

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

