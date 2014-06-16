mkdir /tmp/big
dd if=/dev/zero of=/tmp/big/file1 bs=1M count=50
for i in $(seq 2 3)
do
   cp /tmp/big/file1 /tmp/big/file$i
done
