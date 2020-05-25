#!/bin/bash

trap 'kill 0' EXIT

src/ssfs-srv -p 8080 &

ss -nlt | grep 8080
while [ $? -ne 0 ]; do ss -nlt | grep 8080; done;


nc localhost 8080 &

wait
