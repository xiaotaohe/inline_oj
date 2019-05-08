//model
//MVC
//M => model: 负责数据存储
//V => view: 负责显示界面
//C => controller: 核心业务逻辑


//基于文件的方式完成题目的存储
//约定每一个题目对应一个目录，目录名字就是题目的id
//目录里面包含以下几个问价：
//1)header.cpp代码框架
//2)tail.cpp 代码测试用例
//3)desc.txt 题目详细描述
//
//除此之外，在搞一个oj_config.cfg 文件，作为一个总的入口文件
//这个文件是一个行文本文件
//这个文件的每一行对应一个需要被服务器加载起来的题目
//这一行里面包含以下几个信息：题目的id，题目的名字，题目的难度 题目对应的目录


#pragma once
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<algorithm>
#include"util.hpp"
//model这个模块要做的事请就是把刚才在文件中存储的题目信息加载起来，供服务器随时使用
//


struct Question{
  std::string id;
  std::string name;
  std::string dir;  //题目对应的目录，目录：题目的描述 
  //题目的代码框架 题目的测试用例
  std::string star;//题目的难度

  //以下这几个字段要根据dir字段来获取
  std::string desc;//题目的描述
  std::string header_cpp;//题目的代码框架
  std::string tail_cpp;//题目的测试用例代码
};

class OjModel{
  private:
    //字符串哈希算
    //MD5
    //SHA1
    std::map<std::string,Question> model_;

  public:
    //把文件上的数据加载起来到内存中，加到哈希表中
    bool Load(){
      //1.先打开 oj_config.cfg文件
      std::ifstream file("./oj_data/oj_config.cfg");
      if(!file.is_open()){
        return false;
      }
      //2.按行读取oj_config.cfg文件，并且解析
      std::string line;
      while(std::getline(file,line)){
        //3.根据解析结果拼装成Question结构体
        std::vector<std::string> tokens;
        StringUtil::Split(line,"\t",&tokens);
        if(tokens.size()!=4){
          LOG(ERROR)<<"config file format error"<<std::endl;
          continue;
        }
        //4.把结构体加入hash表中
        Question q;
        q.id=tokens[0];
        q.name=tokens[1];
        q.star=tokens[2];
        q.dir=tokens[3];
        FileUtil::Read(q.dir+"/desc.txt",&q.desc);
        FileUtil::Read(q.dir+"/header.cpp",&q.header_cpp);
        FileUtil::Read(q.dir+"/tail.cpp",&q.tail_cpp);
        model_[q.id] = q;
      }
      file.close();
      LOG(INFO)<<"Load"<<model_.size()<<"question"<<std::endl;
      return true;
    }
    bool GetAllQuestions(std::vector<Question>* question) const{
      //获取所有题目
      //遍历哈希表就行了
      question->clear();
      for(const auto& e:model_){
        question->push_back(e.second); 
      }return true;
    }

    bool GetQuestion(const std::string& id,Question* q) const {
      //获取某个具体题目
      std::map<std::string,Question>::const_iterator it = model_.find(id);
      if(it == model_.end()){
        //该id未查找到
        LOG(INFO)<<"未找到题目\n";
        return false;
      }
      *q = it->second;
      return true;
    }
};
