

tar xf linux-4.12.10.tar.xz
make localmodconfig
sudo apt-get install build-essential libssl-dev
time make j4
sudo make modules_install install

EOM
