# DonkeyID---php扩展-64位自增ID生成器
##原理
	参考Twitter-Snowflake 算法,扩展了其中的细节。具体组成如下图：
	
![bits.jpg](https://github.com/osgochina/donkeyid/blob/master/doc/bits.jpg?raw=true)

> 如图所示，64bits 咱们分成了4个部分。

> 1. 毫秒级的时间戳,有42个bit.能够使用139年，从1970年开始计算，能使用到2109年，当然这些是可以扩展的,可以通知指定起始时间来延长这个日期长度。
> 2. 自定义节点id,防止多进程运行产生重复id,能够256个节点。部署的时候可以配置好服务器id;
> 4. 进程workerid，占位5bit，能够生成32个进程id。根据pid运算获得。
> 4. 进程内毫秒时间自增序号。一毫秒能产生512个id。也就是说并发1秒能产生512000个id。

##使用
###安装

> 下载代码到本地，进入项目文件夹，执行

```Bash
/path/to/phpize &&  ./configure && make && make install
```

```Bssh
echo "extension=donkeyid.so" >> /path/to/php.ini
```
### 模式介绍

> DonkeyId 有两种id生成模式：

> 1. 默认模式,以上的介绍都是基于默认模式, new DonkeyId 的时候 $type=0或者不传参。
> 2. 第二种模式是10进制模式 生成最多20位数字.从右开始算第十位以后的数字是时间戳的秒，
>    第7位到第9位 是节点id。三位数字，最多到999.从第2位到第6位是秒内的自增id,
     最后一位是留给业务方的自定义位数。

###运行
####api接口

* new DonkeyId($type=0,$epoch=0);//$type 类型 值有0,1 epoch 纪元开始时间戳 可以设置从此开始计算秒数
* boolean setNodeId($node_id);
* string getNextId();
* string parseTime($id);
* int parseNodeId($id);
* int parseWorkerId($id);
* int parseSequence($id);

####测试代码

```php

    $donkey = new DonkeyId();
    $donkey->setNodeId(11); //0-511 不要超过这个值
    $id = $donkey->getNextId();
    $time = $donkey->parseTime($id);  //返回的是1970-1-1 00:00:00 到生成事件的毫秒数
    $node = $donkey->parseNodeId($id); //返回生成这个id的节点号
    $worker_id = $donkey->parseWorkerId($id); //返回生成id的进程号 不过是被处理过的，最多0-31
    $sequence = $donkey->parseSequence($id);  //返回同一时间内生成的序号
    
    echo "donkeyid=".$id."\n";
    echo "time=".date("Y-m-d H:i:s",$time/1000)."\n";
    echo "node=".$node."\n";
    echo "workerid=".$worker_id."\n";
    echo "sequence=".$sequence."\n";
   
```
#### 支持版本
> 支持 php5.3+ ,支持php 7
> 可以配置swoole使用,[donkeyid_server](https://github.com/osgochina/donkeyid_server.git)