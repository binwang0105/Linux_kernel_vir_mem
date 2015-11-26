#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include "err.h"
#include <sys/wait.h>

static int print_maps(void);

static void pr_errno(const char *msg)
{
	printf("error: %s, %s", strerror(errno), msg);
}

static void pr_err(const char *msg)
{
	printf("error: %s", msg);
}

static int test_1(void)
{
	int *t1;
	int page_size;
	int total_size;
	page_size = getpagesize();
	total_size = 10 * page_size;

	t1 = (int*) mmap(0, total_size, PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
	printf("\n\n\n--------------- test 1: ");
	printf("[%08x,%08x)", (int)t1, (int)t1 + total_size);
	printf(" ---------------\n");
	print_maps();
	return 0;
}

static int test_2(void)
{
	int *t2;
	int page_size;
	int total_size;
	int i;
	page_size = getpagesize();
	total_size = 10 * page_size;

	t2 = (int*) mmap(0, total_size, PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
	for(i = 0; i < total_size/page_size; i++) {
		*(t2 + i * page_size/sizeof(int)) = 0;
	}

	printf("\n\n\n--------------- test 2: ");
	printf("[%08x,%08x)", (int)t2, (int)t2 + total_size);
	printf(" ---------------\n");
	print_maps();
	return 0;
}

static int test_3(void)
{
	int *t3;
	int page_size;
	int total_size;
	int i;
	page_size = getpagesize();
	total_size = 10 * page_size;

	t3 = (int*) mmap(0, total_size, PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
	for(i = 0; i < total_size/page_size; i++) {
		if (i%2)
			*(t3 + i * page_size/sizeof(int)) = 0;
	}

	printf("\n\n\n--------------- test 3: ");
	printf("[%08x,%08x)", (int)t3, (int)t3 + total_size);
	printf(" ---------------\n");
	print_maps();
	return 0;
}

static int test_4(void)
{

	int *t4;
	int page_size;
	int total_size;
	int i;
	//pid_t pid;
	page_size = getpagesize();
	total_size = 10 * page_size;

	t4 = (int*) mmap(0, total_size, PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
	for(i = 0; i < total_size/page_size/2; i++) {
/*
		pid = fork();
		if (pid < 0) {
			printf("error in fork\n");
			exit(-1);
		}
*/
/*
		if (pid > 0) {
			*(t4 + i * page_size/sizeof(int)) = 0;
			continue;
		}
*/
		*(t4 + i * page_size/sizeof(int)) = 0;
		*(t4 + i * page_size/sizeof(int)) = 1;
		//return 0;
	}

	while (wait(NULL) > 0)
		;

	printf("\n\n\n--------------- test 4: ");
	printf("[%08x,%08x)", (int)t4, (int)t4 + total_size);
	printf(" ---------------\n");
	print_maps();

	return 0;
}

static int test_5(void)
{
	return 0;
}

static int test_6(void)
{
	return 0;
}

static int test_7(void)
{
	return 0;
}

int main(int argc, char **argv)
{
	int i;
	pid_t pid;


	for(i = 1; i <= 7; i++){
		pid = fork();
		if (pid < 0) {
			printf("error in fork\n");
			return -1;
		}
		if (pid > 0)
			continue;
		if (i == 1)
			test_1();
		if (i == 2)
			test_2();
		if (i == 3)
			test_3();
		if (i == 4)
			test_4();
		if (i == 5)
			test_5();
		if (i == 6)
			test_6();
		if (i == 7)
			test_7();
		return 0;
	}

	while (wait(NULL) > 0)
		;

	return 0;
}

static int print_maps(void)
{
	/*
	 * You may not modify print_maps().
	 * Every test should call print_maps() once.
	 */
	char *path;
	char str[25000];
	int fd;
	int r, w;

	path = "/proc/self/maps";
	printf("%s:\n", path);

	fd = open(path, O_RDONLY);

	if (fd < 0)
		pr_errno(path);

	r = read(fd, str, sizeof(str));

	if (r < 0)
		pr_errno("cannot read the mapping");

	if (r == sizeof(str))
		pr_err("mapping too big");

	while (r) {
		w = write(1, str, r);
		if (w < 0)
			pr_errno("cannot write to stdout");
		r -= w;
	}

	return 0;
}
