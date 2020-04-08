# DonkeyID---php扩展-64位自增ID生成器
[0.7版本请访问](https://github.com/osgochina/donkeyid/tree/donkeyid-0.7)
##原理
	参考Twitter-Snowflake 算法,扩展了其中的细节。具体组成如下图：
	
![bits.jpg](https://github.com/osgochina/donkeyid/blob/master/doc/bits.png?raw=true)

> 如图所示，64bits 咱们分成了4个部分。

> 1. 毫秒级的时间戳,有42个bit.能够使用139年，从1970年开始计算，能使用到2109年，当然这些是可以扩展的,可以通知指定起始时间来延长这个日期长度。
> 2. 自定义节点id,防止多进程运行产生重复id,占位12个bit,能够支持4096个节点。部署的时候可以配置好服务器id;
> 4. ~~进程workerid，占位5bit，能够生成32个进程id。根据pid运算获得。(已经取消)~~
> 4. 进程内毫秒时间自增序号。占位10bit,一毫秒能产生1024个id。也就是说并发1秒能产生1024000个id。

###唯一性保证
> 100%唯一性保证,根据nodeid的不一样保证多服务器的唯一性，使用共享内存+自旋锁保证单节点多进程的唯一性
> 同一毫秒内自增变量保证并发的唯一性。



##使用
###安装

> 下载代码到本地，进入项目文件夹，执行

```Bash
cd ./donkeyid
/path/to/phpize
./configure --with-php-config=/path/to/php-config
make
make install
```

```Bssh
echo "extension=donkeyid.so" >> /path/to/php.ini
```
### 模式介绍

> DonkeyId 有三种id生成模式：

> 1. 默认模式,以上的介绍都是基于默认模式,(mysql字段请使用bigint(20)). dk_get_next_id()。
> 2. 第二种模式是10进制模式 生成最多20位数字,(mysql字段请使用varchar(20)).从右开始算第十位以后的数字是时间戳的秒，
>    第7位到第9位 是节点id。三位数字，最多到999.从第2位到第6位是秒内的自增id,
>     最后一位是留给业务方的自定义位数。2016053010150316300120001.dk_get_ts_id().
> 3. 第三种模式是字符串模式，生成一个25位的字符串,前17位是年月日时分秒毫秒,第18位到21位是节点id，第22-25位是毫秒内自增id,(mysql字段请使用varchar(25))。dk_get_dt_id().

### 运行

* 注意，该扩展不支持php的线程安全模式,请在非线程安全模式下使用。

#### 配置

> 在php.ini 中配置节点id

```
[DonkeyId]
;0-4095
donkeyid.node_id=0
;0-当前时间戳
donkeyid.epoch=0

```

####api接口

* dk_get_next_id()

> 获取基于Snowflake算法的id

* dk_get_next_ids($num,$time=0)

> 获取基于Snowflake算法的id列表.$num:生成id的数量,$time:需要生成指定时间的id.$time 默认为0 生成当前时间指定数量的id

* dk_parse_id($id)

> 解析基于Snowflake算法的id元数据,返回值包括:time id生成时间，node_id 节点id，sequence 自增数

* dk_get_ts_id()

> 获取10进制的时间戳类型的id

* dk_get_ts_ids($num,$time=0)

> 获取10进制的时间戳类型的id列表.$num:生成id的数量,$time:需要生成指定时间的id.$time 默认为0 生成当前时间指定数量的id

* dk_parse_ts_id($tsid)

> 解析10进制的时间戳类型的id元数据,返回值包括:time id生成时间，node_id 节点id，sequence 自增数

* dk_get_dt_id()

> 获取字符串类型的id，显式包含日期时间属性

####测试代码

```php

    $nextid =  dk_get_next_id();
    echo "nextid:".$nextid."\n";
    print_r(dk_parse_id($nextid));
    
    $tsid =  dk_get_ts_id();
    echo "tsid:".$tsid."\n";
    print_r(dk_parse_ts_id($tsid));
    
    $dtid = dk_get_dt_id();
    echo "dtid:".$dtid."\n";
    
    echo "nextids:\n";
    print_r(dk_get_next_ids(100,1470298401));
    echo "nextids2:\n";
    print_r(dk_get_next_ids(100));
    
    echo "tsids:\n";
    print_r(dk_get_ts_ids(100,1470298401));
    echo "tsids2:\n";
    print_r(dk_get_ts_ids(100));
   
```
#### 支持版本
> 支持 php5.3+ ,支持php 7 - 7.4