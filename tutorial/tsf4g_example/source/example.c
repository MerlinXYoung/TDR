#include <stdio.h>
#include "tdr_error_code.h"
#include "tdr_string.h"

#include "protocol/tdr_xml_reader.h"
#include "protocol/tdr_xml_writer.h"

#include "protocol/tdr_compact_reader.h"
#include "protocol/tdr_compact_writer.h"

#include "protocol/tdr_binary_reader.h"
#include "protocol/tdr_binary_writer.h"

#include "protocol/tdr_csv_reader.h"

#include "protocol_types.h"
#include "protocol_writer.h"
#include "protocol_reader.h"

#include "sql_types.h"
#include "sql_reader.h"
#include "sql_writer.h"

#include "protocol/tdr_mybind_reader.h"
#include "protocol/tdr_mybind_writer.h"

#include <stdbool.h>

#include <assert.h>


#include <string.h>
#include <time.h>

#define MAX_BUFF_SIZE 1024
static void test_compact()
{
	char buff[MAX_BUFF_SIZE];
	tdr_compact_writer_t compact_writer;
	tdr_compact_reader_t compact_reader;
	message_t message;
	int ret;

	message.mid = E_MID_LOGIN_REQ;
	snprintf(message.body.login_req.name, MAX_NAME_LENGTH, "xiaoxingxing");
	snprintf(message.body.login_req.password, MAX_NAME_LENGTH, "123456");
	message.body.login_req.age = 27;

	tdr_compact_writer_init(&compact_writer, buff, MAX_BUFF_SIZE);
	ret = tdr_write_message(&compact_writer.super, &message);

	memset(&message, 0, sizeof(message));
	tdr_compact_reader_init(&compact_reader, buff, MAX_BUFF_SIZE);
	ret = tdr_read_message(&compact_reader.super, &message);
}

static void test_binary()
{
	char buff[MAX_BUFF_SIZE];
	tdr_binary_writer_t writer;
	tdr_binary_reader_t reader;
	message_t message;
	int ret;

	message.mid = E_MID_LOGIN_RSP;
	message.body.login_rsp.result = 1;
	//ÓÉÓÚresultËµÃ÷µÇÂ¼Ê§°Ü£¬ ËùÒÔsession_idÊµ¼ÊÉÏÊÇÎÞÐ§µÄ£¬ ²¢²»»á±»¶Á³ö¡£
	message.body.login_rsp.session_id = 123321;

	tdr_binary_writer_init(&writer, buff, MAX_BUFF_SIZE);
	ret = tdr_write_message(&writer.super, &message);

	memset(&message, 0, sizeof(message));
	tdr_binary_reader_init(&reader, buff, MAX_BUFF_SIZE);
	ret = tdr_read_message(&reader.super, &message);
}

static void test_protocol()
{
	//compactÐÍÐ­Òé¾ßÓÐ¼òµ¥µÄÑ¹ËõÊý¾Ý¹¦ÄÜ£¬ Í¬Ê±´¦ÀíËÙ¶ÈÒ²·Ç³£¿ì£¬ ÊÊºÏÍâÍøÊý¾ÝµÄ´«Êä
	test_compact();

	//binaryÐÍÐ­ÒéÖ±½Ó°´ÕÕCÓïÑÔÄ¬ÈÏµÄ±àÂë·½Ê½´æ·Å£¬ ÓÃÐ¡¶Ë±íÊ¾£¬ ËÙ¶È×î¿ì£¬ ²»¾ß±¸Ñ¹Ëõ¹¦ÄÜ£¬ ÊÊºÏÄÚÍøÊý¾Ý´«Êä
	test_binary();
}


typedef tdr_error_code_t (*reader_func)(tdr_abstract_reader_t *self, void *data);
static int read_xml_from_file(const char* file, void *ptr, reader_func reader)
{
	bool ret = true;
	tdr_xml_reader_t xml_reader;
	tdr_error_code_t r;

	tdr_xml_reader_init(&xml_reader);	
	if(tdr_xml_reader_push_file(&xml_reader, "tconnd.xml") != E_TDR_NOERROR)
	{
		ret = false;
		goto done;
	}

	r = reader(&xml_reader.super, ptr); 
	if(r != E_TDR_NOERROR)
	{
		const tdr_xml_reader_yyltype_t *lo = tdr_xml_current_location(&xml_reader);
		if(lo)
		{
			fprintf(stderr, "%s(%d,%d - %d,%d) %s\n"
				, lo->file_name
				, lo->first_line, lo->first_column, lo->last_line, lo->last_column
				, tstrerror(r));
		}
		else
		{
			fprintf(stderr, "%s %s", file, tstrerror(r));
		}   	

		ret = false;
		goto pop_file;
	}
pop_file:
	tdr_xml_reader_pop_file(&xml_reader);
done:
	return ret;
}

