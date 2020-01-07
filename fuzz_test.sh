# !!!! run this from root directory of openenclave !!!!

# build OE
mkdir build
cd build
cmake .. -DUSE_LIBSGX=OFF
sudo make
sudo make install
cd ..

# build OE samples
export CC=gcc
. build/output/share/openenclave/openenclaverc
find samples  -name Makefile -execdir make \;

# create fuzz test folders
sudo mkdir /oesigndump /oesigndump/seeds /oesignsign /oesignsign/seeds /oeedger8r /oeedger8r/seeds /tcbinfo_test /tcbinfo_test/seeds /sgxextensions_test /sgxextensions_test/seeds /qeidentity_test /qeidentity_test/seeds

# copy test collaterals for oeedger8r
sudo cp build/tools/oeedger8r/oeedger8r /oeedger8r/
sudo cp `find samples -name *.edl` /oeedger8r/seeds

# copy test collaterals for oesign dump
sudo cp build/output/bin/oesign /oesigndump/oesigndump
sudo cp `find samples -name *.signed` /oesigndump/seeds

# copy test collaterals for oesign sign
sudo cp build/output/bin/oesign /oesignsign/oesignsign
sudo cp `find samples -name *.signed | cut -f -1 -d.` /oesignsign/seeds
sudo cp `find samples -name enc.conf` /oesignsign/
sudo cp `find samples -name private.pem` /oesignsign/

# copy test collaterals for tcbinfo parser
sudo cp build/tests/fuzztests/tcbinfo_test /tcbinfo_test
sudo cp build/tests/fuzztests/seeds/tcbinfoparser/* /tcbinfo_test/seeds

# copy test collaterals for sgx extension parser
sudo cp build/tests/fuzztests/sgxextensions_test /sgxextensions_test
sudo cp build/tests/fuzztests/seeds/parcesgxextensions/* /sgxextensions_test/seeds

# copy test collaterals for qe identity parser
sudo cp build/tests/fuzztests/qeidentity_test /qeidentity_test
sudo cp build/tests/fuzztests/seeds/qeidentity/* /qeidentity_test/seeds

# rename seeds for oesign with same extensions (demanded by fuzzer)
cd /oesignsign/seeds
for f in *;do sudo mv $f $f.unsigned;done
