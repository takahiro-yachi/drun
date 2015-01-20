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
If you set --pid_file <file> option, drun recognize content of <file> as a pid of supervisor process.  
Signal handling of supervisor process is as follows.
```
SIGINT : Stop program without restart
SIGTERM : Stop program without restart
SIGHUP : Relay signal
SIGKILL : Don't use !, Program will be a zombie
```
