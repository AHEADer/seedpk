Less Compiler
==========================

## 简介
**2015 亚马逊·“种子杯”编程 PK 赛初赛试题 LESS 编译器**

by lkx810, hyrathb, AHEADer in Hust UniqueStudio

## 编译环境
g++ 5.2.0

## 目录结构

├── Bin
├── Doc
├── Src
│   ├── advance.cpp               #词法分析器源代码
│   ├── ast.cpp                   #生成抽象语法树源代码
│   ├── CssPrint.cpp              #代码生成器
│   ├── eval.cpp                  #辅助求值
│   ├── include
│   │   ├── advance.h             #词法分析器头文件
│   │   ├── ast.h                 #生成抽象语法树头文件
│   │   ├── CssPrint.h            #生成抽象语法树头文件
│   │   ├── eval.h                #辅助求值头文件
│   │   └── token_list.h          #token链表声明头文件
│   └── main.cpp
└── Test
    ├── css
    │   ├── bootstrap.css
    │   ├── main.css
    │   └── test1.css
    ├── index.html
    └── less
        ├── test1.less
        ├── test2.less
        ├── test3.less
        ├── test4.less
        ├── test5.less
        ├── test6.less
        ├── test7.less
        ├── test8.less
        └── testmix1.less

## 实现功能

#### 基本功能

+ 注释规则
+ 变量规则
+ 运算规则
+ 嵌套规则
+ 作用域规则
+ 混合规则
+ 复合规则
+ 浏览器正确识别

#### 扩展功能

+
+
+
+

## 程序逻辑

程序先用`advance.cpp`中的`split`函数进行语法分析，生成一系列Token，将Token通过`token_list_elem`结构传给`ast.cpp`中的`ast_gen`类，其中的`first_step`方法会生成抽象语法树，可以用`get_root`方法得到抽象语法树的根，并将其传到`CssPrint_main`函数，生成对应的CSS代码。

## 程序接口简介

### token_list_elem结构

```
struct token_list_elem
{
    enum TYPE{
        RAW_TEXT,
        SELECTOR_NAME,
        BLOCK_BEGIN,
        BLOCK_END,
        SEPARATOR,
        COMMA,
        VAR_DEFINE,
        ASSIGN,
        INT,
        FLOAT,
        COLOR,
        PX,
        STRING,
        OPERATOR,
        PROPERTY_NAME,
        FUNC_NAME,
        COMMENT,
        FUNC_ARGUMENT_BEGIN,
        FUNC_ARGUMENT_END,
        Space,
        DelAll,
    } type;
    char content[64];
    struct token_list_elem *next;

};

```

**unkown**



