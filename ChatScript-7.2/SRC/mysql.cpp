#ifndef DISCARDMYSQL
#ifdef TESTX
#include "common.h"
extern "C" {
#include <mysql.h>
}
#include "my_sql.h"

void MySQLShutDown(){}
void MySQLUserFilesCode(){}
void MySQLserFilesCloseCode(){}
extern char* mySQLparams;


FunctionResult MySQLInitCode(char* buffer)
{
   MYSQL *conn;
   MYSQL_RES *res;
   MYSQL_ROW row;
   char *server = "localhost";
   char *user = "root";
   char *password = "mysql"; 
   char *database = "mysql";
   conn = mysql_init(NULL);

   if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\r\n", mysql_error(conn));
      exit(1);
   }
   if (mysql_query(conn, "show tables")) {
      fprintf(stderr, "%s\r\n", mysql_error(conn));
      exit(1);
   }
   res = mysql_use_result(conn);
   printf("MySQL Tables in mysql database:\n");
   while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);
   mysql_free_result(res);
   mysql_close(conn);

	return FAILRULE_BIT;
} 

FunctionResult MySQLCloseCode(char* buffer){return FAILRULE_BIT;}
FunctionResult MySQLExecuteCode(char* buffer){return FAILRULE_BIT;}


#endif
#endif