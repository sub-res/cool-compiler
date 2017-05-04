#!/bin/bash

cd ${0%/*}

self="$PWD"/${0##*/}
echo "RUN TEST:" $self

asmResult=tmpresult.s

result=stderrout.result

#  clean any previous output
rm -f $result
rm -f $asmResult

input=`ls *.cool`
$1 $input -o $asmResult > $result 2>&1
msg=$(cmp $result stderrout.expect)
diff=$?

rm -f $asmResult

if [ $diff = 0 ]; then
 echo "SUCCES"
 exit 0
else
 echo "FAIL:" $msg
 exit 1
fi
