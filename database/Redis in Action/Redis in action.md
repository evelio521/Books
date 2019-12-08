##Redis in action

### Chapter 1

####Redis 体统的五中数据结构类型

![img](https://img-blog.csdnimg.cn/20190425140401529.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Z1dHVyZV9MTA==,size_16,color_FFFFFF,t_70)

##### 1. String

```c++
> set hello world
> get hello
> del hello
> incr key-name
> decr key-name
> incrby key-name amount
> decrby key-name amount
> incrbyfloat key-name amount
> append key-name value
> getrange key-name start end
> setrange key-name offset value
> getbit key-name offset
> setbit key-name offset value
> bitcount key-name [start end]
> vitop operator dest-key key-name [key-name ....]
```



##### 2. List

```
> rpush list-key item
> rpush list-key item2
> rpush(lpush) list-key item
> lrange list-key 0 -1
> lindex list-key 1
> lpop(rpop) list-key
> lrange list-key 0 -1
> rpush key-name value [value ...]
> lpush key-name value [value ...]
> rpop key-name
> lpop key-name
> lindex key-name offset
> lrange key-name start end
> ltrim key-name start end
> blpop key-name [key-name ...] timeout
> brpop key-name [key-name ...] timeout
> rpoplpush source-key dest-key
> brpoplpush source-key dest-key timeout
```



##### 3. Hash

```
> hset hash-key sub-key1 value1
> hset hash-key sub-key2 value2
> hgetall hash-key
> hdel hash-key sub-key2
> hget hash-key sub-key1
> hmget key-name key [key ....]
> hmset key-name key value [key value ....]
> hdel key-name key [key ....]
> hlen key-name
> hexists key-name key
> hkeys key-name
> hvals key-name
> hgetall key-name
> hincrby key-name key increment
> hincrbyfloat key-name key increment

```

##### 4. Set

```
> sadd set-key item
> sadd set-key item2
> sadd set-key item3
> smembers set-key
> sismembers set-key item4
> srem set-key item
> smembers set-key
> sadd key-name item [item ....]
> srem key-name item [item ....]
> sismember key-name item
> scard key-name
> smembers key-name
> srandmember key-name [count]
> spop key-name
> smove source-key dest-key item
> sdiffstore dest-key key-name [key-name ....]
> sinter key-name [key-name ....]
> sinterstore dest-key key-name [key-name ...]
> sunion key-name [key-name ...]
> sunionstore dest-key key-name [key-name ...]
```

##### 5. ZSet

```
> zadd zset-key 728 mber1
> zadd zset-key 928 mber2
> zrange zset-key 0 -1 withscores
> zrangebyscore zset-key 0 800 withscores
> zrem zset-key  mber1
> zadd key-name score member [score member ....]
> zrem key-name member [member ...]
> zcard key-name
> zincrby key-name increment member
> zcount key-name min max
> zrank key-name member
> zscore key-name member
> zrange key-name start stop [withscores]
> zrevrank key-name member
> zrevrange key-name start stop [withscores]
> zrangebyscore key min max [withscores] [limit offset count]
> zrevrangebyscore key min max [withscores] [limit offset count]
> zremrangebyrank key-name start stop
> zremrangebyscore key-name min max
```



##### 文章投票系统Redis设计

<img src="/Users/sunqiang/Library/Application Support/typora-user-images/image-20191119164840565.png" alt="image-20191119164840565" style="zoom:50%;" />

<img src="/Users/sunqiang/Library/Application Support/typora-user-images/image-20191119165226332.png" alt="image-20191119165226332" style="zoom:50%;" />

<img src="/Users/sunqiang/Library/Application Support/typora-user-images/image-20191119165300082.png" alt="image-20191119165300082" style="zoom:50%;" />

<img src="/Users/sunqiang/Library/Application Support/typora-user-images/image-20191119165325493.png" alt="image-20191119165325493" style="zoom:50%;" />

### Chapter 2 使用Redis构建Web应用

> 1. 登陆和cookie缓存，如果令牌cookie超过一定数量，要清楚旧的令牌cookie。
> 2. 购物车信息使用令牌cookies 的存储固定用户的购物车信息。
> 3. 缓存不经常变的网页信息，经常变动的网页不缓存，一般来说网页缓存需要5分钟有效期。
> 4. 持续运行一个守护进程函数，让这个函数将数据指定行存入redis，并不定期更新。
> 5. 缓存最受欢迎的商品信息，通过用户cookie的购物车可以统计

### Chapter 3 核心概念

#### 发布与订阅命令

```
> subscribe channel [channel ...]
> unsubscribe [channel [channel ....]]
> publish channel message
> psubscribe pattern [pattern ...]
> punsubscribe pattern [pattern ...]
```

#### 不使用redis自带发布与订阅命令的原因

> 1. 和redis的系统稳定性有关。如果一个客户端订阅了太多的消息，它的读取速度又不够块的话，会导致redis速度变慢，甚至导致系统崩溃。
>
> 2. 数据传输的可靠性有关。如果网络出现终端的情况，那么客户端将丢失断线期间发布的消息。
>
>    

#### 排序命令

```
> sort source-key [by pattern] [limit offset count] [get pattern [get pattern...]] [asc|desc] [alpha] [store dest-key]
```

#### redis事务

```
> watch 
> multi
> exec
> unwatch
> discard
```

#### 键的过期时间

> 1. 对于列表，集合，散列和有序集合来说，过期时间只能设置整个键值

```
> persist key-name
> ttl key-name
> expire key-name seconds
> expireat key-name timestamp
> pttl key-name
> pexpire key-name milliseconds
> pexpireat key-name timestamp-milliseconds
```

### Chapter 4 数据安全与性能保障

#### 持久化选项

> 1. snapshotting，将某一时刻的所有数据写入磁盘，可以存储在本地或其他服务器。可能因为系统崩溃丢失最后一次snapshotting之后的数据。
> 2. aof(append-only file)，执行写命令，将被执行的写命令复制到磁盘
> 3. snapshotting和aof可以同时使用，也可以都不用

#### snapshotting创建方法

> 1. client 向客户端发送 bgsave，redis调用dork创建一个子进程，子进程负责将快照写入硬盘。
> 2. client 向客户端发送 save，redis在处理完snapshotting之前不再响应其他请求。
> 3. save 配置。 save 60 10000，当60s之内有10000写入，redis自动触发bgsave。
> 4. redis通过shutdown命令接收关闭服务器请求时，或接受一个term信号时，执行save命令。
> 5. 当一个redis服务器连接另外一个服务器，向对方发送stnc命令来开始一次复制操作，那么主服务器会执行bgsave命令。

#### snapshotting 使用场景

> 1. 个人开发
> 2. 对日志进行聚合计算，并且能够承受一定时间段的数据损失
> 3. 数据在几个GB的时候是ok的，十几个GB或者系统剩余内存不多时，不适合

#### AOF

> 1. appendfsync [always | everysec | no]
> 2. bgrewriteaof 移除冗余的命令来减少aof文件大小。
> 3. bgrewriteaof 使redis创建子进程，子进程负责文件重写。

#### 主从服务器配置

> 1. 当从服务器连接主服务器的时候，主服务器执行BGSAVE操作。
> 2. redis启动时，slaveof host port的配置文件，连接主服务器。
> 3. 一个正在运行slaveof no one终止服务器复制操作。
> 4. 也可以发送slaveof host port命令让服务器开始复制一个新的主服务器。
> 5. redis不支持主主复制。
> 6. 为了缓解主从过多导致网络不可用，可以建立树状层次结构的主从链结构。

![image-20191122102714976](/Users/sunqiang/Library/Application Support/typora-user-images/image-20191122102714976.png)

#### 系统故障处理--验证snapshotting和aof

> 1. redis-check-aof  扫描到第一个错误的命令后就停止，后面的记录丢失
> 2. redis-check-dump

####系统故障处理--更换主服务器(A master B slave C new master)

> 1. 向B发送一个SAVE命令，创建一个新的snapshotting
> 2. 将新创建的snapshotting发送给C，并在C上启动Redis
> 3. 让B成为C的slave

####Redis事务

> 1. watch : WATCH命令可以监控一个或多个键，一旦其中有一个键被修改（或删除），之后的事务就不会执行。监控一直持续到EXEC命令（事务中的命令是在EXEC之后才执行的，所以在MULTI命令后可以修改WATCH监控的键值）。
> 2. multi : 多命令开始。
> 3. unwatch : Unwatch 命令用于取消 WATCH 命令对所有 key 的监视。
> 4. discard : Discard 命令用于取消事务，放弃执行事务块内的所有命令。
> 5. exec : 执行多命令。

#### 为什么Redis没有实现典型的加锁功能

> 1. 加锁有可能会造成长时间的等待，redis会尽力减少客户端的等待时间。
> 2. watch是乐观锁，关系数据库是悲观锁。



### Chapter 5 使用redis 构建应用支持程序

#### 使用redis来记录日志

> 1. syslog在514 的tcp和udp端口上接收日志，并负责管理日志。
> 2. 可以用syslog-ng替换syslog。
> 3. redis 记录日志，可以把不同级别的日志写入到不同的List中，使用lpush+ltrim

#### 计数器和统计数据

> 1. 参见原教材

#### IP所属城市以及国家

> 1. 参见原教材

#### 服务器的发现与配置

> 1. 使用Python的装饰器功能  https://www.cnblogs.com/yuzhanhong/p/9180212.html

### Chapter 6 使用redis 构建应用程序组件

#### 自动补全

```
// 自动补全最近联系人 python 程序
```

```
// 通讯录自动补全，需要找到边界
```

#### 分布式锁

> setnx key value

> 1. 使用WATCH来实现锁，并使用MULTI 和 EXEC，这个性能不太好

> 1. 为了防止死锁问题，所有的锁都有超时限制，但是持有锁的进程操作时间不能太长，否则在超过时间限制后可能会自动释放锁。
> 2. 持有锁的进程崩溃，其他进程浪费等待时间。
> 3. 一个进程锁过期后，其他进程同时获得了尝试锁，但是每个进程都以为自己是唯一获得锁的进程。
> 4. redis在最新硬件上可以每秒执行100000个操作，高端硬件上可以执行225000个操作。

```
// 粗力度锁代码
```

```
// 细力度锁代码
```

#### 计数信号量

```
// 基本的技术信号量代码 
```

```
// 公平信号量代码
```

#### 任务队列

```
// 任务队列代码
// 优先级队列代码
// 延迟任务代码
```

#### 消息拉取

```
// publish subscribe 代码（必须要求双方一直在线，否则可能下次丢失）
// publish subscribe 替代代码
```



### Chapter 7 基于搜索的应用程序

#### 使用Redis进行搜索

```
// 思想和代码
```

#### 有序搜索

```
// 思想和代码
```

#### 广告定向

```
// 思想和代码
```

#### 职位搜索

```
// 思想和代码
```

#### 

### Chapter 8 构建简单的社交网站

#### 用户和状态

```
// 设计思想，表设计和代码
```

#### 主页时间线

```
// 设计思想，表设计和代码
```

#### 关注列表和正在关注列表

```
// 设计思想，表设计和代码
```

#### 状态消息的发布和删除

```
// 设计思想，表设计和代码
```

#### 流API

```
// 设计思想，表设计和代码
```

### Chapter 9 降低内存占用

#### 短结构

> 1. 在列表、散列和有序集合的长度较短或者体积较小的时候，Redis可以选择使用一种ziplist的紧凑方式来存储这些结构。
> 2. ziplist是一种非结构化的数据结构，需要解码和编码等。
> 3. 集合的压缩方式是intset

```
// ziplist配置（列表、散列和有序集合的压缩方式）
// 每个结点包含三个字段，上一个结点字段，当前结点字段和值
list-max-ziplist-entries 512 // 被编码为压缩表，允许包含最大元素数量
list-max-ziplist-value 64    // 每个节点的最大体积
hash-max-ziplist-entries 512
hash-max-ziplist-value 64
zset-max-ziplist-entries 128
zset-max-ziplist-value 64

// 判断一个结构是否是压缩表
conn.rpush('test','a','b','c')
conn.debug_object('test') // encoding等于ziplist表示是压缩表格式

```

```
// intset 配置（集合的压缩配置）
set-max-intset-entries 512  // 被编码为intset，允许包含最大元素数量
// 判断是否使用intset
conn.sadd('set-object', *range(500))
conn.debug_pbject('set-object')

```

> 1. 当一个结构突破了用户为压缩表列表或者整数集合设置的限制条件时，Redis就会将他转换为更为典型的底层信息结构

```python
# 性能测试代码，在list-max-ziplist-entries 110000的时候
def long_ziplist_performance(conn, key, length, passes, psize): 
    # 删除指定的键，确保被测试数据的准确性。
    conn.delete(key)                    
    # 通过从右端推入指定数量的元素来对列表进行初始化。
    conn.rpush(key, *range(length))     
    # 通过流水线来降低网络通信给测试带来的影响。
    pipeline = conn.pipeline(False)     

    # 启动计时器。
    t = time.time()                     
    # 根据 passes 参数来决定流水线操作的执行次数。
    for p in xrange(passes):           
        # 每个流水线操作都包含了 psize 次 RPOPLPUSH 命令调用。
        for pi in xrange(psize):        
            # 每个 rpoplpush() 函数调用都会将列表最右端的元素弹出，
            # 并将它推入到同一个列表的左端。
            pipeline.rpoplpush(key, key)
        # 执行 psize 次 RPOPLPUSH 命令。
        pipeline.execute()              

    # 计算每秒钟执行的 RPOPLPUSH 调用数量。
    return (passes * psize) / (time.time() - t or .001)
long_ziplist_performance(conn, 'list', 1, 1000, 100)        
long_ziplist_performance(conn, 'list', 100, 1000, 100)      
long_ziplist_performance(conn, 'list', 1000, 1000, 100)     
long_ziplist_performance(conn, 'list', 5000, 1000, 100)     
long_ziplist_performance(conn, 'list', 10000, 1000, 100)    
long_ziplist_performance(conn, 'list', 50000, 1000, 100)   
long_ziplist_performance(conn, 'list', 100000, 500, 100)  
# entries 1024 value 64 性能比较优
```

```python
// inset 性能问题
def long_ziplist_index(conn, key, length, passes, psize): #A
    conn.delete(key)                    #B
    conn.rpush(key, *range(length))     #C
    length >>= 1
    pipeline = conn.pipeline(False)     #D
    t = time.time()                     #E
    for p in xrange(passes):            #F
        for pi in xrange(psize):        #G
            pipeline.lindex(key, length)#H
        pipeline.execute()              #I
    return (passes * psize) / (time.time() - t or .001) #J

def long_intset_performance(conn, key, length, passes, psize): #A
    conn.delete(key)                    #B
    conn.sadd(key, *range(1000000, 1000000+length))     #C
    cur = 1000000-1
    pipeline = conn.pipeline(False)     #D
    t = time.time()                     #E
    for p in xrange(passes):            #F
        for pi in xrange(psize):        #G
            pipeline.spop(key)#H
            pipeline.sadd(key, cur)
            cur -= 1
        pipeline.execute()              #I
    return (passes * psize) / (time.time() - t or .001) #J
```



#### 分片结构

#####分片式散列

> 1. 使用Lua进行分片是比较容易。
> 2. 散列、集合和有序集合可以使用分片。
> 3. 分片是基于某些简单的规则，把数据划分为更小的部分

```python
# 在调用 shard_key() 函数时，
# 用户需要给定基础散列的名字、将要被储存到分片散列里面的键、预计的元素总数量以及请求的分片数量。
def shard_key(base, key, total_elements, shard_size):  
    # 如果值是一个整数或者一个看上去像是整数的字符串，
    # 那么它将被直接用于计算分片 ID 。
    if isinstance(key, (int, long)) or key.isdigit():  
        # 整数键将被程序假定为连续指派的 ID ，
        # 并基于这个整数 ID 的二进制位的高位来选择分片 ID 。
        # 此外，程序在进行整数转换的时候还使用了显式的基数（以及 str()`` 函数），
        # 使得键 010 可以被转换为 10 ，而不是 8 。
        shard_id = int(str(key), 10) // shard_size     
    else:
        # 对于不是整数的键，
        # 程序将基于预计的元素总数量以及请求的分片数量，
        # 计算出实际所需的分片总数量。
        shards = 2 * total_elements // shard_size      
        # 在得知了分片的数量之后，
        # 程序就可以通过计算键的散列值与分片数量之间的模数来得到分片 ID 。
        shard_id = binascii.crc32(key) % shards         
    # 最后，程序会把基础键和分片 ID 组合在一起，得出分片键。
    return "%s:%s"%(base, shard_id)   
  
def shard_hset(conn, base, key, value, total_elements, shard_size):
    # 计算出应该由哪个分片来储存值。
    shard = shard_key(base, key, total_elements, shard_size)   
    # 将值储存到分片里面。
    return conn.hset(shard, key, value)                        

def shard_hget(conn, base, key, total_elements, shard_size):
    # 计算出值可能被储存到了哪个分片里面。
    shard = shard_key(base, key, total_elements, shard_size)    
    # 取得储存在分片里面的值。
    return conn.hget(shard, key)  
  
TOTAL_SIZE = 320000                                             # 把传递给分片函数的参数设置为全局常量，
SHARD_SIZE = 1024                                               # 确保每次传递的值总是相同的。

def import_cities_to_redis(conn, filename):
    for row in csv.reader(open(filename)):
        ...
        # 为了对数据进行设置，用户需要传递 TOTAL_SIZE 参数和 SHARD_SIZE 参数。
        shard_hset(conn, 'cityid2city:', city_id, 
            json.dumps([city, region, country]),         # 不过因为这个程序处理的 ID 都是数字，
            TOTAL_SIZE, SHARD_SIZE)                      # 所以 TOTAL_SIZE 实际上并没有被使用。

def find_city_by_ip(conn, ip_address):
    ...
    data = shard_hget(conn, 'cityid2city:', city_id,            # 程序在获取数据时，
        TOTAL_SIZE, SHARD_SIZE)    # 需要根据相同的 TOTAL_SIZE 参数和 SHARD_SIZE 参数查找被分片的键。
    return json.loads(data)
```

##### 分片式集合

> 1. uuid的前56个十六进制数字转成整数进行存储，可以节省空间

```python
def shard_sadd(conn, base, member, total_elements, shard_size):
    shard = shard_key(base,
        # 计算成员应该被储存到哪个分片集合里面；
        # 因为成员并非连续 ID ，所以程序在计算成员所属的分片之前，会先将成员转换为字符串。
        'x'+str(member), total_elements, shard_size)           
    # 将成员储存到分片里面。
    return conn.sadd(shard, member)  
# 为整数集合编码的集合预设一个典型的分片大小。
SHARD_SIZE = 512                       

def count_visit(conn, session_id):
    # 取得当天的日期，并生成唯一访客计数器的键。
    today = date.today()                               
    key = 'unique:%s'%today.isoformat()                
    # 计算或者获取当天的预计唯一访客人数。
    expected = get_expected(conn, key, today)          
 
    # 根据 128 位的 UUID ，计算出一个 56 位的 ID 。
    id = int(session_id.replace('-', '')[:15], 16)     
    # 将 ID 添加到分片集合里面。
    if shard_sadd(conn, key, id, expected, SHARD_SIZE): 
        # 如果 ID 在分片集合里面并不存在，那么对唯一访客计数器执行加一操作。
        conn.incr(key)    
 # 这个初始的预计每日访客人数会设置得稍微比较高一些。
DAILY_EXPECTED = 1000000                               
# 在本地储存一份计算得出的预计访客人数副本。
EXPECTED = {}                                          

def get_expected(conn, key, today):
    # 如果程序已经计算出或者获取到了当日的预计访客人数，
    # 那么直接使用已计算出的数字。
    if key in EXPECTED:                                
        return EXPECTED[key]                            
 
    exkey = key + ':expected'
    # 如果其他客户端已经计算出了当日的预计访客人数，
    # 那么直接使用已计算出的数字。
    expected = conn.get(exkey)                         
 
    if not expected:
        # 获取昨天的唯一访客人数，如果该数值不存在就使用默认值一百万。
        yesterday = (today - timedelta(days=1)).isoformat() 
        expected = conn.get('unique:%s'%yesterday)          
        expected = int(expected or DAILY_EXPECTED)          
 
        # 基于“明天的访客人数至少会比今天的访客人数多 50%”这一假设，
        # 给昨天的访客人数加上 50% ，然后向上舍入至下一个底数为 2 的幂。
        expected = 2**int(math.ceil(math.log(expected*1.5, 2))) 
        # 将计算出的预计访客人数写入到 Redis 里面，以便其他程序在有需要时使用。
        if not conn.setnx(exkey, expected):                 
            # 如果在我们之前，
            # 已经有其他客户端储存了当日的预计访客人数，
            # 那么直接使用已储存的数字。
            expected = conn.get(exkey)                      
 
    # 将当日的预计访客人数记录到本地副本里面，并将它返回给调用者。
    EXPECTED[key] = int(expected)                       
    return EXPECTED[key]    
```

#### 打包存储二进制位和字节

> 1. getrange setrange setbit getbit

```python
# 调用 split() 函数会根据空白对这个字符串进行分割，
# 并将它转换为一个由国家编码组成的列表。
COUNTRIES = '''
ABW AFG AGO AIA ALA ALB AND ARE ARG ARM ASM ATA ATF ATG AUS AUT AZE BDI
BEL BEN BES BFA BGD BGR BHR BHS BIH BLM BLR BLZ BMU BOL BRA BRB BRN BTN
BVT BWA CAF CAN CCK CHE CHL CHN CIV CMR COD COG COK COL COM CPV CRI CUB
CUW CXR CYM CYP CZE DEU DJI DMA DNK DOM DZA ECU EGY ERI ESH ESP EST ETH
FIN FJI FLK FRA FRO FSM GAB GBR GEO GGY GHA GIB GIN GLP GMB GNB GNQ GRC
GRD GRL GTM GUF GUM GUY HKG HMD HND HRV HTI HUN IDN IMN IND IOT IRL IRN
IRQ ISL ISR ITA JAM JEY JOR JPN KAZ KEN KGZ KHM KIR KNA KOR KWT LAO LBN
LBR LBY LCA LIE LKA LSO LTU LUX LVA MAC MAF MAR MCO MDA MDG MDV MEX MHL
MKD MLI MLT MMR MNE MNG MNP MOZ MRT MSR MTQ MUS MWI MYS MYT NAM NCL NER
NFK NGA NIC NIU NLD NOR NPL NRU NZL OMN PAK PAN PCN PER PHL PLW PNG POL
PRI PRK PRT PRY PSE PYF QAT REU ROU RUS RWA SAU SDN SEN SGP SGS SHN SJM
SLB SLE SLV SMR SOM SPM SRB SSD STP SUR SVK SVN SWE SWZ SXM SYC SYR TCA
TCD TGO THA TJK TKL TKM TLS TON TTO TUN TUR TUV TWN TZA UGA UKR UMI URY
USA UZB VAT VCT VEN VGB VIR VNM VUT WLF WSM YEM ZAF ZMB ZWE'''.split()

STATES = {
    # 加拿大的省信息和属地信息。
    'CAN':'''AB BC MB NB NL NS NT NU ON PE QC SK YT'''.split(),      
    # 美国各个州的信息。
    'USA':'''AA AE AK AL AP AR AS AZ CA CO CT DC DE FL FM GA GU HI IA ID
IL IN KS KY LA MA MD ME MH MI MN MO MP MS MT NC ND NE NH NJ NM NV NY OH
OK OR PA PR PW RI SC SD TN TX UT VA VI VT WA WI WV WY'''.split(),     
}

def get_code(country, state):
    # 寻找国家对应的偏移量。
    cindex = bisect.bisect_left(COUNTRIES, country)             
    # 没有找到指定的国家时，将索引设置为 -1 。
    if cindex > len(COUNTRIES) or COUNTRIES[cindex] != country: 
        cindex = -1                                             
    # 因为 Redis 里面的未初始化数据在返回时会被转换为空值，
    # 所以我们要将“未找到指定国家”时的返回值改为 0 ，
    # 并将第一个国家的索引变为 1 ，以此类推。
    cindex += 1                                                 

    sindex = -1
    if state and country in STATES:
        # 尝试取出国家对应的州信息。
        states = STATES[country]                               
        # 寻找州对应的偏移量。
        sindex = bisect.bisect_left(states, state)             
        # 像处理“未找到指定国家”时的情况一样，处理“未找到指定州”的情况。
        if sindex > len(states) or states[sindex] != state:     
            sindex = -1                                        
    # 如果没有找到指定的州，那么索引为 0 ；
    # 如果找到了指定的州，那么索引大于 0 。
    sindex += 1                                                

    # chr() 函数会将介于 0 至 255 之间的整数值转换为对应的 ASCII 字符。
    return chr(cindex) + chr(sindex)   
  
 # 设置每个分片的大小。
USERS_PER_SHARD = 2**20                                   

def set_location(conn, user_id, country, state):
    # 取得用户所在位置的编码。
    code = get_code(country, state)                        
    
    # 查找分片 ID 以及用户在指定分片中的位置（position）。
    shard_id, position = divmod(user_id, USERS_PER_SHARD)   
    # 计算用户数据的偏移量。
    offset = position * 2                                  

    pipe = conn.pipeline(False)
    # 将用户的位置信息储存到分片后的位置表格里面。
    pipe.setrange('location:%s'%shard_id, offset, code)     

    # 对记录目前已知最大用户 ID 的有序集合进行更新。
    tkey = str(uuid.uuid4())                                
    pipe.zadd(tkey, 'max', user_id)                         
    pipe.zunionstore('location:max',                        
        [tkey, 'location:max'], aggregate='max')            
    pipe.delete(tkey)                                       

    pipe.execute()
```

```python
def aggregate_location(conn):
    # 初始化两个特殊结构，
    # 以便快速地对已存在的计数器以及缺失的计数器进行更新。
    countries = defaultdict(int)                                
    states = defaultdict(lambda:defaultdict(int))               

    # 获取目前已知的最大用户 ID ，
    # 并使用它来计算出程序需要访问的最大分片 ID 。
    max_id = int(conn.zscore('location:max', 'max'))            
    max_block = max_id // USERS_PER_SHARD                       

    # 按顺序地处理每个分片……
    for shard_id in xrange(max_block + 1):                      
        # 读取每个块……
        for block in readblocks(conn, 'location:%s'%shard_id):  
            # 从块里面提取出每个编码，
            # 并根据编码查找原始的位置信息，
            # 然后对这些位置信息进行聚合计算。
            for offset in xrange(0, len(block)-1, 2):           
                code = block[offset:offset+2]
                # 对聚合数据进行更新。
                update_aggregates(countries, states, [code])    

    return countries, states
  
def update_aggregates(countries, states, codes):
    for code in codes:
        # 只对合法的编码进行查找。
        if len(code) != 2:                             
            continue                                   

        # 计算出国家和州在查找表格中的实际偏移量。
        country = ord(code[0]) - 1                     
        state = ord(code[1]) - 1                      
        
        # 如果国家所处的偏移量不在合法范围之内，那么跳过这个编码。
        if country < 0 or country >= len(COUNTRIES):    
            continue                                   

        # 获取 ISO3 国家编码。
        country = COUNTRIES[country]                   
        # 在对国家信息进行解码之后，
        # 把用户计入到这个国家对应的计数器里面。
        countries[country] += 1                         

        # 如果程序没有找到指定的州信息，
        # 或者查找州信息时的偏移量不在合法的范围之内，
        # 那么跳过这个编码。
        if country not in STATES:                       
            continue                                    
        if state < 0 or state >= STATES[country]:       
            continue                                    

        # 根据编码获取州名。
        state = STATES[country][state]                  
        # 对州计数器执行加一操作。
        states[country][state] += 1 
        
 def aggregate_location_list(conn, user_ids):
    # 设置流水线，减少操作执行过程中与 Redis 的通信往返次数。
    pipe = conn.pipeline(False)                                
    #  和之前一样，设置好基本的聚合数据。
    countries = defaultdict(int)                               
    states = defaultdict(lambda: defaultdict(int))             

    for i, user_id in enumerate(user_ids):
        # 查找用户位置信息所在分片的 ID ，以及信息在分片中的偏移量。
        shard_id, position = divmod(user_id, USERS_PER_SHARD)   
        offset = position * 2                                   

        # 发送另一个被流水线包裹的命令，获取用户的位置信息。
        pipe.substr('location:%s'%shard_id, offset, offset+1)   

        # 每处理 1000 个请求，
        # 程序就会调用之前定义的辅助函数对聚合数据进行一次更新。
        if (i+1) % 1000 == 0:                                   
            update_aggregates(countries, states, pipe.execute())

    # 对遍历余下的最后一批用户进行处理。
    update_aggregates(countries, states, pipe.execute())        

    # 返回聚合数据。
    return countries, states      
```

### Chapter 10 扩展redis

#### 扩展读性能

##### 1. 提高读性能的几个途径

> 1. 短结构时，确保压缩列表的最大长度不会太大以至于影响性能。
> 2. 根据程序要执行的查询类型，选择能够为这种查询提供最好性能的结构。不要把列表当集合使用，不要取出数据在进行排序，应该直接用有序集合。
> 3. 对于大体积的数据，应该先压缩在存储。

##### 2.提高读能力方法

> 1. 最简单的是添加制度服务器。添加主从服务器。
> 2. 构建服务树，减少每个机器都从主服务器传输snapshotting的需求
> 3. redis sentinel 是 redis 的高可用实现方案，多个 sentinel 进程协同工作，组成了一套分布式的架构，它负责持续监控主从节点的健康状况，当主节点挂掉时，自动选择一个最优的从节点切换为主节点。客户端来连接集群时，会首先连接 sentinel，通过 sentinel 来查询主节点的地址，然后再去连接主节点进行数据交互。当主节点发生故障时，客户端会重新向 sentinel 要地址，sentinel 会将最新的主节点地址告诉客户端。如此应用程序将无需重启即可自动完成节点切换。

#### 扩展写性能和内存容量

##### 1. 提高写性能的几个途径

> 1. 对自己编写的方法进行检查，尽量减少程序需要读取的数据量。
> 2. 将无关的功能迁至其他服务器。
> 3. 在对redis写入之前，尝试对本地内存中的数据进行聚合操作。
> 4. 使用锁替换可能会给速度带来限制的WATCH/MULTI/EXEC事务

##### 2.提高写能力方法

> 1. 数据分片到多台服务器。

```python
def get_redis_connection(component, wait=1):
    key = 'config:redis:' + component
    # 尝试获取旧的配置。
    old_config = CONFIGS.get(key, object())           
    # 尝试获取新的配置。
    config = get_config(                                
        config_connection, 'redis', component, wait)    

    # 如果新旧配置不相同，那么创建一个新的连接。
    if config != old_config:                            
        REDIS_CONNECTIONS[key] = redis.Redis(**config)  

    # 返回用户指定的连接对象。
    return REDIS_CONNECTIONS.get(key)   
def get_sharded_connection(component, key, shard_count, wait=1):
    # 计算出 “<组件名>:<分片数字>” 格式的分片 ID 。
    shard = shard_key(component, 'x'+str(key), shard_count, 2)  
    # 返回连接。
    return get_redis_connection(shard, wait)  

# 装饰器接受组件名以及预期的分片数量作为参数。
def sharded_connection(component, shard_count, wait=1):        
    # 创建一个包装器，使用它去装饰传入的函数。
    def wrapper(function):                                     
        # 从原始函数里面复制一些有用的元信息到配置处理器。
        @functools.wraps(function)                            
        # 创建一个函数，它负责计算键的分片 ID ，并对连接管理器进行设置。
        def call(key, *args, **kwargs):                        
            # 获取分片连接。
            conn = get_sharded_connection(                     
                component, key, shard_count, wait)            
            # 实际地调用被装饰的函数，并将分片连接以及其他参数传递给它。
            return function(conn, key, *args, **kwargs)        
        # 返回被包装后的函数。
        return call                                             
    # 返回一个函数，它可以对需要分片连接的函数进行包装。
    return wrapper    
  
 # 将 count_visit() 函数分片到 16 台机器上面执行，
# 执行所得的结果将被自动地分片到每台机器的多个数据库键上面。
@sharded_connection('unique', 16)                      
def count_visit(conn, session_id):
    today = date.today()
    key = 'unique:%s'%today.isoformat()
    # 经过修改的 get_expected() 调用。
    conn2, expected = get_expected(key, today)        

    id = int(session_id.replace('-', '')[:15], 16)
    if shard_sadd(conn, key, id, expected, SHARD_SIZE):
        # 使用 get_expected() 函数返回的非分片（nonsharded）连接，
        # 对唯一计数器执行自增操作。
        conn2.incr(key)                               

# 对 get_expected() 函数使用非分片连接。
@redis_connection('unique')                            
def get_expected(conn, key, today):
    'all of the same function body as before, except the last line'
    # 返回非分片连接，
    # 使得 count_visit() 函数可以在有需要的时候，
    # 对唯一计数器执行自增操作。
    return conn, EXPECTED[key]     
```

#### 扩展复杂的查询

> 1. 添加只读从服务器可以对redis处理读查询能力进行扩展，slave-read-only 设置为yes

```python
# 通过组织数据来支持分片搜索查询
# 基于SORT命令实现的搜索操作进行分片
# 这个函数接受的参数与 search_and_sort() 函数接受的参数完全相同。
def search_get_values(conn, query, id=None, ttl=300, sort="-updated", 
                      start=0, num=20):                               
    # 首先取得搜索操作和排序操作的执行结果。
    count, docids, id = search_and_sort(                           
        conn, query, id, ttl, sort, 0, start+num)                  

    key = "kb:doc:%s"
    sort = sort.lstrip('-')

    pipe = conn.pipeline(False)
    # 根据结果的排序方式来获取数据。
    for docid in docids:                                           
        pipe.hget(key%docid, sort)                                 
    sort_column = pipe.execute()                                   

    # 将文档 ID 以及对文档进行排序产生的数据进行配对（pair up）。
    data_pairs = zip(docids, sort_column)                          
    # 返回结果包含的文档数量、排序之后的搜索结果以及结果的缓存 ID 。
    return count, data_pairs, id 
# 程序为了获知自己要连接的服务器，
# 会假定所有分片服务器的信息都记录在一个标准的配置位置里面。
def get_shard_results(component, shards, query, ids=None, ttl=300,  
                  sort="-updated", start=0, num=20, wait=1):        

    # 准备一些结构，用于储存之后获取的数据。
    count = 0      
    data = []      
    # 尝试使用已被缓存的搜索结果；
    # 如果没有缓存结果可用，那么重新执行查询。
    ids = ids or shards * [None]       
    for shard in xrange(shards):
        # 获取或者创建一个连向指定分片的连接。
        conn = get_redis_connection('%s:%s'%(component, shard), wait)
        # 获取搜索结果以及它们的排序数据。
        c, d, i = search_get_values(                        
            conn, query, ids[shard], ttl, sort, start, num) 

        # 将这个分片的计算结果与其他分片的计算结果进行合并。
        count += c          
        data.extend(d)     
        ids[shard] = i      

    # 把所有分片的原始（raw）计算结果返回给调用者。
    return count, data, ids   
 def to_numeric_key(data):
    try:
        # 这里之所以使用 Decimal 数字类型，
        # 是因为这种类型可以合理地对整数和浮点数进行转换，
        # 并在值缺失或者不是数字值的时候，
        # 返回默认值 0 。
        return Decimal(data[1] or '0')     
    except:
        return Decimal('0')               

def to_string_key(data):
    # 总是返回一个字符串，即使在值缺失的情况下，也是如此。
    return data[1] or ''                   

# 这个函数需要接受所有分片参数和搜索参数，
# 这些参数大部分都会被传给底层的函数，
# 而这个函数本身只会用到 sort 参数以及搜索偏移量。
def search_shards(component, shards, query, ids=None, ttl=300,     
                  sort="-updated", start=0, num=20, wait=1):       

    # 获取未经排序的分片搜索结果。
    count, data, ids = get_shard_results(                           
        component, shards, query, ids, ttl, sort, start, num, wait) 

    # 准备好进行排序所需的各个参数。
    reversed = sort.startswith('-')                    
    sort = sort.strip('-')                             
    key = to_numeric_key                               
    if sort not in ('updated', 'id', 'created'):        
        key = to_string_key                             

    # 根据 sort 参数对搜索结果进行排序。
    data.sort(key=key, reverse=reversed)               

    results = []
    # 只获取用户指定的那一页搜索结果。
    for docid, score in data[start:start+num]:         
        results.append(docid)                          

    # 返回被选中的结果，其中包括由每个分片的缓存 ID 组成的序列。
    return count, results, ids   
```

```python
# 对基于有序集合实现的搜索操作进行分片
# 这个函数接受 search_and_zsort() 函数所需的全部参数。
def search_get_zset_values(conn, query, id=None, ttl=300, update=1, 
                    vote=0, start=0, num=20, desc=True):            

    # 调用底层的 search_and_zsort() 函数，
    # 获取搜索结果的缓存 ID 以及结果包含的文档数量。
    count, r, id = search_and_zsort(                                
        conn, query, id, ttl, update, vote, 0, 1, desc)             

    # 获取指定的搜索结果以及这些结果的分值。
    if desc:                                                        
        data = conn.zrevrange(id, 0, start + num - 1, withscores=True)
    else:                                                          
        data = conn.zrange(id, 0, start + num - 1, withscores=True) 

    # 返回搜索结果的数量、搜索结果本身、搜索结果的分值以及搜索结果的缓存 ID 。
    return count, data, id                                          

# 函数需要接受所有分片参数以及所有搜索参数。
def search_shards_zset(component, shards, query, ids=None, ttl=300,   
                update=1, vote=0, start=0, num=20, desc=True, wait=1):

    # 准备一些结构，用于储存之后获取到的数据。
    count = 0                       
    data = []                       
    # 尝试使用已有的缓存结果；
    # 如果没有缓存结果可用，那么开始一次新的搜索。
    ids = ids or shards * [None]    
    for shard in xrange(shards):
        # 获取或者创建指向每个分片的连接。
        conn = get_redis_connection('%s:%s'%(component, shard), wait) 
        # 在分片上面进行搜索，并取得搜索结果的分值。
        c, d, i = search_get_zset_values(conn, query, ids[shard],     
            ttl, update, vote, start, num, desc)                      

        # 对每个分片的搜索结果进行合并。
        count += c      
        data.extend(d)  
        ids[shard] = i  

    # 定义一个简单的排序辅助函数，让它只返回与分值有关的信息。
    def key(result):       
        return result[1]   

    # 对所有搜索结果进行排序。
    data.sort(key=key, reversed=desc)   
    results = []
    # 从结果里面提取出文档 ID ，并丢弃与之关联的分值。
    for docid, score in data[start:start+num]:  
        results.append(docid)                  

    # 将搜索结果返回给调用者。
    return count, results, ids 
```

##### 对社交网站进行扩展

> 1. 对社交网站的扩展，首先找出经常被读取的数据和经常写入的数据，并考虑将常用的数据和不常用的数据分开。

```python
# 对时间线进行分片
class KeyShardedConnection(object):
    # 对象使用组件名字以及分片数量进行初始化。
    def __init__(self, component, shards):        
        self.component = component                 
        self.shards = shards                      
    # 当用户尝试从对象里面获取一个元素的时候，
    # 这个方法就会被调用，
    # 而调用这个方法时传入的参数就是用户请求的元素。
    def __getitem__(self, key):                    
        # 根据传入的键以及之前已知的组件名字和分片数量，
        # 获取分片连接。
        return get_sharded_connection(             
            self.component, key, self.shards) 
      
# 创建一个连接，这个连接包含对拥有指定分片数量的组件进行分片所需的相关信息。
sharded_timelines = KeyShardedConnection('timelines', 8)   

def follow_user(conn, uid, other_uid):
    fkey1 = 'following:%s'%uid
    fkey2 = 'followers:%s'%other_uid

    if conn.zscore(fkey1, other_uid):
        print "already followed", uid, other_uid
        return None

    now = time.time()

    pipeline = conn.pipeline(True)
    pipeline.zadd(fkey1, other_uid, now)
    pipeline.zadd(fkey2, uid, now)
    pipeline.zcard(fkey1)
    pipeline.zcard(fkey2)
    following, followers = pipeline.execute()[-2:]
    pipeline.hset('user:%s'%uid, 'following', following)
    pipeline.hset('user:%s'%other_uid, 'followers', followers)
    pipeline.execute()

    pkey = 'profile:%s'%other_uid
    # 从正在关注的用户的个人时间线里面，取出最新的状态消息。
    status_and_score = sharded_timelines[pkey].zrevrange(   
        pkey, 0, HOME_TIMELINE_SIZE-1, withscores=True)     

    if status_and_score:
        hkey = 'home:%s'%uid
        # 根据被分片的键获取一个连接，然后通过连接获取一个流水线对象。
        pipe = sharded_timelines[hkey].pipeline(True)       
        # 将一系列状态消息添加到位于分片上面的定制时间线有序集合里面，
        # 并在添加操作完成之后，对有序集合进行修剪。
        pipe.zadd(hkey, **dict(status_and_score))           
        pipe.zremrangebyrank(hkey, 0, -HOME_TIMELINE_SIZE-1)
        # 执行事务。
        pipe.execute()                                      

    return True
```

```python
# 通过分片对关注者列表以及正在关注列表进行扩展
# 把关注者列表和正在关注列表有序集合划分到多个分片上面
class KeyDataShardedConnection(object):
    # 对象使用组件名和分片数量进行初始化。
    def __init__(self, component, shards):         
        self.component = component                 
        self.shards = shards                       
    # 当一对 ID 作为字典查找操作的其中一个参数被传入时，
    # 这个方法将被调用。
    def __getitem__(self, ids):                   
        # 取出两个 ID ，并确保它们都是整数。
        id1, id2 = map(int, ids)                   
        # 如果第二个 ID 比第一个 ID 要小，
        # 那么对调两个 ID 的位置，
        # 从而确保第一个 ID 总是小于或等于第二个 ID 。
        if id2 < id1:                              
            id1, id2 = id2, id1                    
        # 基于两个 ID 构建出一个键。
        key = "%s:%s"%(id1, id2)                    
        # 使用构建出的键以及之前已知的组件名和分片数量，
        # 获取分片连接。
        return get_sharded_connection(             
            self.component, key, self.shards)  
_follow_user = follow_user
# 创建一个连接，
# 这个连接包含对拥有指定分片数量的组件进行分片所需的相关信息。
sharded_timelines = KeyShardedConnection('timelines', 8)        
sharded_followers = KeyDataShardedConnection('followers', 16)   

def follow_user(conn, uid, other_uid):
    fkey1 = 'following:%s'%uid
    fkey2 = 'followers:%s'%other_uid

    # 根据 uid 和 other_uid 获取连接对象。
    sconn = sharded_followers[uid, other_uid]          
    # 检查 other_uid 代表的用户是否已经关注了 uid 代表的用户。
    if sconn.zscore(fkey1, other_uid):                 
        return None

    now = time.time()
    spipe = sconn.pipeline(True)
    # 把关注者的信息以及被关注者的信息添加到有序集合里面。
    spipe.zadd(fkey1, other_uid, now)                  
    spipe.zadd(fkey2, uid, now)                        
    following, followers = spipe.execute()

    pipeline = conn.pipeline(True)
    # 为执行关注操作的用户以及被关注的用户更新关注者信息和正在关注信息。
    pipeline.hincrby('user:%s'%uid, 'following', int(following))      
    pipeline.hincrby('user:%s'%other_uid, 'followers', int(followers))
    pipeline.execute()

    pkey = 'profile:%s'%other_uid
    status_and_score = sharded_timelines[pkey].zrevrange(
        pkey, 0, HOME_TIMELINE_SIZE-1, withscores=True)

    if status_and_score:
        hkey = 'home:%s'%uid
        pipe = sharded_timelines[hkey].pipeline(True)
        pipe.zadd(hkey, **dict(status_and_score))
        pipe.zremrangebyrank(hkey, 0, -HOME_TIMELINE_SIZE-1)
        pipe.execute()

    return True
# 函数接受组件名称、分片数量以及那些可以在分片环境下产生正确行为的参数作为参数。
def sharded_zrangebyscore(component, shards, key, min, max, num):  
    data = []
    for shard in xrange(shards):
        # 获取指向当前分片的分片连接。
        conn = get_redis_connection("%s:%s"%(component, shard))     
        # 从 Redis 分片上面取出数据。
        data.extend(conn.zrangebyscore(                             
            key, min, max, start=0, num=num, withscores=True))      

    # 首先基于分值对数据进行排序，然后再基于成员进行排序。
    def key(pair):                     
        return pair[1], pair[0]        
    data.sort(key=key)                 

    # 根据用户请求的数量返回元素。
    return data[:num]   
  
def syndicate_status(uid, post, start=0, on_lists=False):
    root = 'followers'
    key = 'followers:%s'%uid
    base = 'home:%s'
    if on_lists:
        root = 'list:out'
        key = 'list:out:%s'%uid
        base = 'list:statuses:%s'

    # 通过 ZRANGEBYSCORE 调用，找出下一组关注者。
    followers = sharded_zrangebyscore(root,                         
        sharded_followers.shards, key, start, 'inf', POSTS_PER_PASS)

    # 基于预先分片的结果对个人信息进行分组，
    # 并把分组后的信息储存到预先准备好的结构里面。
    to_send = defaultdict(list)                            
    for follower, start in followers:
        # 构造出储存时间线的键。
        timeline = base % follower                          
        # 找到负责储存这个时间线的分片。
        shard = shard_key('timelines',                     
            timeline, sharded_timelines.shards, 2)         
        # 把时间线的键添加到位于同一个分片的其他时间线的后面。
        to_send[shard].append(timeline)                    

    for timelines in to_send.itervalues():
        # 根据储存这组时间线的服务器，
        # 找出连向它的连接，
        # 然后创建一个流水线对象。
        pipe = sharded_timelines[timelines[0]].pipeline(False) 
        for timeline in timelines:
            # 把新发送的消息添加到时间线上面，
            # 并移除过于陈旧的消息。
            pipe.zadd(timeline, **post)                
            pipe.zremrangebyrank(                      
                timeline, 0, -HOME_TIMELINE_SIZE-1)    
        pipe.execute()

    conn = redis.Redis()
    if len(followers) >= POSTS_PER_PASS:
        execute_later(conn, 'default', 'syndicate_status',
            [uid, post, start, on_lists])

    elif not on_lists:
        execute_later(conn, 'default', 'syndicate_status',
            [uid, post, 0, True])
```

