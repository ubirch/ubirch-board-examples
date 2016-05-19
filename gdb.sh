#! /bin/bash
sed -u -e 's/^-exec-arguments.*$/-enable-timings no/' | exec /usr/local/bin/arm-none-eabi-gdb-py --init-command=`dirname $0`/.gdbinit $* | tee /tmp/gdbout.txt
