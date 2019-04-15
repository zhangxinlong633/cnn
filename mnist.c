#include "file.h"

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

