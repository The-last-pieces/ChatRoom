# 项目介绍

本项目旨在使用Linux下原生socket api搭建一个网络通信框架 . 

并在编写过程中学习 Linux,socket,多线程,数据库 等相关技术 . 

并基于此框架实现一个多人聊天室demo .

# 开发环境

- 开发平台  

  Ubuntu 18.04.4 LTS

- 语言标准  

  C++17

- 构建工具  

  CMake 3.10.2  

  gcc 7.5.0

- 数据库版本  

  mysql 5.7.38

# 项目功能

## 网络

- 封装TCP接口 , 使用io复用

- 封装UDP接口 , 使用io复用

- 实现RUDP协议 , 基于UDP

## 数据库

在服务端储存的数据 :

- 账户信息

- 好友关系

- 群组数据

在客户端储存的数据 :

- 聊天记录

## 工具库

- 定时器

- 线程库

## 聊天室

- 好友

- 私聊

- 群聊

- 信息类型

  - 纯文本

  - 文件

- 通知系统

- 控制台UI