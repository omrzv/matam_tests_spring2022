#!/bin/bash
cd $(dirname $0)
ulimit -Sv 500000
$1 < 10.in | diff 10.out -
