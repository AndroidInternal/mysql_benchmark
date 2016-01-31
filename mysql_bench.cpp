// =====================================================================================
// 
//       Filename:  mysql_bench.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  01/28/2016 08:38:49 AM
//       Revision:  none
//       Compiler:  gcc
// 
//         Author:  李锐, root@lirui.in
//        License:  Copyright © 2016 李锐. All Rights Reserved.
// 
// =====================================================================================
#include<mysql.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/time.h>
#include<pthread.h>

const int SIZE_1B = 1;
const int SIZE_1K = 1024;
const int SIZE_1M = 1024 * 1024;

char *server = "localhost"; 
char *user = "root";
char *password = "lirui";
char *database = "benchmark";
char *set_autocommit = "set autocommit=0";

//数字转字符串
char *my_itoa(int num)
{
    char *num_str = (char *)malloc(20);
    int pos = 0;
    if(num == 0)
        num_str[pos++] = '0';
    while(num)
    {
        num_str[pos++] = num % 10 + '0';
        num /= 10;
    }
    num_str[pos] = 0;
    for(int l = 0, r = pos - 1; l < r; l++, r--)
    {
        char tmp = num_str[l];
        num_str[l] = num_str[r];
        num_str[r] = tmp;
    }
    //printf("---%d %s\n", num, num_str);
    return num_str;
}

//插入sql语句字符串
char* get_query_str(int insert_size, int isReplaceInto, int pos)
{
    //printf("insert size: %d\n", insert_size);
    char *query = (char *)malloc(insert_size + 50);
    if(isReplaceInto == 0)
    {
        strcpy(query, "insert into tbl2(data) values(\"");
        int i, query_tmp_len = strlen(query);
        for(i = query_tmp_len; i < query_tmp_len + insert_size; i++)
        {
            query[i] = 't';
        }
        query[i] = '"';
        query[i + 1] = ')';
        query[i + 2] = ';';
        query[i + 3] = 0;
    }
    else
    {
        strcpy(query, "replace into tbl4 values(\"");
        char *num_str = my_itoa(pos);
        //printf("%d %s\n", pos, num_str);
        strcat(query, num_str);
        strcat(query, "\",\"");
        int i, query_tmp_len = strlen(query);
        for(i = query_tmp_len; i < query_tmp_len + insert_size; i++)
            query[i] = 't';
        query[i] = '"';
        query[i + 1] = ')';
        query[i + 2] = ';';
        query[i + 3] = 0;
    }
    return query;
}

//插入数据单元
struct Insert_Unit
{
    MYSQL *conn;
    int insert_size;
    char *insert_str;
};

//插入单元操作
void *single_insert(void *data)
{
    struct Insert_Unit *insert_unit = (struct Insert_Unit*)data;
    //printf("%s\n", get_query_str(insert_size));
    if(mysql_query(insert_unit->conn, insert_unit->insert_str))
    {
        printf("%s\n", mysql_error(insert_unit->conn));
    }
}

//测试插入
void test_insert(MYSQL *conn, int insert_time, int insert_size, int isReplaceInto)
{
    //innodb下使用这个避免每句sql都是事务操作
    //if(mysql_query(conn, set_autocommit))
      //  printf("%s\n", mysql_error(conn));
    printf("start inserting...\n");
    clock_t start, finish;
    double duration;
    timeval starttime, endtime;
    start = clock();
    gettimeofday(&starttime, 0);
    int i;
    if(isReplaceInto == 0)
    {
        struct Insert_Unit insert_unit;
        insert_unit.conn = conn;
        insert_unit.insert_size = insert_size;
        insert_unit.insert_str = get_query_str(insert_size, isReplaceInto, 0);
        //printf("%s\n", insert_unit.insert_str); 
        for(i = 0; i < insert_time; i++)
        {
            single_insert((void *)&insert_unit);
        }
    }
    else
    {
        for(i = 0; i < insert_time; i++)
        {
            struct Insert_Unit insert_unit;
            insert_unit.conn = conn;
            insert_unit.insert_size = insert_size;
            insert_unit.insert_str = get_query_str(insert_size, isReplaceInto, i);
            //printf("%s\n", insert_unit.insert_str);
            single_insert((void *)&insert_unit);
        }
    }
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    gettimeofday(&endtime, 0);
    double timeuse = 1000000 * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
    timeuse /= 1000;
    printf("finish inserting...\n");
    printf("clock method : %f seconds\n", duration);
    printf("linux api method : %f milliseconds\n", timeuse);
}

//测试并发情况下的插入
void test_concurrency_insert(MYSQL *conn, int insert_time, int insert_size)
{
  // if(mysql_query(conn, set_autocommit))
    //  printf("%s\n", mysql_error(conn));
   timeval starttime, endtime;
   gettimeofday(&starttime, 0);
   for(int i = 0; i < insert_time; i++)
   {
       struct Insert_Unit insert_unit;
       insert_unit.conn = conn;
       insert_unit.insert_size = insert_size;
       insert_unit.insert_str = get_query_str(insert_size, 0, 0);
       pthread_t thread_new;
       int ret_thread = pthread_create(&thread_new, NULL, single_insert, (void *)&insert_unit);
       if(ret_thread != 0)
       {
           printf("%d thread error\n");
       }
       pthread_join(thread_new, NULL);
   }
   gettimeofday(&endtime, 0);
   double timeuse = 1000000 * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
   timeuse /= 1000;
   printf("%f milliseconds\n", timeuse);
}

