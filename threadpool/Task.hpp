#include <iostream>
#include <string>

class Task
{
public:
    Task(int elem1, int elem2, char oper)
        : _elem1(elem1),
          _elem2(elem2),
          _oper(oper)
    {
    }

    int operator()()
    {
        return run();
    }
    int run()
    {
        int res = 0;
        switch (_oper)
        {
        case '+':
            res = _elem1 + _elem2;
            break;
        case '-':
            res = _elem1 - _elem2;
            break;
        case '*':
            res = _elem1 * _elem2;
            break;
        case '/':
        {
            if (_elem2 == 0)
            {
                std::cout << "div zero!" << std::endl;
                res = -1;
            }
            else
            {
                res = _elem1 / _elem2;
            }
            break;
        }
        case '%':
        {
            if (_elem2 == 0)
            {
                std::cout << "mod zero!" << std::endl;
                res = -1;
            }
            else
            {
                res = _elem1 % _elem2;
            }
            break;
        }
        default:
            std::cout << "非法输入" << std::endl;
            break;
        }
    }

    void get(int *elem1, int *elem2, char *oper)
    {
        *elem1 = _elem1;
        *elem2 = _elem2;
        *oper = _oper;
    }

private:
    int _elem1;
    int _elem2;
    char _oper;
};
