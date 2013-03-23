weather_client
==============

计算机网络实验lab02 逆向工程套接字编程实验

weather_client
程序作用：天气查询客户端程序，可以查看某个城市某一天或之后三天的天气。
编译：make
运行：.\weather_client
清除编译结果：make clean

weather_server
程序作用：天气查询服务器程序，可以提供中国34个省会城市的天气信息
编译：make
运行：.\weather_server
清除编译结果：make clean

（注意：若需要使得我自己写的服务器和客户端程序可以互相通信，完成功能，需要将weather_client.c文件中第12行的IP信息"114.212.191.33"改成对应服务器端的IP地址，然后再make）


附件中的trace文件为使用老师提供的客户端运行抓包的文件，而terminal为对于运行的命令及命令行显示的结果。
附件中的mytrace文件为使用我自己编写的客户端运行抓包的文件，而myterminal为对于运行的命令及命令行显示的结果。

