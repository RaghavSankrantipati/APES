

os=$(lsb_release -a)
kv=$(uname -a)
kb=$(cat /proc/version)
sys_arch=$(uname -m)

cat <<EOM >sysscript.txt


OS Type and Distribution:
$os

Kernel Version:
$kv

Kernel Build:
$kb

System Architecture:
$sys_arch

EOM
