#!/bin/bash

cd ${0%/*}

self="$PWD"/${0##*/}
echo "RUN TEST:" $self

result=asm.result

#  clean any previous output
rm -f $result

input=`ls *.cool`
$1 $input -o $result > stderrout.result 2>&1
msg=$(cmp $result asm.expect)
diff=$?

if [ $diff = 0 ]; then
 echo "SUCCES"
 exit 0
else
 echo "FAIL:" $msg
 exit 1
fi
