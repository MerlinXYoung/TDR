#include "tdr_string.h"

const char *tstrerror(tdr_error_code_t terrno)
{
	switch (terrno)
	{
	case E_TDR_NOERROR:
		return "正常";
	case E_TDR_ERROR:
		return "错误";
	case E_TDR_OUT_OF_MEMORY:
		return "内存溢出";
	case E_TDR_NOT_FOUND:
		return "找不到";
	case E_TDR_SYNTAX:
		return "语法错误";
	case E_TDR_MISMATCH:
		return "不匹配";
	case E_TDR_ERRNO:
		return "errno";
	case E_TDR_BAD_FILE:
		return "错误的文件";
	case E_TDR_EOF:
		return "文件结束";
	case E_TDR_INTEGER_OVERFLOW:
		return "整数溢出";
	case E_TDR_IGNORE:
		return "忽略";
	case E_TDR_PLEASE_READ_ENUM_NAME:
		return "请读取枚举名";
	default:
		return "未知错误";
	}
}
