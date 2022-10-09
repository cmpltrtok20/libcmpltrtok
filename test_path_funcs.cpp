#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "cmpltrtok.h"

using namespace std;

int main() {
	if (1) {
		char *arr[] = {
			"/",
			"/var",
			"/var/asuspei/backup",
			"/var/asuspei/backup/xxx",
			"/var/asuspei/backup/xxx.jpeg",
			"/var/asuspei/backup/xxx.tmp.jpeg",

			"abc",
			"abc.txt",
			"abc.tmp.txt",

			"result/pred.jpg",
			"result/pred.tmp.jpg",
			"result/pred",

			"dir/",
			"/var/dir/",

			"",
		};
		int i;
		char dir_buf[MAXLINE], main_buf[MAXLINE];
		for (i = 0; i < sizeof(arr)/sizeof(char*); i++) {
			char *str = arr[i];
			if (1) {
				char *base = cmp_path_split(str, dir_buf, MAXLINE);
				char *base_main = cmp_get_base_main_name(str);
//				printf("|%s|: dir=|%s|, base=|%s|, base_main=|%s|\n", str, dir_buf, base, base_main);
				cout << "|" << str << "|: dir=|" << dir_buf << "|, base=|" << base << "|, base_main=|" << base_main << "|" << endl;
				free(base);
				free(base_main);
			}
			if (1) {
				char *ext = cmp_path_split_ext(str, main_buf, MAXLINE);
//				printf("|%s|: main=|%s|, ext=|%s|\n", str, main_buf, ext);
				cout << "|" << str << "|: main=|" << main_buf << "|, ext=|" << ext << "|" << endl;
				free(ext);
			}
		}
	}
	if (1) {
		char *arr[] = {
			"/var/asuspei/my_svn/c_darknet/darknet-debug-cpu/scripts/VOCdevkit/VOC2012/JPEGImages/2008_000002.jpg",
			"/var/asuspei/my_svn/c_darknet/darknet-debug-cpu/scripts/VOCdevkit/VOC2012/JPEGImages/2008_000003.jpeg",
			"scripts/VOCdevkit/VOC2012/JPEGImages/2008_000003.bmp",
			"JPEGImages/2008_000003.png"
		};
		int i;
		for (i = 0; i < sizeof(arr)/sizeof(char*); i++) {
			char *str = arr[i];
			char *label_path = cmp_path_get_cousin(str, "labels", "txt");
			char *cousin_path = cmp_path_get_cousin(str, "cousin", 0);
//			printf("%s\n%s\n%s\n", str, label_path, cousin_path);
			cout << str << endl << label_path << endl << cousin_path << endl;
			free(label_path);
			free(cousin_path);
		}
	}
	if (1) {
		char *arr[] = {
			"/home/asuspei/python_ai/category/data/_data/mnist_dataset_png_gen.py/test/1000_1.png",
			"/home/asuspei/python_ai/category/data/_data/mnist_dataset_png_gen.py/test/1001_2.png",
			"/home/asuspei/python_ai/category/data/_data/mnist_dataset_png_gen.py/test/1002_0.png",
			"/home/asuspei/python_ai/category/data/_data/mnist_dataset_png_gen.py/test/1003_9.png",
			"/home/asuspei/python_ai/category/data/_data/mnist_dataset_png_gen.py/test/100_3.png",
			"/home/asuspei/python_ai/category/data/_data/mnist_dataset_png_gen.py/test/1004_8.png",
			"/home/asuspei/python_ai/category/data/_data/mnist_dataset_png_gen.py/test/1005_6.png"
		};
		char *labels[] = {"0", "1", "2", "8", "9"};
		int i, j;
		size_t len_j = sizeof(labels)/sizeof(char*);
		for (i = 0; i < sizeof(arr)/sizeof(char*); ++i) {
			char *str = arr[i];
			printf("%s", str);
			for (j = 0; j < len_j; ++j) {
				printf(" %s:%d", labels[j], cmp_path_is_label(str, labels[j]));
			}
			printf("\n");
		}
	}
	if (1) {
		char *arr[] = {
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/4031_deer.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/9219_truck.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/9944_horse.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/1182_automobile.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/6280_ship.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/4562_deer.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/2587_airplane.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/336_cat.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/6326_horse.png",
			"/var/asuspei/large_data/DL1/_many_files/cifar-10_pngs/cifar/test/6066_bird.png"
		};
		char *labels[] = {
			"airplane",
			"automobile",
			"bird",
			"cat",
			"deer",
			"dog",
			"frog",
			"horse",
			"ship",
			"truck"
		};
		int i, j;
		size_t len_j = sizeof(labels)/sizeof(char*);
		for (i = 0; i < sizeof(arr)/sizeof(char*); ++i) {
			char *str = arr[i];
//			printf("%s", str);
			cout << str;
			for (j = 0; j < len_j; ++j) {
//				printf(" %s:%d", labels[j], path_is_label(str, labels[j]));
				cout << " " << labels[j] << ":" << cmp_path_is_label(str, labels[j]);
			}
//			printf("\n");
			cout << endl;
		}
	}
}
