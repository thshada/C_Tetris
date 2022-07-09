#include <stdio.h>
#include <mysql.h>
#pragma comment(lib,"libmysql.lib")

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "abc123"
#define DB_NAME "test"
#define CHOP(x) x[strlen(x)-1]=' '

int main(void) {
	MYSQL* connection = NULL, conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	
	int query_stat;
	char ans = 0;
	char name[15];
	char address[20];
	char tel[15];
	char query[255];
	char password[50];

	mysql_init(&conn);

	mysql_options(&conn, MYSQL_SET_CHARSET_NAME, "euckr");
	mysql_options(&conn, MYSQL_INIT_COMMAND, "SET NAMES euckr");

	connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306, NULL, 0);
	if (connection == NULL) {
		fprintf(stderr, "연결실패원인 : %s\n", mysql_error(&conn));
		return 1;
	}


	query_stat = mysql_query(connection, "select*from address");
	if (query_stat != 0) {
		fprintf(stderr, "연결실패원인 : %s\n", mysql_error(&conn));
	}

	sql_result = mysql_store_result(connection);

	printf("%s %15s %15s\n", "이름", "주소", "전화번호");
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
		printf("%s %15s %15s\n", sql_row[0], sql_row[1], sql_row[2]);
	}
	
	mysql_free_result(sql_result);

	//입력 ==>DB
	printf("등록 하시겠습니까 : ");
	scanf("%c", &ans);
	scanf("%*c");

	if (ans == 'y' || ans == 'Y') {
		printf("이름 : ");
		fgets(name, 20, stdin);
		CHOP(name);

		printf("패스워드: ");
		fgets(password, 20, stdin);
		CHOP(password);
		
		sprintf(query, "insert into login values" "('%s','%s')", name, password);
		query_stat = mysql_query(connection, query);
		if (query_stat != 0) {
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return 1;
		}
		sql_result = mysql_store_result(connection);
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL) {
			printf("로그인 성공 \n");
		}
		
	}
	mysql_close(connection);
	//fgets()
	//sprintf()
	//mysql_query()
	//mysql_close()
	return 0;
}