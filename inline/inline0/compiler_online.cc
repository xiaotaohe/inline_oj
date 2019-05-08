#include"httplib.h"
#include"compiler.hpp"
#include<jsoncpp/json/json.h>
#include<unordered_map>

int main()
{
  using namespace httplib;
  //创建一个server对象
  Server server;
  //给server对象创建一个回调函数，这个函数的调用时机
  //.Get注册了一个Get方法，处理Get方法
  //lamada表达式，就是一个匿名函数
 
  //路由：通过不同的路径，触发不同处理函数
  //通过不同的路径，触发不同的回调函数
  server.Post("/compiler",[](const Request& req, Response& resp) {
      //根据具体的问题场景，根据请求，计算出响应结果
      (void)req;
      //如何从req中获取到JSON请求，JSON如何和HTTP协议结合。
      //JSON如何进行解析和构造？jsoncpp第三方库
      //需要的请求格式是JSON格式，而HTTP的格式是另一种格式（另一种键值对格式），所以我们
      //此处需要进行格式转换
      //此处由于提交的代码中可能会包含一些特殊的符号，这些特殊符号想要
      //正确传输，就需要进行转义，浏览器自动帮我们完成了
      //服务器收到请求后要做的第一件事就是urldecode，
      //然后解析这个数据整理成需要的JSON格式
      //键值对，用哪个数据结构来表示
      std::unordered_map<std::string,std::string> body_kv;//使用unordered_map来表示键值对
      //处理http中body的内容（键值对）
      UrlUtil::ParseBody(req.body,&body_kv);//请求内容放在body中（post方法），此时需要将内容解析出来
      
      //在这里调用CompileAndRun函数
      Json::Value req_json;//从req对象中获取
      //前置加加返回的是一个引用，后置加加返回的是一对象
      /*for(std::unordered_map<std::string,
          std::string>::iterator it = body_kv.begin();
          it != body_kv.end();++it){
          req_josn[it->first] = it->second;
      }*/
      //range based for(范围for)
      for(auto e:body_kv)
      {
        //e的类型和*it的类型是一致的
        req_json[e.first] = e.second;
      }
      Json::Value resp_json;//resp_json放到响应中
      Compiler::CompileAndRun(req_json,&resp_json);
      Json::FastWriter write;
      resp.set_content(write.write(resp_json),"text/plain");
      });
  //加上这个目录是为了让浏览器能够访问到一个静态页面
  //静态页面：index.html不会发生变化
  //动态页面：编译结果 随着参数不同而发生变化
  server.set_base_dir("./wwwroot");
  server.listen("0.0.0.0",9090);
  return 0;
}
