#include <unistd.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h> 
#include <sys/wait.h>

typedef struct option {
	char * std_log;
	char * err_log;
	char * pid_file;
	char * retry;
	char * cmd_param[1024];
	size_t cmd_params;
} option_t;

void usage(const char * cmd)
{
	printf(
		"run program as daemon\n"
		"COMMAND:\n"
		"  %s OPTIONS PROGRAM [PARAMETERS...]\n"
		"  %s --help\n"
		"OPTIONS:\n"
		"  --std_log: log file for stdout\n"
		"  --err_log: log file for stderr\n"
		"  --pid_file: pid file(supervisor pid)\n"
		"  --retry: PROGRAM restart limit\n"
		"  --help: show this message and exit\n"
		"EXAMPLE:\n"
		"  %s --std_log /var/log/hoge/errlog --err_log /var/log/hoge/stdlog --pid_file /var/run/hoge.pid --retry 10 hoge param\n"
		, cmd, cmd, cmd
	);
	exit(0);
}

int option_parser(int argc, char * argv[], option_t * opt)
{
	int i;
	memset(opt, 0, sizeof(*opt));
	for (i=1; i<argc; ++i) {
#define CMP_AND_SET(key) (!strcmp(argv[i], "--"#key)) opt->key = argv[++i]
		if (opt->cmd_params > 0) opt->cmd_param[opt->cmd_params++] = argv[i];
		else if (!strcmp(argv[i], "--help")) usage(argv[0]);
		else if CMP_AND_SET(std_log);
		else if CMP_AND_SET(err_log);
		else if CMP_AND_SET(pid_file);
		else if CMP_AND_SET(retry);
		else opt->cmd_param[opt->cmd_params++] = argv[i];
#undef CMP_AND_SET
		if (opt->cmd_params == 1024) {
			fprintf(stderr, "too many arguments (%zd)\n", opt->cmd_params);
			return -1;
		}
	}
	if (opt->cmd_params == 0) {
		fprintf(stderr, "command must be set\n");
		return -1;
	}
	return 0;
}

static pid_t s_child;
static void sig_term(int sig)
{
	kill(s_child, sig);
	s_child = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}
static void sig_hup(int sig)
{
	kill(s_child, sig);
}

static int supervisor(const option_t * opt)
{
	int retry = 1;
	signal(SIGINT, sig_term);
	signal(SIGTERM, sig_term);
	signal(SIGHUP, sig_hup);
	if (opt->retry) {
		retry += atoi(opt->retry);
	}
	while(retry--)
	{
		if ((s_child = fork()) == 0) {
			signal(SIGINT, SIG_DFL);
			signal(SIGTERM, SIG_DFL);
			signal(SIGHUP, SIG_DFL);
			execvp(opt->cmd_param[0], opt->cmd_param);
			fprintf(stderr, "error exec\n");
			kill(getppid(), SIGTERM);
			return -1;
		} else {
			int status;
			if (wait(&status) == s_child) {
				continue;
			}
			break;
		}
	}
	if (s_child) {
		kill(s_child, SIGTERM);
	}
	if (opt->pid_file) {
		unlink(opt->pid_file);
	}
	return 0;
}

int main(int argc, char * argv[])
{
	option_t opt;
	if (option_parser(argc, argv, &opt) < 0) {
		fprintf(stderr, "command error\n");
		return -1;
	}
	if (daemon(1,1) < 0) {
		fprintf(stderr, "daemon error\n");
		return -1;
	}
	if (opt.std_log) {
		if (freopen(opt.std_log, "a", stdout) == NULL) {
			fprintf(stderr, "std_log open error\n");
			return -1;
		}
	}
	if (opt.err_log) {
		if (freopen(opt.err_log, "a", stderr) == NULL) {
			fprintf(stderr, "err_log open error\n");
			return -1;
		}
	}
	if (opt.pid_file) {
		FILE * pidf = fopen(opt.pid_file, "w");
		if (pidf == NULL) {
			fprintf(stderr, "can not open pid file %s\n", opt.pid_file);
			return -1;
		}
		fprintf(pidf,"%d",getpid());
		fclose(pidf);
	}
	if (supervisor(&opt) < 0) {
		fprintf(stderr, "super visor error\n");
		return -1;
	}
	return 0;
}


