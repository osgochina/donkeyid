# DonkeyID---64位自增ID生成器
##原理
	参考Twitter-Snowflake 算法,扩展了其中的细节。具体组成如下图：
	
![bits.jpg](https://github.com/osgochina/donkeyid/blob/master/doc/bits.jpg?raw=true)

> 如果所示，64bits 咱们分成了4个部分。
> 1. 用户自定义端,因为最后生成的id需要乘10,需要进位，所以预留了位置。
> 2. 毫秒级的时间戳,有42个bit.能够使用139年，从1970年开始计算，能使用到2109年，当然这些是可以扩展的。
> 3. 自定义节点id,防止多进程运行产生重复id,能够512个节点。部署的时候可以配置好服务器id+处理进程id
> 4. 进程内毫秒时间自增序号。一毫秒能产生256个id。也就是说并发1秒能产生256000个id。
##使用
###安装
> 下载代码到本地，进入项目文件夹，执行

```Bash
/path/to/phpize &&  ./configure && make && make install
```

```Bssh
echo "extension=donkeyid.so" >> /path/to/php.ini
```

###运行
####api接口
* boolean setNodeId($node_id);
* string getNextId();
* string parseTime($id);
* string parseNodeId($id);

####测试代码
```php

    $donkey = new DonkeyId();
    $donkey->setNodeId(11); //0-511 不要超过这个值
    $id = $donkey->getNextId();
    $time = $donkey->parseTime($id);  //返回的是1970-1-1 00:00:00 到生成事件的毫秒数
    $node = $donkey->parseNodeId($id); //返回生成这个id的节点号
    
    echo "donkeyid=".$id."\n";
    echo "time=".date("Y-m-d H:i:s",$time/1000)."\n";
    echo "node=".$node."\n";
   
```