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