//测试删
void test_delete(MYSQL *conn)
{
    timeval starttime, endtime;
    gettimeofday(&starttime, 0);
    char *query = (char *)malloc(100);
    strcpy(query, "delete from tbl2");
    if(mysql_query(conn, query))
    {
        printf("%s\n", mysql_error(conn));
    }
    gettimeofday(&endtime, 0);
    double timeuse = 1000000 * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
    timeuse /= 1000;
    printf("%f seconds\n", timeuse);
}

//更新数据单元
struct Update_Unit
{
    MYSQL *conn;
    char *update_str;
};

//获取更新sql语句字符串
char* get_update_str(int update_size)
{
    char *query = (char *)malloc(update_size + 100);
    strcpy(query, "update tbl2 set data=\"");
    int i, query_tmp_len = strlen(query);
    for(i = query_tmp_len; i < query_tmp_len + update_size; i++)
    {
        query[i] = 't';
    }
    query[i] = 0;
    char *tmp = "\" where id > 0";
    //strcat(query, tmp);
    query_tmp_len = strlen(query);
    for(i = query_tmp_len; i < query_tmp_len + strlen(tmp); i++)
        query[i] = tmp[i - query_tmp_len];
    query[i] = 0;
    //printf("%s\n", query);
    return query;
}

//更新单元操作
void *single_update(void *data)
{
    Update_Unit *update_unit = (Update_Unit *)data;
    if(mysql_query(update_unit->conn, update_unit->update_str))
        printf("%s\n", mysql_error(update_unit->conn));
}

//测试改 
void test_update(MYSQL *conn, int update_size)
{
    timeval starttime, endtime;
    gettimeofday(&starttime, 0);
    Update_Unit update_unit;
    update_unit.conn = conn;
    update_unit.update_str = get_update_str(update_size);
    //for(int i = 0; i < update_time; i++)
    //{
    //    if(update_type == 1)
    //        single_update((void *)&update_unit);
    //    else{
    //        pthread_t thread_new;
    //        int ret_thread = pthread_create(&thread_new, NULL, single_insert, (void *)&update_unit);
    //        if(ret_thread != 0)
    //            printf("thread error %d\n", i);
    //        pthread_join(thread_new, NULL);
    //    }
    //}
    single_update((void *)&update_unit);
    gettimeofday(&endtime, 0);
    double timeuse = 1000000 * (endtime.tv_sec - starttime.tv_sec) + endtime.tv_usec - starttime.tv_usec;
    timeuse /= 1000;
    printf("%f milliseconds\n", timeuse);
}

//测试最大连接数量
void test_concurrency_connect()
{
    int i;
    for(i = 0; i < 1000000; i++)
    {
        MYSQL *conn;
        conn = mysql_init(NULL);
        if(!mysql_real_connect(conn, server,
                    user, password, database, 0, NULL, 0))
        {
            printf("%d %s\n", i, mysql_error(conn));
            break;
        }
    }
}

void close(MYSQL *conn)
{
    mysql_close(conn);
}

int main()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, server,
                user, password, database, 0, NULL, 0))
    {
        printf("%s\n", mysql_error(conn));
        return 0;
    }
    printf("print the type of benchmark:\n");
    printf("1.insert\n2.delete\n3.update\n4.replace\n");
    int type;
    scanf("%d", &type);
    switch(type)
    {
        case 1:
            int insert_type, insert_time, insert_size;
            printf("1.simple insert\n2.concurrency insert\n");
            scanf("%d", &insert_type);
            printf("print the time you wanto insert:\n");
            scanf("%d", &insert_time);
            printf("print the size you wanto insert:\n1.1B\n2.1K\n3.1M\nOthers\n");
            scanf("%d", &insert_size);
            if(insert_size == 1)
                insert_size = SIZE_1B;
            else if(insert_size == 2)
                insert_size = SIZE_1K;
            else if(insert_size == 3)
                insert_size = SIZE_1M;
            if(insert_type == 1)
                test_insert(conn, insert_time, insert_size, 0);
            else
            {
                test_concurrency_insert(conn, insert_time, insert_size);
            }
            break;
        case 2:
            test_delete(conn);
            break;
        case 3:
            printf("print the update size\n1.1B\n2.1K\n3.1M\nOther\n");
            int update_size;
            scanf("%d", &update_size);
            if(update_size == 1)
                update_size = SIZE_1B;
            else if(update_size == 2)
                update_size = SIZE_1K;
            else if(update_size == 3)
                update_size = SIZE_1M;
            test_update(conn, update_size);
            break;
        case 4:
            int rep_insert_time, rep_insert_type, rep_insert_size;
            printf("print the time you wanto insert:\n");
            scanf("%d", &rep_insert_time);
            printf("print the size you wanto insert:\n1.1B\n2.1K\n3.1M\nOthers\n");
            scanf("%d", &rep_insert_size);
            if(rep_insert_size == 1)
                rep_insert_size = SIZE_1B;
            else if(rep_insert_size == 2)
                rep_insert_size = SIZE_1K;
            else if(rep_insert_size == 3)
                rep_insert_size = SIZE_1M;
            test_insert(conn, rep_insert_time, rep_insert_size, 1);
            break;
    }
    return 0;
}
