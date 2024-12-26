#!/bin/bash

# Check if worker2.c exists
if [ ! -f workers/worker2.c ]; then
    echo "worker2.c does not exist." >&2
    exit 1
fi

# Check if worker2.c is executable
if [ ! -x workers/worker2.c ]; then
    chmod +x workers/worker2.c
fi

# Execute worker2.c in the background
./workers/worker2.c &

# Wait 3 seconds
sleep 3

# Check if supervisor.c exists
if [ ! -f manager/supervisor.c ]; then
    echo "supervisor.c does not exist." >&2
    exit 1
fi

# Check if worker2.c is executable
if [ ! -x manager/supervisor.c ]; then
    chmod +x workers/worker2.c
fi

# Check if supervisor exists, if not compile it
if [ ! -f manager/supervisor ]; then
    gcc manager/supervisor.c -o manager/supervisor
    if [ $? -ne 0 ]; then
        echo "Error compiling supervisor.c" >&2
        exit 2
    fi
fi

# Execute supervisor
./manager/supervisor pairs.txt

exit 0 # Success !
