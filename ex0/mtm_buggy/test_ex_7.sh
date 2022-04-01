#!/bin/bash
cd $(dirname $0)
ulimit -Sv 500000
$1 < 7.in | diff 7.out -
