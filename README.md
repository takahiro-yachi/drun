# drun
Run program as daemon

## COMMAND
```
  drun OPTIONS PROGRAM [PARAMETERS...]
  drun --help
```
## OPTIONS
```
  --std_log <file>: log file for stdout
  --err_log <file>: log file for stderr
  --pid_file <file>: pid file(supervisor pid)
  --retry <integer>: PROGRAM restart limit
  --help: show this message and exit
```
## EXAMPLE
```
  drun --std_log /var/log/hoge/errlog --err_log /var/log/hoge/stdlog --pid_file /var/run/hoge.pid --retry 10 hoge param
```

## SIGNALS
If you set a --pid_file parameter, pid file contains a pid of supervisor process.  
Signal handling of supervisor process is as follows.
```
SIGINT : stop program without restart
SIGTERM : stop program without restart
SIGHUP : relay signal
SIGKILL : zombie
```