static int read_xml_from_buff(const char* start, const char*limit, void *ptr, reader_func reader)
{
	int ret = true;
	tdr_xml_reader_t xml_reader;
	tdr_error_code_t r;

	tdr_xml_reader_init(&xml_reader);	
	if(tdr_xml_reader_push_buff(&xml_reader, start, limit) != E_TDR_NOERROR)
	{
		ret = false;
		goto done;
	}

	r = reader(&xml_reader.super, ptr); 
	if(r != E_TDR_NOERROR)
	{
		const tdr_xml_reader_yyltype_t *lo = tdr_xml_current_location(&xml_reader);
		if(lo)
		{
			fprintf(stderr, "(%d,%d - %d,%d) %s\n"
				, lo->first_line, lo->first_column, lo->last_line, lo->last_column
				, tstrerror(r));
		}
		else
		{
			fprintf(stderr, "%s", tstrerror(r));
		}   	

		ret = false;
		goto pop_file;
	}
pop_file:
	tdr_xml_reader_pop_file(&xml_reader);
done:
	return ret;
}

#define XML_OUTPUT_BUF 65536
static void test_xml()
{
	tdr_xml_reader_t xml_reader;
	tdr_xml_writer_t xml_writer;
	int ret;
	tconnd_config_t config;
	int bret;
	FILE* fout;
	const char*ch;

	static char output_buff[XML_OUTPUT_BUF];

	snprintf(config.log_config, MAX_NAME_LENGTH, "/usr/local/tconnd/etc/tconnd_log.xml");
	config.instance_config_num = 2;
	snprintf(config.instance_config[0].ip, IP_LENGTH, "127.0.0.1");
	config.instance_config[0].level = e_low;
	config.instance_config[0].port = 7001;	
	config.instance_config[0].backlog = 5;
	config.instance_config[0].epoll_size = 65536;

	snprintf(config.instance_config[1].ip, IP_LENGTH, "localhost");
	config.instance_config[1].level = e_high;
	config.instance_config[1].port = 7002;
	config.instance_config[1].backlog = 1;
	config.instance_config[1].epoll_size = 1024;

	tdr_xml_writer_init(&xml_writer, output_buff, output_buff + sizeof(output_buff));
	ret = tdr_write_tconnd_config(&xml_writer.super, &config);
	assert(ret == E_TDR_NOERROR);
	fout = fopen("tconnd.xml", "w");
	assert(fout != NULL);
	for(ch = xml_writer.start; ch < xml_writer.cur; ++ch)
	{
		fputc(*ch, fout);
	}
	fclose(fout);
	memset(&config, 1, sizeof(tconnd_config_t));
	bret = read_xml_from_buff(output_buff, output_buff + sizeof(output_buff), &config, (reader_func)tdr_read_tconnd_config);
	memset(&config, 1, sizeof(tconnd_config_t));
	bret = read_xml_from_file("tconnd.xml", &config, (reader_func)tdr_read_tconnd_config);


	memset(&config, 0, sizeof(tconnd_config_t));
	//ÓÃÏÂÃæÕâ¸öÃüÁî¿ÉÒÔÀ´Ìí¼Ó²éÕÒ°üº¬ÎÄ¼þµÄÄ¿Â¼
	tdr_xml_reader_init(&xml_reader);
	ret = tdr_xml_add_include(&xml_reader, "./etc");
	ret = tdr_xml_reader_push_file(&xml_reader, "./etc/tconnd_inc.xml");
	ret = tdr_read_tconnd_config(&xml_reader.super, &config);
	tdr_xml_reader_pop_file(&xml_reader);
}
#define MAX_ITEM_NUM 65536
item_table_t g_item_table[MAX_ITEM_NUM];
size_t g_item_table_num;
#define COL_STR_LEN 1024

