# drun
Run program as daemon

## COMMAND
```
  drun OPTIONS PROGRAM [PARAMETERS...]
  drun --help
```
## OPTIONS
```
  --std_log: log file for stdout
  --err_log: log file for stderr
  --pid_file: pid file(supervisor pid)
  --retry: PROGRAM restart limit
  --help: show this message and exit
```
## EXAMPLE
```
  drun --std_log /var/log/hoge/errlog --err_log /var/log/hoge/stdlog --pid_file /var/run/hoge.pid --retry 10 hoge param
```
