# DonkeyID---64位自增ID生成器
##原理
	参考Twitter-Snowflake 算法,扩展了其中的细节。具体组成如下图：
	
![bits.jpg](https://github.com/osgochina/donkeyid/blob/master/doc/bits.jpg?raw=true)

    如果所示，64bits 咱们分成了4个部分。
    第一段是给用户自定义端，因为最后生成的id需要乘10,需要进位，所以预留了位置。
    第二段是毫秒级的时间戳，有42个bit.能够使用139年，从1970年开始计算，能使用到2109年，当然这些是可以扩展的。
    第三段是自定义节点id，防止多进程运行产生重复id,能够512个节点。
    第四段是进程内毫秒时间自增序号。一毫秒能产生256个id。也就是说并发1秒能产生256000个id。
##使用
###安装
    下载代码到本地，进入项目文件夹，执行

```Bash
/path/to/phpize &&  ./configure && make && make install
```

```Bssh
echo "extension=donkeyid.so" >> /path/to/php.ini
```
    
###运行
####api接口
* set_node_id($nodeid);
* get_next_id();
* get_time_by_donkeyid($id);
* get_node_by_donkeyid($id);

####测试代码
```php

    $donkey = new DonkeyId();
    $donkey->set_node_id(11);
    $id = $donkey->get_next_id();
    $time = $donkey->get_time_by_donkeyid($id);
    $node = $donkey->get_node_by_donkeyid($id);
    
    echo "donkeyid=".$id."\n";
    echo "time=".date("Y-m-d H:i:s",$time/1000)."\n";
    echo "node=".$node."\n";
   
```