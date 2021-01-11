# SAO Utils NetworkData Extension Plugin

## 简介
NET Data Extension是一个以NERvSDK为基础开发的插件。目标是直接解析互联网的数据,自由度高。

目前NET Data Extension实现的功能有Json/Text解析,GET/POST请求,自定义UA,自定义Header,小数点精度,数据除法处理

## 安装
- NERvSDK版本需求：>= NERvSDK 0.2.1；
- SAO Utils版本需求：>= SAO Utils Beta 1；

从GitHub存储库的Release获取最新版本的发布NVG包，在SAO Utils中打开首选项->插件，点击“安装”。选择下载的NVG包，勾选“启用”，并保存配置。即可安装成功。

## 待开发功能
html/xml解析

WininetHttp异步请求

在线配置生成页面

## 已知问题
目前没有做请求异步,如产生网络问题,两次请求时间过短将导致SAO Utils陷入等待状态
