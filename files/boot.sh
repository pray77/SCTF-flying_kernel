#!/bin/sh
qemu-system-x86_64 \
    -m 128M \
    -kernel /home/ctf/bzImage \
    -initrd /home/ctf/rootfs.img \
    -monitor /dev/null \
    -append "root=/dev/ram console=ttyS0 oops=panic panic=1 nosmap" \
    -cpu kvm64,+smep \
    -smp cores=2,threads=2 \
    -netdev user,id=t0, -device e1000,netdev=t0,id=nic0 \
    -nographic

