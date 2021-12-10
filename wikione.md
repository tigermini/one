# wikione
[toc]

## FAQ
### server和client能ping通，但是client提示Socket connect error
- 原因之一是因为服务器没有将端口号加入防火墙的放通列表里面
```shell
# 在centos中，将12356端口加入到防火墙的放通列表，示例如下：
[tiger@localhost server]$ sudo firewall-cmd --zone=public --add-port=12356/tcp --permanent 
Warning: ALREADY_ENABLED: 12356:tcp
success
[tiger@localhost server]$ sudo firewall-cmd --reload
success
[tiger@localhost server]$ sudo firewall-cmd --zone=public --list-ports
12356/tcp
```

### 在vmware虚拟机实验，server收到的client的ip地址异常
- 在vmware的clent的虚拟机上，进行虚拟机的网络适配器设置，将桥接模式的“复制物理网络连接状态(P)”勾选上
