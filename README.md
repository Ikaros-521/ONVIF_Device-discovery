## 相关配置

ONVIF官网：[http://www.onvif.org/](http://www.onvif.org/)

gSOAP安装配置：[gSOAP安装配置+使用案例参考+参考链接](https://blog.csdn.net/Ikaros_521/article/details/103232677)

操作系统：CentOS7

## 资料参考：

许振坪的ONVIF专栏：[传送门](https://blog.csdn.net/benkaoya/category_6924052.html)

[onvif开发之设备发现功能的实现](https://blog.csdn.net/love_xjhu/article/details/11821037)

[Linux下onvif服务端之发现设备](https://blog.csdn.net/qq_26600237/article/details/81145602)

还有一个博文找不到网址了，如果网友发现可以私信补充。

## 代码实战

完整源码下载：[GitHub](https://github.com/Ikaros-521/ONVIF_Device-discovery)

### 客户端

#### 1、WS-Discovery的Ad hoc模式，使用多播（不使用gSOAP实现设备发现）

参考文章：[ONVIF协议网络摄像机（IPC）客户端程序开发（7）：设备搜索](https://blog.csdn.net/benkaoya/article/details/72476120)

源码如下 search_c.c：

Linux编译： `gcc search_c.c` ， Windows下编译：  `gcc search_c.c -lws2_32`

Windows下效果和Linux下效果一样。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203135206652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

我Linux是用的虚拟机，所以需要关闭防火墙，不然搜不到 `systemctl stop iptables.service` 或 `service iptables stop`

我们也使用 ONVIF Device Test Tool![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203135730447.png)，测一下 

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203135849329.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

#### 2、使用gSOAP生成ONVIF框架，编写客户端

如何生成ONVIF框架参考：[ONVIF协议网络摄像机（IPC）客户端程序开发（6）：使用gSOAP生成ONVIF框架代码](https://blog.csdn.net/benkaoya/article/details/72466827)，原文没有生成soapServer.c，我在评论里面写了如何修改，可以参考。

部分文件下载于：[GitHub](https://github.com/As772309423/onvif-server-discover)

参考文章：[onvif开发之设备发现功能的实现](https://blog.csdn.net/love_xjhu/article/details/11821037)

我做的相关改动都写在评论区了，server编译通不过，但client是可行的。

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019120314004826.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

**client.c**源码如下：


Linux下编译： `gcc -o client client.c stdsoap2.c soapC.c soapClient.c wsaapi.c duration.c -luuid`

没有libuuid库，可自行安装，命令仅供参考 `sudo apt-get install uuid-dev `

效果如图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203141449855.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

### 服务端

#### 1、不使用gSOAP，虚拟机伪装成ipc

文章参考：[Linux下onvif服务端之发现设备](https://blog.csdn.net/qq_26600237/article/details/81145602)


Linux下编译：`gcc ipc_server_c.c -o ipc_server`

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203141956890.png)

直接运行

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019120314201152.png)

现在我们用 ONVIF Device Test Tool，测试一下

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203142119887.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

ONVIF Device Test Tool 搜索后，服务端就打印了这些信息

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203142221919.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

ifconfig查看下虚拟机ip地址

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203142300230.png)

同样可以被我们之前写的 客户端 搜索到

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203142522136.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

#### 2、使用gSOAP生成ONVIF框架，编写服务端

文件下载于：[GitHub](https://github.com/As772309423/onvif-server-discover)

参考文章：[onvif服务器篇之onvif 服务器框架的搭建](https://blog.csdn.net/weixin_44362642/article/details/86749042)

碰到的问题同样写在评论区，可自行查看。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203142607157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

修改Makefile，GSOAP_ROOT改成自己的soap路径

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203143202882.png)

终端 `make` 编译。生成 deviceserver

我们直接运行 `./deviceserver`

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203143624179.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)

**我们同样用 ONVIF Device Test Tool，搜索一下**

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203143842910.png)

服务端也有所反应

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191203143742708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0lrYXJvc181MjE=,size_16,color_FFFFFF,t_70)



---


那么本次的服务端和客户端都测试完毕了。网上资料不多，真的进行的十分困难。



## 相关链接

ONVIF官网：[http://www.onvif.org/](http://www.onvif.org/)

gSOAP安装配置：[gSOAP安装配置+使用案例参考+参考链接](https://blog.csdn.net/Ikaros_521/article/details/103232677)

[许振坪的ONVIF专栏](https://blog.csdn.net/benkaoya/category_6924052.html)

[onvif开发之设备发现功能的实现](https://blog.csdn.net/love_xjhu/article/details/11821037)

[Linux下onvif服务端之发现设备](https://blog.csdn.net/qq_26600237/article/details/81145602)

[ONVIF协议网络摄像机（IPC）客户端程序开发（7）：设备搜索](https://blog.csdn.net/benkaoya/article/details/72476120)

[ONVIF协议网络摄像机（IPC）客户端程序开发（6）：使用gSOAP生成ONVIF框架代码](https://blog.csdn.net/benkaoya/article/details/72466827)

[onvif开发之设备发现功能的实现](https://blog.csdn.net/love_xjhu/article/details/11821037)

[Linux下onvif服务端之发现设备](https://blog.csdn.net/qq_26600237/article/details/81145602)

[GitHub服务端下载](https://github.com/As772309423/onvif-server-discover)

[onvif服务器篇之onvif 服务器框架的搭建](https://blog.csdn.net/weixin_44362642/article/details/86749042)

[ONVIF Device Test Tool](http://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=1&rsv_idx=2&ch=14&tn=98010089_dg&wd=ONVIF%20Device%20Test%20Tool&rsv_spt=1&oq=uuid_generate%25E2%2580%2599%25E6%259C%25AA%25E5%25AE%259A%25E4%25B9%2589%25E7%259A%2584%25E5%25BC%2595%25E7%2594%25A8&rsv_pq=fce567870001ae85&rsv_t=a164/AiScSOCRPNmIrBcLJbgENJ9FWFgopyq5W9tKZ%2bFeClsBvhaPOxGtfkOB7MJrh4&rqlang=cn&rsv_enter=0&rsv_dl=tb&inputT=629&rsv_n=2&rsv_sug3=3&rsv_sug4=629)


