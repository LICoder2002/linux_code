#pragma once

#include <iostream>
#include <string>
#include <cassert>
#include <jsoncpp/json/json.h>
#include "util.hpp"

#define CRLF "\r\n"
#define CRLF_LEN strlen(CRLF)
#define SPACE " "
#define SPACE_LEN strlen(SPACE)

#define OPS "+-*/%"

// #define MY_PROTOCOL

// decode，整个序列化之后的字符串进行提取长度
// 1. 必须具有完整的长度
// 2. 必须具有和len相符合的有效载荷
// 我们才返回有效载荷和len
// 否则，我们就是一个检测函数！
// 9\r\n100 + 200\r\n    9\r\n112 / 200\r\n
std::string decode(std::string &in, uint32_t *len)
{
    assert(len);
    // 1. 确认是否是一个包含len的有效字符串
    *len = 0;
    std::size_t pos = in.find(CRLF);
    if (pos == std::string::npos)
        return nullptr;

    // 2. 提取长度
    std::string strLen = in.substr(0, pos);
    int intLen = atoi(strLen.c_str());

    // 3. 确认有效载荷是否符合要求
    int leftLen = in.size() - pos - 2 * CRLF_LEN;
    if (leftLen < intLen)
        return nullptr;

    // 4. 确认完整的报文结构
    std::string package = in.substr(pos + CRLF_LEN, intLen);
    *len = intLen;

    // 5. 将当前报文完整的从in中移除
    int removeLen = strLen.size() + package.size() + 2 * CRLF_LEN;
    in.erase(0, removeLen);

    // 6. 正常返回
    return package;
}

// encode, 整个序列化之后的字符串进行添加长度
std::string encode(std::string &in, uint32_t len)
{
    // "........"
    // "len\r\n""........""\r\n"

    std::string encodein = std::to_string(len);
    encodein += CRLF;
    encodein += in;
    encodein += CRLF;
    return encodein;
}

// 定制请求 _x _op _y
class Request
{
public:
    Request() {}
    ~Request() {}

    // 序列化 -- 结构化的数据 -> 字符串
    void serialize(std::string *out)
    {
#ifdef MY_PROTOCOL
        std::string xstr = std::to_string(_x);
        std::string ystr = std::to_string(_y);

        *out = xstr;
        *out += SPACE;
        *out += _op;
        *out += SPACE;
        *out += ystr;
#else   

        //json
        //1. Value对象：万能对象
        //2. json基于kv模型
        //3. json有两套操作方法
        //4. 序列化时，会将所有的数据内容转化为字符串
        Json::Value root;
        root["x"] = _x;
        root["y"] = _y;
        root["op"] = _op;

        Json::FastWriter fw;
        *out = fw.write(root);

#endif
    }
    // 反序列化 -- 字符串 -> 结构化的数据
    bool deserialize(std::string &in)
    {
#ifdef MY_PROTOCOL
        // 100 + 200
        std::size_t space_first = in.find(SPACE);
        if (space_first == std::string::npos)
            return false;
        std::size_t space_second = in.rfind(SPACE);
        if (space_second == std::string::npos)
            return false;

        std::string data_first = in.substr(0, space_first);
        std::string data_second = in.substr(space_second + SPACE_LEN);
        // std::string oper = in.substr(space_first + SPACE_LEN, space_second - (space_first + SPACE_LEN));
        std::string oper = in.substr(space_first + SPACE_LEN, 1);
        if (oper.size() != 1)
            return false;

        // 转化为内部成员
        _x = atoi(data_first.c_str());
        _y = atoi(data_second.c_str());
        _op = oper[0];
        return true;
#else

        //json
        Json::Value root;
        Json::Reader reader;
        reader.parse(in, root);

        _x = root["x"].asInt();
        _y = root["y"].asInt();
        _op = root["op"].asInt();
        
        return true;

#endif
    }

    void debug()
    {
        std::cout << "#################################" << std::endl;
        std::cout << "x_: " << _x << std::endl;
        std::cout << "op_: " << _op << std::endl;
        std::cout << "y_: " << _y << std::endl;
        std::cout << "#################################" << std::endl;
    }

public:
    int _x;
    int _y;
    char _op;
};

// 定制回应 _exitCode _result
class Response
{
public:
    Response() : _exitCode(0), _result(0) {}
    ~Response() {}

    // 序列化
    void serialize(std::string *out)
    {
#ifdef MY_PROTOCOL
        //"_exitCode _result"
        std::string ec = std::to_string(_exitCode);
        std::string res = std::to_string(_result);

        *out = ec;
        *out += SPACE;
        *out += res;
#else

        //json
        Json::Value root;
        root["exitcode"] = _exitCode;
        root["result"] = _result;

        // Json::FastWriter fw;
        // *out = fw.write(root);
        Json::StyledWriter sw;
        *out = sw.write(root);

#endif
    }
    // 反序列化
    bool deserialize(std::string &in)
    {
        // "0 100"
#ifdef MY_PROTOCOL
        std::size_t pos = in.find(SPACE);
        if (pos == std::string::npos)
            return false;

        std::string codestr = in.substr(0, pos);
        std::string resultstr = in.substr(pos + SPACE_LEN);

        _exitCode = atoi(codestr.c_str());
        _result = atoi(resultstr.c_str());

        return true;
#else

    Json::Value root;
    Json::Reader reader;
    reader.parse(in, root);
    _exitCode = root["exitcode"].asInt();
    _result = root["result"].asInt();

    return true;

#endif
    }

    void debug()
    {
        std::cout << "#################################" << std::endl;
        std::cout << "_exitCode: " << _exitCode << std::endl;
        std::cout << "_result: " << _result << std::endl;
        std::cout << "#################################" << std::endl;
    }

public:
    // 退出状态码 0表示正常，-1表示除0异常，-2表示模0异常
    int _exitCode;
    // 运算结果
    int _result;
};

bool makeRequest(const std::string &str, Request *req)
{
    char strtmp[BUFFER_SIZE];
    snprintf(strtmp, sizeof strtmp, "%s", str.c_str());
    char *left = strtok(strtmp, OPS);
    if (!left)
        return false;

    char *right = strtok(nullptr, OPS);
    if (!right)
        return false;

    char mid = str[strlen(left)];

    req->_x = atoi(left);
    req->_y = atoi(right);
    req->_op = mid;
    return true;
}