#pragma one
//google 搞的一个开源库
#include"oj_model.hpp"
#include<ctemplate/template.h>
#include<string>
#include<vector>

class OjView{
  public:
    //根据数据，生成html这个动作，通常叫做网页渲染(reader)
    static void ReaderAllQuestions(
        const std::vector<Question>& all_questions,std::string* html){
      //这个函数帮我们把所有题目数据转换成题目列表也 html
      //在C++代码中通过字符串拼接的方式构造html太麻烦
      //通过网页模板的方式解决问题
      //类似于填空
      //准备好一个html把其中一些需要动态计算的数据挖空留下来，
      //处理请求过程中，根据需要计算结果来填这些空

      //1.先创建一个ctemplate的对象，这是一个总的组织数据的对象
      //2.循环的往这个对象中添加一些子对象
      //3.每个子对象再设置一些键值对(和模板中留下的{{}}是要对应的)
      //4.进行数据替换，生成最终的html
      ctemplate::TemplateDictionary dict("all_questions");
      for(const auto& question : all_questions){
        ctemplate::TemplateDictionary* table_dict = dict.AddSectionDictionary("question");
        dict.SetValue("id",question.id);
        dict.SetValue("name",question.name);
        dict.SetValue("star",question.star);
      }
      ctemplate::Template* tpl;
      tpl = ctemplate::Template::GetTemplate(
          "template/all_questions.html",
          ctemplate::DO_NOT_STRIP);
      tpl->Expand(html,&dict);
    }
    static void RenderQuestion(const Question& question,std::string* html)
    {
      ctemplate::TemplateDictionary dict("questions");
      dict.SetValue("id",question.id);
      dict.SetValue("name",question.name);
      dict.SetValue("star",question.star);
      dict.SetValue("desc",question.desc);
      dict.SetValue("header",question.header_cpp);
      ctemplate::Template* tpl;
      tpl = ctemplate::Template::GetTemplate(
          "template/question.html",
          ctemplate::DO_NOT_STRIP);
      tpl->Expand(html,&dict);
    }
    static void RenderResult(const std::string& str_stdout,const std::string& reason,std::string* html)
    {
      ctemplate::TemplateDictionary dict("result");
      dict.SetValue("stdout",str_stdout);
      dict.SetValue("reason",reason);
      
      ctemplate::Template* tpl;
      tpl = ctemplate::Template::GetTemplate(
          "./template/result.html",
          ctemplate::DO_NOT_STRIP
          );
      tpl->Expand(html,&dict);
    }
};
