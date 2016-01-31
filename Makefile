mysql_bench: mysql_bench.cpp
#g++ -o mysql_bench $(mysql_config --cflags) mysql_bench.cpp $(mysql_config --libs) -lpthread
  g++ -o mysql_bench -I/usr/include/mysql  -g -pipe -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp    -buffer-size=4 -m64 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fno-strict-aliasing -fwrapv -fPIC   -DUNIV    _LINUX -DUNIV_LINUX mysql_bench.cpp -rdynamic -L/usr/lib64/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -lssl -lcrypto
