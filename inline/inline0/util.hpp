#pragma once
#include<stdint.h>
#include<sys/time.h>
#include<cstdlib>
#include<fstream>
#include<iostream>
//bost库的split字符串切分
#include<boost/algorithm/string.hpp>
#include<unordered_map>
#include<vector>
#include<string>
using namespace std;

class TimeUtil
{
	//获取时间戳
	public:
		static int64_t TimeStamp()
		{
			//微妙级
			struct timeval tv;
			::gettimeofday(&tv,NULL);
			return tv.tv_sec;
		}
		static int64_t TimeStampMS()
		{   
			struct timeval tv;
			::gettimeofday(&tv,NULL);
			return tv.tv_sec*1000+tv.tv_usec/1000;
		}
};


///打印日志
//[I1552655528 util.hpp:31] hello
//[E1552655528 util.hpp:31] hello
//[W1552655528 util.hpp:31] hello
//[F1552655528 util.hpp:31] hello
//日志使用方式形如
//LOG(INFO)<<"hello"<<"\n"
//日志级别.
//FATAL 错误
//ERROR致命
//WARNING 警告
//INFO 提示
enum Level{
	INFO,
	WARNING,
	ERROR,
	FATAL,
};

inline std::ostream& Log(Level level,
		const std::string& file_name,int line_num){
	std::string prefix = "[";
	if(level == INFO){
		prefix+="I";
	}else if(level == WARNING){
		prefix +="W";
	}else if(level == ERROR){
		prefix+="E";
	}else if(level == FATAL){
		prefix+="F";
	}

	prefix += std::to_string(TimeUtil::TimeStamp());
	prefix += " ";
	prefix += file_name;
	prefix += ":";
	prefix += std::to_string(line_num);
	prefix += "]";
	std::cout<<prefix;
	return std::cout;
}

#define LOG(level) Log(level,__FILE__,__LINE__)



///////////////////////////////////////
//准备下文件相关工具类
////////////////////////////////////////////



class FileUtil{
	public:
		//传入一个文件路径，帮我们把文件的所有内容都读出来放到content字符串
		//content用引用同样可以
		//输入型参数const引用
		//输出型参数 指针
		//输入输出型参数 引用

		//读操作函数的封装
		static bool Read(const std::string& file_path,
				std::string* content){
			content->clear();
			std::ifstream file(file_path.c_str());

			if(!file.is_open())
			{
				return false;
			}
			//将文件的所有内容都读取到content中
			//getline按行读取，会去掉"\n"
			std::string line;
			while(std::getline(file,line)){
				*content += line+"\n";
			}
			file.close();
			return true;
		}

		//写操作函数的封装
		static bool Write(const std::string& file_path,
				const std::string& content){
			std::ofstream file(file_path.c_str());
			if(!file.is_open()){
				return false;
			}
			file.write(content.c_str(),content.size());
			file.close();
			return true;
		}
};


//字符串切分怎么搞
//1. strtok
//2. stringstream
//3. boost split 函数
class StringUtil{
	public:
		// aaa bbb ccc => 3
		// aaa  bbb ccc => 3 vs 4
		static void Split(const std::string& input,
				const std::string& split_char,
				std::vector<std::string>* output)
		{
			//split_char是字符串类型，is_any_of允许多个分隔符
			boost::split(*output,input,boost::is_any_of(split_char),
					boost::token_compress_off);//token_compress_off关闭分隔符压缩
		}
};


///////////////////////////////////////////////////////////////
//URL/body解析模块
//////////////////////////////////////////////////////////////
class UrlUtil{
	public:
		static void ParseBody(const std::string& body,
				std::unordered_map<std::string,std::string>* params){
			//1.先对这里的body字符串进行切分，切分成键值对的形式
			//  a)先按照 & 符号切分
			//  b)再按照 = 切分
			std::vector<std::string> kvs;
			StringUtil::Split(body,"&",&kvs);
			for(size_t i = 0;i<kvs.size();i++)
			{
				std::vector<std::string> kv;
				StringUtil::Split(kvs[i],"=",&kv);
				if(kv.size()!=2)
					continue;
				//unordered_map[] 操作的行为：如果key不存在，就新增.
				//如果key存在,就获取到对应的value
				//key是 "code" "stdin"没有特殊字符不需urldecode
				//2.对这里的键值对进行 urldecode,key不用转义，value需要转义 
				(*params)[kv[0]] = UrlDecode(kv[1]);
			}
		}
		unsigned char ToHex(unsigned char x) 
		{ 
			return  x > 9 ? x + 55 : x + 48; 
		}

		static unsigned char FromHex(unsigned char x) 
		{ 
			unsigned char y;
			if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
			else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
			else if (x >= '0' && x <= '9') y = x - '0';
			else assert(0);
			return y;
		}

		static std::string UrlDecode(const std::string& str)
		{
			std::string strTemp = "";
			size_t length = str.length();
			for (size_t i = 0; i < length; i++)
			{
				if (str[i] == '+') strTemp += ' ';
				else if (str[i] == '%')
				{
					assert(i + 2 < length);
					unsigned char high = FromHex((unsigned char)str[++i]);
					unsigned char low = FromHex((unsigned char)str[++i]);
					strTemp += high*16 + low;
				}
				else strTemp += str[i];
			}
			return strTemp;
		}
};