static void test_csv()
{
	FILE *fin = NULL;
	char line[65536];
	size_t line_size = 0;
	int32_t row = 0;

	tdr_csv_reader_t reader;
	item_table_t item;
	tdr_error_code_t ret;

	fin = fopen("etc/item.csv", "r");

	//¿¿¿¿¿¿¿
	++row;
	fgets(line, sizeof(line), fin);
	line_size = strlen(line);

	//¿¿¿¿¿¿¿¿
	++row;
	fgets(line, sizeof(line), fin);
	line_size = strlen(line);
	tdr_csv_reader_init(&reader, line, (uint16_t)(line_size));


	++row;
	fgets(line, sizeof(line), fin);
	line_size = strlen(line);
	tdr_csv_reader_store(&reader, line, line_size);
	ret = tdr_read_item_table(&reader.super, &item);
	if(ret != E_TDR_NOERROR)
	{
		int32_t col = tdr_csv_reader_get_cur_col(&reader);
		const char *name = tdr_csv_reader_get_cur_name(&reader);
		printf("error(%d, %d[%s]), %s\n", row, col, name, tstrerror(ret));
	}
	tdr_csv_reader_close(&reader);

	++row;
	fgets(line, sizeof(line), fin);
	line_size = strlen(line);
	tdr_csv_reader_store(&reader, line, line_size);
	ret = tdr_read_item_table(&reader.super, &item);
	tdr_csv_reader_close(&reader);

	tdr_csv_reader_fini(&reader);
}
#include <mysql.h>
void test_mysql_insert()
{
	tdr_error_code_t ret;
	MYSQL *mysql = NULL;	
	const char *sql_insert = "insert into user value(?, ?, ?, ?);";
	MYSQL_STMT *stmt;
	int iret;
	user_t user;

	MYSQL_BIND   par_bind[1024];

	tdr_mybind_reader_t bind_reader;

	mysql = mysql_init(NULL);
	if(mysql == NULL)
	{		
		printf("mysql_client_init Error %u: %s", mysql_errno(mysql), mysql_error(mysql));
		exit(1);
	}

	if(mysql_real_connect(mysql, "192.168.0.71", "tsqld", "tsqld", "testdb"
		, 3306, NULL, 0) == NULL)
	{
		printf("mysql_real_connect Error %u: %s", mysql_errno(mysql), mysql_error(mysql));
		exit(1);
	}

	stmt = mysql_stmt_init(mysql);
	if(stmt == NULL)
	{
		printf("mysql_stmt_init Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	if(mysql_stmt_prepare(stmt, sql_insert, strlen(sql_insert)))
	{
		printf("mysql_stmt_prepare Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	memset(&par_bind, 0, sizeof(par_bind));
	tdr_mybind_reader_init(&bind_reader, par_bind, sizeof(par_bind));
	user.id = 2;
	user.exp = 123.321;
	user.gold = UINT32_MAX;
	snprintf(user.username, MAX_NAME_LENGTH, "xiaoxingxing");

	ret = tdr_read_user(&bind_reader.super, &user);
	iret = mysql_stmt_bind_param(stmt, par_bind);
	if(iret)
	{
		printf("mysql_stmt_bind_param Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}
	
	


	iret = mysql_stmt_execute(stmt);
	if(iret != 0)
	{
		printf("mysql_real_query Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	mysql_stmt_close(stmt);
	mysql_close(mysql);
}

void test_mysql_select()
{
	tdr_error_code_t ret;
	MYSQL *mysql = NULL;	
	const char *sql_insert = "select * from user_s;";
	MYSQL_STMT *stmt;
	int iret;
	user_t user;

	MYSQL_BIND   res_bind[1024];

	tdr_mybind_writer_t bind_writer;

	mysql = mysql_init(NULL);
	if(mysql == NULL)
	{		
		printf("mysql_client_init Error %u: %s", mysql_errno(mysql), mysql_error(mysql));
		exit(1);
	}

	if(mysql_real_connect(mysql, "192.168.0.71", "tsqld", "tsqld", "testdb"
		, 3306, NULL, 0) == NULL)
	{
		printf("mysql_real_connect Error %u: %s", mysql_errno(mysql), mysql_error(mysql));
		exit(1);
	}

	stmt = mysql_stmt_init(mysql);
	if(stmt == NULL)
	{
		printf("mysql_stmt_init Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	if(mysql_stmt_prepare(stmt, sql_insert, strlen(sql_insert)))
	{
		printf("mysql_stmt_prepare Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	memset(&res_bind, 0, sizeof(res_bind));
	tdr_mybind_writer_init(&bind_writer, res_bind, sizeof(res_bind));
	ret = tdr_write_user(&bind_writer.super, &user);
	iret = mysql_stmt_bind_result(stmt, res_bind);
	if(iret)
	{
		printf("mysql_stmt_bind_param Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}

	iret = mysql_stmt_execute(stmt);
	if(iret != 0)
	{
		printf("mysql_real_query Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
		exit(1);
	}


	iret = mysql_stmt_store_result(stmt);
	if(iret)
	{
			printf("mysql_stmt_store_result Error %u: %s", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
			exit(1);
	}

	for(;;)
	{
		memset(&user, 0, sizeof(user));
		if(mysql_stmt_fetch(stmt) == MYSQL_NO_DATA)
		{
			break;
		}
		printf("%u\n", user.id);
	}
	mysql_stmt_free_result(stmt);
	mysql_stmt_close(stmt);
	mysql_close(mysql);
}
int main()
{	
	test_csv();

	test_protocol();
	
	test_xml();	

	return 0;
}
