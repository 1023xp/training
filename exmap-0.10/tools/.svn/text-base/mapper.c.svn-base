/* (c) John Berthels 2005 <jjberthels@gmail.com>. See COPYING for license */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h> /* For mmap() */
#include <sys/fcntl.h> /* For open flags */
#include <unistd.h> /* For getopt */
#include <stdio.h>
#include "pagesize.h"

enum MapType {
     PRIVATE,
     SHARED
};

enum WorkType {
     READ,
     WRITE,
     JUSTMAP
};

static int create_fill_file(const char *fname, int size)
{
     int i;
     FILE *fp = fopen(fname, "w+");
     if (fp == NULL) {
	  perror("fopen failed");
	  return -1;
     }
     for (i = 0; i < size; ++i) {
	  fputc('A', fp);
     }
     fflush(fp);
     fsync(fileno(fp));
     fclose(fp);
     return 0;
}

static void *make_map(const char *fname,
		      void *map_at,
		      int map_size,
		      enum WorkType wt,
		      enum MapType mt)
{
     int fd = -1;
     int mmap_flags;
     int mmap_prot;
     void *p = NULL;

     switch (mt) {
     case PRIVATE:
	  mmap_flags = MAP_PRIVATE;
	  break;
     case SHARED:
	  mmap_flags = MAP_SHARED;
	  break;
     default:
	  fprintf(stderr, "Invalid map flags\n");
	  goto Err;
     }

     switch (wt) {
     case READ:
	  mmap_prot = PROT_READ;
	  break;
     case WRITE:
	  mmap_prot = PROT_READ | PROT_WRITE;
	  break;
     case JUSTMAP:
	  mmap_prot = PROT_READ | PROT_WRITE;
	  break;
     default:
	  fprintf(stderr, "Invalid map flags\n");
	  goto Err;
     }
     
     /* Null fname is used to signal anonymous map */
     if (fname != NULL) {
	  fd = open(fname, O_RDWR);
	  if (fd < 0) {
	       perror("open failed");
	       goto Err;
	  }
     }
     else {
	  mmap_flags |= MAP_ANONYMOUS;
     }

     if (map_at != NULL) {
	  mmap_flags |= MAP_FIXED;
     }
     
     p = mmap(map_at,
	      map_size,
	      mmap_prot,
	      mmap_flags,
	      fd,
	      0);

     /* printf("pid %d mmap returned %p\n", getpid(), p); */
     if (p == MAP_FAILED) {
	  perror("mmap failed");
	  goto Err;
     }
     if (map_at != NULL && p != map_at) {
	  fprintf(stderr, "can't map at required address [%p] got [%p]\n",
		  map_at, p);
	  goto Err;
     }

     return p;
Err:
     if (fd >= 0) {
	  close(fd);
     }
     if (p != MAP_FAILED) {
	  munmap(p, map_size);
     }
     return MAP_FAILED;
}

static int do_work(char *p, int map_size, enum WorkType wt, enum MapType mt)
{
     int i;
     int sum = 0;
     pid_t pid = getpid();

     for (i = 0; i < map_size; ++i) {
	  switch (wt) {
	  case READ:
	       sum += p[i];
	       sum %= 10;
	       break;
	  case WRITE:
	       p[i] = pid % 100;
	       break;
	  case JUSTMAP:
	       break;
	  default:
	       fprintf(stderr, "Invalid work type\n");
	       return -1;
	  }
     }
     
     return 0;
}

static void wait_for_all_children(void)
{
     int rc, status;

     do {
	  rc = wait(&status);
     }
     while (rc > 0);
}

/*
 * fork n copies, each copy then mmap's a file and twiddles it.
 */
int main(int argc, char **argv)
{
     char *fname = NULL;
     int num_pages = 100, num_children = 5, sleep_secs = 60;
     enum WorkType wt = READ;
     enum MapType mt = SHARED;
     char opt;
     int pid, map_size;
     void *p = MAP_FAILED;
     void *map_at = NULL;
     int shared_anon_map = 0;
     
     while ((opt = getopt(argc, argv, "a:PSrwmf:p:n:s:")) > 0) {
	  switch(opt) {
	  case 'a': /* map at this address */
	       sscanf(optarg, "%p", &map_at);
	       if (map_at == NULL) {
		    fprintf(stderr, "Bad ptr argument for map at [%s]\n", optarg);
	       }
	       break;
	  case 'r':
	       wt = READ;
	       break;
	  case 'w':
	       wt = WRITE;
	       break;
	  case 'm':
	       wt = JUSTMAP;
	       break;
	  case 'P':
	       mt = PRIVATE;
	       break;
	  case 'S':
	       mt = SHARED;
	       break;
	  case 's':
	       sleep_secs = atoi(optarg);
	       break;
	  case 'f':
	       fname = optarg;
	       break;
	  case 'p':
	       num_pages = atoi(optarg);
	       break;
	  case 'n':
	       num_children = atoi(optarg);
	       break;
	  case ':':
	       fprintf(stderr, "Missing argument to option '%c'\n", opt);
	       return -1;
	  case '?':
	       fprintf(stderr, "Unrecognised option '%c'\n", opt);
	       return -1;
	  default:
	       fprintf(stderr, "Error processing options [%d:%c]\n", opt, opt);
	       return -1;
	  }
     }

     map_size = CURRENT_PAGE_SIZE * num_pages;

     shared_anon_map = (fname == NULL && mt == SHARED);
     
     if (fname != NULL) {
	  /* Create file before we fork off */
	  create_fill_file(fname, map_size);
     }

     if (shared_anon_map) {
	  /* Shared anon maps must be created by the parent */
	  p = make_map(fname, map_at, map_size, wt, mt);
     }
     
     do {
	  pid = fork();
     }
     while (pid != 0 && --num_children > 0);

     if (pid == 0) {
	  /* child */
	  if (p == MAP_FAILED) {
	       p = make_map(fname, map_at, map_size, wt, mt);
	  }
	  if (p == MAP_FAILED) {
	       fprintf(stderr, "Failed to create map\n");
	       return -1;
	  }
	  if (do_work(p, map_size, wt, mt) < 0) {
	       fprintf(stderr, "Error touching map\n");
	       return -1;
	  }
	  if (sleep_secs > 0) {
	       sleep(sleep_secs);
	  }
     }
     else {
	  /* parent */
	  if (shared_anon_map) {
	       /* should really have a short pause here...children are racing */
	       /* For measuring/testing, only the children should have the map */
	       munmap(map_at, map_size);
	  }
	  wait_for_all_children();
	  unlink(fname);
     }

     return 0;
}
