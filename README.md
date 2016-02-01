# mysql_benchmark
##MyISAM
### insert
| Size | 1000次非并发 |  1000次并发  |
| --- | ---:  | :----:  |
| 1B  |   153.042000 milliseconds      |      351.299000 milliseconds  |
| 1K  |    178.235000 milliseconds     |    367.479000 milliseconds  |
| 1M  |    22743.378000 milliseconds     |   36416.666000 milliseconds |

| Size | 10000次非并发 |  10000次并发  |
| --- | ---:  | :----:  |
| 1B  |  1283.575000 milliseconds       |    3704.788000 milliseconds    |
| 1K  |   1451.538000 milliseconds      |     3924.011000 milliseconds |
| 1M  |   216376.021000 milliseconds      |    Killed|

| Size | 100000次非并发 |  100000次并发  |
| --- | ---:  | :----:  |
| 1B  |   10987.371000 milliseconds      |     33661.334000 milliseconds   |
| 1K  |      13003.071000 milliseconds   |    42299.104000 milliseconds  |

| Size | 1000000次非并发 |  1000000次并发  |
| --- | ---:  | :----:  |
| 1B  |   106574.142000 milliseconds      |     334873.662000 milliseconds   |
###update
| Size | 10000条 |
| --- | ---:  | :----:  |
| 1B -> 1B |   12.728000 milliseconds      | 
| 1B -> 1K  |    119.833000 milliseconds     |  
| 1K -> 1M  |    150762.863000 milliseconds   | 

| Size | 100000条 |
| --- | ---:  | :----:  |
| 1B -> 1M  |   70047.413000 milliseconds      | 
| 1B -> 1B |    30.576000 milliseconds     |  
| 1B -> 1K |    1069.717000 milliseconds     |  
| 1K -> 1B |    245.646000 milliseconds     |  

| Size | 1000000条 |
| --- | ---:  | :----:  |
| 1B -> 1B  |   2209.890000 milliseconds   | 
| 1K -> 1B |    20819.543000 milliseconds     | 
| 1B -> 1K  |   21960.751000 milliseconds   | 

###replace
| Size | 10000条 |
| --- | ---:  | :----:  |
| 1B  |   1368.830000 millisecond     | 
| 1K  |    1733.311000 milliseconds     |  
| 1M  |    Killed   | 

| Size | 100000条 |
| --- | ---:  | :----:  |
| 1K->1B  |   44736.703000 milliseconds     | 
| 1B->1K  |   25949.895000 milliseconds     |  

##InnoDB
###insert use autocommit
| Size | 100次非并发 |  100次并发  |
| --- | ---:  | :----:  |
| 1B  |   1825.954000 milliseconds      |     1700.624000 milliseconds   |

| Size | 1000次非并发 |  1000次并发  |
| --- | ---:  | :----:  |
| 1B  |   17087.246000 milliseconds      |     17677.181000 milliseconds   |
###insert doesn't use autocommit
| Size | 1000次非并发 |  1000次并发  |
| --- | ---:  | :----:  |
| 1B  |   135.862000 milliseconds      |      336.357000 milliseconds  |
| 1K  |    178.235000 milliseconds     |    367.479000 milliseconds  |
| 1M  |    22743.378000 milliseconds     |   36416.666000 milliseconds |

| Size | 100000次非并发 |  100000次并发  |
| --- | ---:  | :----:  |
| 1B  |   9233.542000 milliseconds      |      31769.326000 milliseconds  |
| 1K  |    25374.079000 milliseconds     |    45352.992000 milliseconds  |

| Size | 1000000次非并发 |  1000000次并发  |
| --- | ---:  | :----:  |
| 1B  |   92098.966000 milliseconds      |      316967.485000 milliseconds  |


###性能瓶颈及分析
>* 大于1万条操作时，就有大于1秒的明显延迟等待，应该直接操作内存作为缓存加速，换SSD硬盘
>* 1B->1K大小的数据操作速度无太大明显差别，1K->1M降速明显，应该避免在数据库中存储大文件
>* 并发对于增加延时有明显影响，要减少并发，可以一台服务器专门接收请求转发sql请求

###碰到和解决的问题
>* mysql编译选项
>* 确定测试方法
>* gcc 和 g++ 对于pthread_create参数的影响和差别
>* 计时方法的科学性
>* max_allowed_packet
>* blob 和 mediumblob大小的确定，性能的影响确定，截断机制
>* makefile的编写http://harttle.com/2014/01/01/makefile.html

###对于结果准确性的分析
>* 表结构简单插入数据高度一致和现实情况并不相符
>* 线程模拟的并发，线程本身也有的上下文切换时间（context switch）难以计算

###待完善
>* 表结构简单
>* 有魔术数字
>* 计时有重复代码
>* 切换表不灵活
>* c++特性不明显（拿c++当c用）
>* innodb的auto_commit并没有彻底解决，对于事务目前还不了解


