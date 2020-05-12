#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static const char * const ssfs_srv_usage[] = {
	"usage: ssfs-srv [options]\n Runs the ssfs server",
  "-h|--help        print this message.",
	"-v|--verbose     increase verbosity about performed actions.",
	"-p|--port        TCP port to listen on. (Default 80)",
};

void usage() {
  printf("%s\n", ssfs_srv_usage[0]);
  for (int i = 1; i < 4; ++i) {
    printf("\t%s\n", ssfs_srv_usage[i]);
  }
}

int main(int argc, char **argv) {
  int c;
  optind = 0;
  
  int verbosity = 0;
  int port = 80;
  char *p_end;

  while (1) {
    static const struct option long_opts[] = {
      { "help", no_argument, NULL, 'h' },
      { "verbose", no_argument, NULL, 'v' },
      { "port", required_argument, NULL, 'p' },
      { NULL, 0, NULL, 0 }
    };

    c = getopt_long(argc, argv, "hvp:", long_opts, NULL);
    if (c < 0)
      break;
    switch(c) {
    case 'h':
      usage();
      return 0;
    case 'v':
      verbosity++;
      break;
    case 'p':
      port = strtol(optarg, &p_end, 10);
      break;
    }
  }

  printf("Starting ssfs-srv listening on port %d\n", port);
  return 0;
};
