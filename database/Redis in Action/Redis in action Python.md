##Redis in action Python

### Chapter 1

```shell
127.0.0.1:6379> set hello world
OK
127.0.0.1:6379> get hello
"world"
127.0.0.1:6379> del hello
(integer) 1
127.0.0.1:6379> rpush list-key item
(integer) 4
127.0.0.1:6379> rpush list-key item2
(integer) 5
127.0.0.1:6379> rpush list-key item
(integer) 6
127.0.0.1:6379> lrange list-key 0 -1
1) "item"
2) "item2"
3) "item"
4) "item"
5) "item2"
6) "item"
127.0.0.1:6379> lindex list-key 1
"item2"
127.0.0.1:6379> lpop list-key
"item"
127.0.0.1:6379> lrange list-key 0 -1
1) "item2"
2) "item"
3) "item"
4) "item2"
5) "item"
127.0.0.1:6379> sadd set-key item
(integer) 1
127.0.0.1:6379> sadd set-key item2
(integer) 0
127.0.0.1:6379> sadd set-key item3
(integer) 0
127.0.0.1:6379> sadd set-key item
(integer) 0
127.0.0.1:6379> smembers set-key
1) "item"
2) "item2"
3) "item3"
127.0.0.1:6379> sismember set-key item
(integer) 1
127.0.0.1:6379> srem set-key item2
(integer) 1
127.0.0.1:6379> hset hash-key sub-key1 value1
(integer) 0
127.0.0.1:6379> hset hash-key sub-key2 value2
(integer) 1
127.0.0.1:6379> hset hash-key sub-key1 value1
(integer) 0
127.0.0.1:6379> hgetall hash-key
1) "sub-key1"
2) "value1"
3) "sub-key2"
4) "value2"
127.0.0.1:6379> hdel hash-key sub-key2
(integer) 1
127.0.0.1:6379> hget hash-key sub-key1
"value1"
127.0.0.1:6379> zadd zset-key 728 mem1
(integer) 1
127.0.0.1:6379> zadd zset-key 982 mem0
(integer) 1
127.0.0.1:6379> zadd zset-key 982 mem0
(integer) 0
127.0.0.1:6379> zrange zset-key 0 -1 withscores
1) "mem1"
2) "728"
3) "mber2"
4) "928"
5) "mem0"
6) "982"
127.0.0.1:6379> zrangebyscore zset-key 0 800 withscores
1) "mem1"
2) "728"
127.0.0.1:6379> zrem zset-key mem1
(integer) 1
127.0.0.1:6379> zrem zset-key mem1
(integer) 0
127.0.0.1:6379> zrange zset-key 0 -1 withscores
1) "mber2"
2) "928"
3) "mem0"
4) "982"
127.0.0.1:6379> zscore zset-key mem0
"982"
```



```python
import redis
import time

conn = redis.Redis()
conn.flushall()

conn.hset("article:100408", "title", "Go to statement considered harmful")
conn.hset("article:100408", "link", "www.baidu.com")
conn.hset("article:100408", "poster", "user:83271")
conn.hset("article:100408", "time", "1574931480")
conn.hset("article:100408", "votes", "528")

conn.hset("article:100635", "title", "Go to statement considered harmful")
conn.hset("article:100635", "link", "www.baidu.com")
conn.hset("article:100635", "poster", "user:83271")
conn.hset("article:100635", "time", "1574931588")
conn.hset("article:100635", "votes", "528")

conn.hset("article:100716", "title", "Go to statement considered harmful")
conn.hset("article:100716", "link", "www.baidu.com")
conn.hset("article:100716", "poster", "user:83271")
conn.hset("article:100716", "time", "1574931680")
conn.hset("article:100716", "votes", "528")

conn.zadd("time:", {"article:100408": 1574931480})
conn.zadd("time:", {"article:100635": 1574931588})
conn.zadd("time:", {"article:100716": 1574931688})

conn.zadd("score:", {"article:100408": 1332065417.47})
conn.zadd("score:", {"article:100635": 1332075503.47})
conn.zadd("score:", {"article:100716": 1332082035.47})

conn.sadd("voted:100408", "user:234487")
conn.sadd("voted:100408", "user:234598")
conn.sadd("voted:100408", "user:236687")

ONE_WEEK_IN_SECONDS = 7 * 86400
VOTE_SCORE = 432


def article_vote(conn, user, article):

    # 计算文章的投票截止时间。
    cutoff = time.time() - ONE_WEEK_IN_SECONDS

    # 检查是否还可以对文章进行投票
    #（虽然使用散列也可以获取文章的发布时间，
    # 但有序集合返回的文章发布时间为浮点数，
    # 可以不进行转换直接使用）。
    if conn.zscore('time:', article) < cutoff:
        return

    # 从article:id标识符（identifier）里面取出文章的ID。
    article_id = article.partition(':')[-1]

    # 如果用户是第一次为这篇文章投票，那么增加这篇文章的投票数量和评分。
    if conn.sadd('voted:' + article_id, user):
        conn.zincrby('score:', article, VOTE_SCORE)
        conn.hincrby(article, 'votes', 1)


def post_article(conn, user, title, link):
    # 生成一个新的文章ID。
    article_id = str(conn.incr('article:'))

    voted = 'voted:' + article_id
    # 将发布文章的用户添加到文章的已投票用户名单里面，
    # 然后将这个名单的过期时间设置为一周（第3章将对过期时间作更详细的介绍）。
    conn.sadd(voted, user)
    conn.expire(voted, ONE_WEEK_IN_SECONDS)

    now = time.time()
    article = 'article:' + article_id
    # 将文章信息存储到一个散列里面。
    conn.hmset(article, {
        'title': title,
        'link': link,
        'poster': user,
        'time': now,
        'votes': 1,
    })

    # 将文章添加到根据发布时间排序的有序集合和根据评分排序的有序集合里面。
    conn.zadd('score:', article, now + VOTE_SCORE)
    conn.zadd('time:', article, now)

    return article_id


ARTICLES_PER_PAGE = 25


def get_articles(conn, page, order='score:'):
    # 设置获取文章的起始索引和结束索引。
    start = (page-1) * ARTICLES_PER_PAGE
    end = start + ARTICLES_PER_PAGE - 1

    # 获取多个文章ID。
    ids = conn.zrevrange(order, start, end)
    articles = []
    # 根据文章ID获取文章的详细信息。
    for id in ids:
        article_data = conn.hgetall(id)
        article_data['id'] = id
        articles.append(article_data)

    return articles


def add_remove_groups(conn, article_id, to_add=[], to_remove=[]):
    # 构建存储文章信息的键名。
    article = 'article:' + article_id
    for group in to_add:
        # 将文章添加到它所属的群组里面。
        conn.sadd('group:' + group, article)
    for group in to_remove:
        # 从群组里面移除文章。
        conn.srem('group:' + group, article)


def get_group_articles(conn, group, page, order='score:'):
    # 为每个群组的每种排列顺序都创建一个键。
    key = order + group
    # 检查是否有已缓存的排序结果，如果没有的话就现在进行排序。
    if not conn.exists(key):
        # 根据评分或者发布时间，对群组文章进行排序。
        conn.zinterstore(key,
            ['group:' + group, order],
            aggregate='max',
        )
        # 让Redis在60秒钟之后自动删除这个有序集合。
        conn.expire(key, 60)
    # 调用之前定义的get_articles()函数来进行分页并获取文章数据。
    return get_articles(conn, page, key)
```



### Chapter 2

```python
import redis
import time
import json
import threading
import time
import unittest
import uuid
import urllib.parse

class Inventory(object):
    def __init__(self, id):
        self.id = id

    @classmethod
    def get(cls, id):
        return Inventory(id)

    def to_dict(self):
        return {'id':self.id, 'data':'data to cache...', 'cached':time.time()}

def extract_item_id(request):
    parsed = urllib.parse.urlparse(request)
    query = urllib.parse.parse_qs(parsed.query)
    return (query.get('item') or [None])[0]

def is_dynamic(request):
    parsed = urllib.parse.urlparse(request)
    query = urllib.parse.parse_qs(parsed.query)
    return '_' in query

def hash_request(request):
    return str(hash(request))

def check_token(conn, token):
    return conn.hget('login:', token)   # 尝试获取并返回令牌对应的用户。


def update_token(conn, token, user, item=None): # 每秒可以记录20000件商品
    # 获取当前时间戳。
    timestamp = time.time()
    # 维持令牌与已登录用户之间的映射。
    conn.hset('login:', token, user)
    # 记录令牌最后一次出现的时间。
    conn.zadd('recent:', token, timestamp)
    if item:
        # 记录用户浏览过的商品。
        conn.zadd('viewed:' + token, item, timestamp)
        # 移除旧的记录，只保留用户最近浏览过的25个商品。
        conn.zremrangebyrank('viewed:' + token, 0, -26)

QUIT = False
LIMIT = 10000000

def clean_sessions(conn): # 网络每秒清理10000个 本地每秒清理60000个
    while not QUIT:
        # 找出目前已有令牌的数量。
        size = conn.zcard('recent:')
        # 令牌数量未超过限制，休眠并在之后重新检查。
        if size <= LIMIT:
            time.sleep(1)
            continue

        #  获取需要移除的令牌ID。
        end_index = min(size - LIMIT, 100)
        tokens = conn.zrange('recent:', 0, end_index-1)

        # 为那些将要被删除的令牌构建键名。
        session_keys = []
        for token in tokens:
            session_keys.append('viewed:' + token)

        # 移除最旧的那些令牌。
        conn.delete(*session_keys)
        conn.hdel('login:', *tokens)
        conn.zrem('recent:', *tokens)

def add_to_cart(conn, session, item, count):
    if count <= 0:
        # 从购物车里面移除指定的商品。
        conn.hrem('cart:' + session, item)
    else:
        # 将指定的商品添加到购物车。
        conn.hset('cart:' + session, item, count)


def clean_full_sessions(conn):
    while not QUIT:
        size = conn.zcard('recent:')
        if size <= LIMIT:
            time.sleep(1)
            continue

        end_index = min(size - LIMIT, 100)
        sessions = conn.zrange('recent:', 0, end_index-1)

        session_keys = []
        for sess in sessions:
            session_keys.append('viewed:' + sess)
            session_keys.append('cart:' + sess)   # 新增加的这行代码用于删除旧会话对应用户的购物车。

        conn.delete(*session_keys)
        conn.hdel('login:', *sessions)
        conn.zrem('recent:', *sessions)

def can_cache(conn, request):
    # 尝试从页面里面取出商品ID。
    item_id = extract_item_id(request)
    # 检查这个页面能否被缓存以及这个页面是否为商品页面。
    if not item_id or is_dynamic(request):
        return False
    # 取得商品的浏览次数排名。
    rank = conn.zrank('viewed:', item_id)
    # 根据商品的浏览次数排名来判断是否需要缓存这个页面。
    return rank is not None and rank < 10000

def cache_request(conn, request, callback):
    # 对于不能被缓存的请求，直接调用回调函数。
    if not can_cache(conn, request):
        return callback(request)

    # 将请求转换成一个简单的字符串键，方便之后进行查找。
    page_key = 'cache:' + hash_request(request)
    # 尝试查找被缓存的页面。
    content = conn.get(page_key)

    if not content:
        # 如果页面还没有被缓存，那么生成页面。
        content = callback(request)
        # 将新生成的页面放到缓存里面。
        conn.setex(page_key, content, 300)

    # 返回页面。
    return content

def schedule_row_cache(conn, row_id, delay):
    # 先设置数据行的延迟值。
    conn.zadd('delay:', row_id, delay)
    # 立即缓存数据行。
    conn.zadd('schedule:', row_id, time.time())

def cache_rows(conn):
    while not QUIT:
        # 尝试获取下一个需要被缓存的数据行以及该行的调度时间戳，
        # 命令会返回一个包含零个或一个元组（tuple）的列表。
        next = conn.zrange('schedule:', 0, 0, withscores=True)
        now = time.time()
        if not next or next[0][1] > now:
            # 暂时没有行需要被缓存，休眠50毫秒后重试。
            time.sleep(.05)
            continue

        row_id = next[0][0]
        # 获取下一次调度前的延迟时间。
        delay = conn.zscore('delay:', row_id)
        if delay <= 0:
            # 不必再缓存这个行，将它从缓存中移除。
            conn.zrem('delay:', row_id)
            conn.zrem('schedule:', row_id)
            conn.delete('inv:' + row_id)
            continue

        # 读取数据行。
        row = Inventory.get(row_id)
        # 更新调度时间并设置缓存值。
        conn.zadd('schedule:', row_id, now + delay)
        conn.set('inv:' + row_id, json.dumps(row.to_dict()))

def update_token(conn, token, user, item=None):
    timestamp = time.time()
    conn.hset('login:', token, user)
    conn.zadd('recent:', token, timestamp)
    if item:
        conn.zadd('viewed:' + token, item, timestamp)
        conn.zremrangebyrank('viewed:' + token, 0, -26)
        conn.zincrby('viewed:', item, -1)                   # 这行代码是新添加的。

def rescale_viewed(conn):
    while not QUIT:
        # 删除所有排名在20 000名之后的商品。
        conn.zremrangebyrank('viewed:', 20000, -1)
        # 将浏览次数降低为原来的一半
        conn.zinterstore('viewed:', {'viewed:': .5})
        # 5分钟之后再执行这个操作。
        time.sleep(300)


if __name__ == "__main__":
    conn = redis.Redis(host='217.0.0.1', port=6379, db=7)
    conn.flushall()
```



### Chapter 3

```shell
# 字符串
## 字符串可以存储 字符串、整数和浮点数
```



```python
import redis
import threading
import time

if __name__ == "__main__":
    conn = redis.Redis(host='127.0.0.1', port=6379, db=0)
    conn.flushall()

    ################ STRING ##################
    conn.get('string-key')
    conn.set('string-key', '13')
    conn.delete('string-key')
    conn.incr('string-key')                  # incr key-name
    conn.decr('string-key')                  # decr key-name
    conn.incrby('string-key', 15)            # incrby key-name amount
    conn.decrby('string-key', 5)             # decrby key-name amount
    conn.incrbyfloat('string-key', 12.5)     # incrbyfloat key-name amount

    conn.append('string-key', 'append')      # append key-name value
    conn.getrange('string-key', 0, 4)        # getrange key-name start end

    conn.setrange('string-key', 4, 'W')  # setrange key-name offset value
    conn.getbit('string-key', 6)              # getbit key-name offset
    conn.setbit('string-key', 4, 1)         # setbit key-name offset value
    conn.bitcount('string-key')               # bitcount key-name [start end]
    #conn.bitop('AND', 'new-string-key', ['key']) # bitop operation dest-key key-name [key-name]
    ################ STRING ##################

    ################ LIST ##################
    conn.rpush('list-key', 'last')             # rpush key-name value [value...]
    conn.lpush('list-key', 'first')            # lpush key-name value [value...]
    conn.rpush('list-key', 'new last')         #
    conn.lrange('list-key', 0, -1)             # lrange key-name start end

    conn.lpop('list-key')                      # lpop key-name
    conn.rpop('list-key')                      # rpop key-name
    conn.lrange('list-key', 0, -1)
    conn.rpush('list-key', 'a', 'b', 'c')
    conn.lrange('list-key', 0, -1)
    conn.ltrim('list-key', 2, -1)              # ltrim key-name start end
    conn.lrange('list-key', 0, -1)
    conn.lindex('list-key', 1)                 # lindex key-name offset

    conn.rpush('list', 'item')
    conn.rpush('list', 'item2')
    conn.rpush('list2', 'item3')

    conn.brpoplpush('list2', 'list', 1)
    conn.rpoplpush('list2', 'list')
    conn.blpop(['list', 'list2'], 1)
    conn.brpop(['list', 'list2'], 1)
    ################ LIST ##################

    ################ SET ##################
    conn.sadd('set-key', 'a', 'b', 'c')
    conn.srem('set-key', 'c', 'd')
    conn.scard('set-key')
    conn.smembers('set-key')
    conn.smove('set-key', 'set-key2', 'a')
    conn.spop('set-key')
    conn.srandmember('set-key', 4)
    conn.sismember('set-key', 'a')

    conn.sadd('skey1', 'a', 'b', 'c', 'd')
    conn.sadd('skey2', 'e', 'f', 'c', 'd')
    conn.sdiff('skey1', 'skey2')
    conn.sinter('skey1', 'skey2')
    conn.sunion('skey1', 'skey2')

    conn.sdiffstore('skey3', 'skey1', 'skey2')
    conn.sinterstore('skey4', 'skey1', 'skey2')
    conn.sunionstore('skey5', 'skey1', 'skey2')
    ################ SET ##################

    ################ HASH ##################
    conn.hmset('hash-key', {'k1':'v1', 'k2':'v2', 'k3':'v3'})
    conn.hmget('hash-key', ['k2', 'k3'])
    conn.hlen('hash-key')
    conn.hdel('hash-key', 'k1', 'k2')

    conn.hexists('hash-key', 'k1')
    conn.hkeys('hash-key')
    conn.hvals('hash-key')
    conn.hgetall('hash-key')
    conn.hincrby('hash-key', 'k2', 2)
    conn.hincrbyfloat('hash-key', 'k1', 10.0)
    ################ HASH ##################


    ################ ZSET ##################
    conn.zadd('zset-key', {'a': 3, 'b': 2, 'c': 1})
    conn.zcard('zset-key')
    conn.zincrby('zset-key', 2, 'a')
    conn.zscore('zset-key', 'b')
    conn.zrank('zset-key', 'c')
    conn.zcount('zset-key', 0, 3)
    conn.zrem('zset-key', 'b')
    conn.zrange('zset-key', 0, -1, withscores=True)

    conn.zadd('zset-key1', {'a': 3, 'b': 2, 'c': 1})
    conn.zadd('zset-key2', {'b': 4, 'c': 1, 'd': 0})
    conn.zinterstore('zset-key-3',['zset-key1', 'zset-key2'])
    conn.zrange('zset-key-3', 0, -1, withscores=True)
    conn.zunionstore('zset-key-u', ['zset-key1', 'zset-key2'], aggregate='min')
    conn.sadd('set-1', 'a', 'd')
    conn.zunionstore('zset-key-u2',  ['zset-key1', 'zset-key2', 'set-1'])
    conn.zunionstore('zset-key-u2', ['zset-key1', 'zset-key2'], aggregate='min')

    conn.zrevrank('zset-key1', 'a')
    conn.zrevrange('zset-key1', 1, 3, withscores=True)
    ################ ZSET ##################

    ################ 发布与订阅 ##################
    def publisher(n):
        time.sleep(1)
        for i in range(n):
            conn.publish('channel', i)
            time.sleep(1)


    def run_pubsub():
        threading.Thread(target=publisher, args=(3,)).start()
        pubsub = conn.pubsub()
        pubsub.subscribe(['channel'])
        count = 0
        for item in pubsub.listen():
            print(item)
            count += 1
            if count == 4:
                pubsub.unsubscribe()
            if count == 5:
                break
    run_pubsub()
    ################ 发布与订阅 ##################

    ################ OTHERS ##################
    conn.rpush('sort-key',23, 15, 110, 7)
    conn.sort('sort-key')

    conn.hset('d-7', 'field', 5)
    conn.hset('d-15', 'field', 1)
    conn.hset('d-23', 'field', 9)
    conn.hset('d-110', 'field', 3)

    conn.sort('sort-key', by='d-*->field')
    conn.sort('sort-key', by='d-*->field', get='d-*->field')


    def trans():
        pipeline = conn.pipeline()  # 创建一个事务型（transactional）流水线对象。
        pipeline.incr('trans:')  # 把针对‘trans:’计数器的自增操作放入队列。
        time.sleep(.1)  # 等待100毫秒。
        pipeline.incr('trans:', -1)  # 把针对‘trans:’计数器的自减操作放入队列。
        print(pipeline.execute()[0])  # 执行事务包含的命令并打印自增操作的执行结果。
    if 1:
        for i in range(3):  # 启动三个线程来执行被事务包裹的自增、休眠和自减三个操作。
            threading.Thread(target=trans).start()  #
        time.sleep(.5)  # 等待500毫秒，让操作有足够的时间完成。
    ################ OTHERS ##################

    ################ 过期时间 ##################
    conn.set('exp-key', 'value')
    conn.expire('exp-key')
    conn.set('exp-key2', 'value')
    conn.expire('exp-key2', 100)
    conn.ttl('exp-key2')

    conn.persist('exp-key2')
    conn.expireat('exp-key2', 123456789)
    conn.pttl('exp-key2')
    conn.pexpire('exp-key2', 3)
    conn.pexpireat('exp-key2', 123456789)
    ################ 过期时间 ##################

```



###Chapter 4

```python
import redis
import time
import uuid
'''
# <start id="persistence-options"/>
save 60 1000                        # 快照持久化选项。
stop-writes-on-bgsave-error no      #
rdbcompression yes                  #
dbfilename dump.rdb                 #

appendonly no                       # 只追加文件持久化选项。
appendfsync everysec                #
no-appendfsync-on-rewrite no        #
auto-aof-rewrite-percentage 100     #
auto-aof-rewrite-min-size 64mb      #

dir ./                              # 共享选项，这个选项决定了快照文件和只追加文件的保存位置。
'''

'''
redis-check-aof --fix
redis-check-dump dup.rdb
'''

'''
# 更换主服务器
# <start id="master-failover"/>
user@vpn-master ~:$ ssh root@machine-b.vpn                          # 通过VPN网络连接机器B。
Last login: Wed Mar 28 15:21:06 2012 from ...                       #
root@machine-b ~:$ redis-cli                                        # 启动命令行Redis客户端来执行几个简单的操作。
redis 127.0.0.1:6379> SAVE                                          # 执行SAVE命令，
OK                                                                  # 并在命令完成之后，
redis 127.0.0.1:6379> QUIT                                          # 使用QUIT命令退出客户端。
root@machine-b ~:$ scp \\                                           # 将快照文件发送至新的主服务器——机器C。
> /var/local/redis/dump.rdb machine-c.vpn:/var/local/redis/         #
dump.rdb                      100%   525MB  8.1MB/s   01:05         #
root@machine-b ~:$ ssh machine-c.vpn                                # 连接新的主服务器并启动Redis。
Last login: Tue Mar 27 12:42:31 2012 from ...                       #
root@machine-c ~:$ sudo /etc/init.d/redis-server start              #
Starting Redis server...                                            #
root@machine-c ~:$ exit
root@machine-b ~:$ redis-cli                                        # 告知机器B的Redis，让它将机器C用作新的主服务器。
redis 127.0.0.1:6379> SLAVEOF machine-c.vpn 6379                    #
OK                                                                  #
redis 127.0.0.1:6379> QUIT
root@machine-b ~:$ exit
user@vpn-master ~:$
# <end id="master-failover"/>
#A Connect to machine B on our vpn network
#B Start up the command line redis client to do a few simple operations
#C Start a SAVE, and when it is done, QUIT so that we can continue
#D Copy the snapshot over to the new master, machine C
#E Connect to the new master and start Redis
#F Tell machine B's Redis that it should use C as the new master
#END
'''
def wait_for_sync(mconn, sconn):
    identifier = str(uuid.uuid4())
    # 将令牌添加至主服务器。
    mconn.zadd('sync:wait', identifier, time.time())

    # 如果有必要的话，等待从服务器完成同步。
    while sconn.info()['master_link_status'] != 'up':
        time.sleep(.001)

    # 等待从服务器接收数据更新。
    while not sconn.zscore('sync:wait', identifier):
        time.sleep(.001)

    # 最多只等待一秒钟。
    deadline = time.time() + 1.01
    while time.time() < deadline:
        # 检查数据更新是否已经被同步到了磁盘。
        if sconn.info()['aof_pending_bio_fsync'] == 0:
            break
        time.sleep(.001)

    # 清理刚刚创建的新令牌以及之前可能留下的旧令牌。
    mconn.zrem('sync:wait', identifier)

def list_item(conn, itemid, sellerid, price):
    inventory = "inventory:%s"%sellerid
    item = "%s.%s"%(itemid, sellerid)
    end = time.time() + 5
    pipe = conn.pipeline()

    while time.time() < end:
        try:
            # 监视用户包裹发生的变化。
            pipe.watch(inventory)
            # 验证用户是否仍然持有指定的物品。
            if not pipe.sismember(inventory, itemid):
                # 如果指定的物品不在用户的包裹里面，
                # 那么停止对包裹键的监视并返回一个空值。
                pipe.unwatch()
                return None

            # 将指定的物品添加到物品买卖市场里面。
            pipe.multi()
            pipe.zadd("market:", item, price)
            pipe.srem(inventory, itemid)
            # 如果执行execute方法没有引发WatchError异常，
            # 那么说明事务执行成功，
            # 并且对包裹键的监视也已经结束。
            pipe.execute()
            return True
        # 用户的包裹已经发生了变化；重试。
        except redis.exceptions.WatchError:
            pass
    return False

def purchase_item(conn, buyerid, itemid, sellerid, lprice):
    buyer = "users:%s"%buyerid
    seller = "users:%s"%sellerid
    item = "%s.%s"%(itemid, sellerid)
    inventory = "inventory:%s"%buyerid
    end = time.time() + 10
    pipe = conn.pipeline()

    while time.time() < end:
        try:
            # 对物品买卖市场以及买家账号信息的变化进行监视。
            pipe.watch("market:", buyer)

            # 检查指定物品的价格是否出现了变化，
            # 以及买家是否有足够的钱来购买指定的物品。
            price = pipe.zscore("market:", item)
            funds = int(pipe.hget(buyer, "funds"))
            if price != lprice or price > funds:
                pipe.unwatch()
                return None

            # 将买家支付的货款转移给卖家，并将卖家出售的物品移交给买家。
            pipe.multi()
            pipe.hincrby(seller, "funds", int(price))
            pipe.hincrby(buyer, "funds", int(-price))
            pipe.sadd(inventory, itemid)
            pipe.zrem("market:", item)
            pipe.execute()
            return True
        # 如果买家的账号或者物品买卖市场出现了变化，那么进行重试。
        except redis.exceptions.WatchError:
            pass

    return False

def update_token(conn, token, user, item=None):
    # 获取时间戳。
    timestamp = time.time()
    # 创建令牌与已登录用户之间的映射。
    conn.hset('login:', token, user)
    # 记录令牌最后一次出现的时间。
    conn.zadd('recent:', token, timestamp)
    if item:
        # 把用户浏览过的商品记录起来。
        conn.zadd('viewed:' + token, item, timestamp)
        # 移除旧商品，只记录最新浏览的25件商品。
        conn.zremrangebyrank('viewed:' + token, 0, -26)
        # 更新给定商品的被浏览次数。
        conn.zincrby('viewed:', item, -1) 

def update_token_pipeline(conn, token, user, item=None):
    timestamp = time.time()
    # 设置流水线。
    pipe = conn.pipeline(False)                         #A
    pipe.hset('login:', token, user)
    pipe.zadd('recent:', token, timestamp)
    if item:
        pipe.zadd('viewed:' + token, item, timestamp)
        pipe.zremrangebyrank('viewed:' + token, 0, -26)
        pipe.zincrby('viewed:', item, -1)
    # 执行那些被流水线包裹的命令。
    pipe.execute()

def benchmark_update_token(conn, duration):
    # 测试会分别执行update_token()函数和update_token_pipeline()函数。
    for function in (update_token, update_token_pipeline):
        # 设置计数器以及测试结束的条件。
        count = 0                                               #B
        start = time.time()                                     #B
        end = start + duration                                  #B
        while time.time() < end:
            count += 1
            # 调用两个函数的其中一个。
            function(conn, 'token', 'user', 'item')             #C
        # 计算函数的执行时长。
        delta = time.time() - start                             #D
        # 打印测试结果。
        print(function.__name__, count, delta, count / delta)    #E


if __name__ == "__main__":
    print("main")
```



### Chapter 5

```python
import redis
import time
import uuid
import logging
import urllib.request
import csv
import json
import contextlib

# 设置一个字典，它可以帮助我们将大部分日志的安全级别转换成某种一致的东西。
SEVERITY = {
    logging.DEBUG: 'debug',
    logging.INFO: 'info',
    logging.WARNING: 'warning',
    logging.ERROR: 'error',
    logging.CRITICAL: 'critical',
}
SEVERITY.update((name, name) for name in SEVERITY.values())

def log_recent(conn, name, message, severity=logging.INFO, pipe=None):
    # 尝试将日志的级别转换成简单的字符串。
    severity = str(SEVERITY.get(severity, severity)).lower()
    # 创建负责存储消息的键。
    destination = 'recent:%s:%s'%(name, severity)
    # 将当前时间添加到消息里面，用于记录消息的发送时间。
    message = time.asctime() + ' ' + message
    # 使用流水线来将通信往返次数降低为一次。
    pipe = pipe or conn.pipeline()
    # 将消息添加到日志列表的最前面。
    pipe.lpush(destination, message)
    # 对日志列表进行修剪，让它只包含最新的100条消息。
    pipe.ltrim(destination, 0, 99)
    # 执行两个命令。
    pipe.execute()


def log_common(conn, name, message, severity=logging.INFO, timeout=5):
    # 设置日志的级别。
    severity = str(SEVERITY.get(severity, severity)).lower()
    # 负责存储最新日志的键。
    destination = 'common:%s:%s'%(name, severity)
    # 因为程序每小时需要轮换一次日志，所以它使用一个键来记录当前所处的小时数。
    start_key = destination + ':start'
    pipe = conn.pipeline()
    end = time.time() + timeout
    while time.time() < end:
        try:
            # 对记录当前小时数的键进行监视，确保轮换操作可以正确地执行。
            pipe.watch(start_key)
            # 取得当前时间。
            now = datetime.utcnow().timetuple()
            # 取得当前所处的小时数。
            hour_start = datetime(*now[:4]).isoformat()

            existing = pipe.get(start_key)
            # 创建一个事务。
            pipe.multi()
            # 如果目前的常见日志列表是上一个小时的……
            if existing and existing < hour_start:
                # ……那么将旧的常见日志信息进行归档。
                pipe.rename(destination, destination + ':last')
                pipe.rename(start_key, destination + ':pstart')
                # 更新当前所处的小时数。
                pipe.set(start_key, hour_start)

            # 对记录日志出现次数的计数器执行自增操作。
            pipe.zincrby(destination, message)
            # log_recent()函数负责记录日志并调用execute()函数。
            log_recent(pipe, name, message, severity, pipe)
            return
        except redis.exceptions.WatchError:
            # 如果程序因为其他客户端在执行归档操作而出现监视错误，那么重试。
            continue

# 以秒为单位的计数器精度，分别为1秒钟、5秒钟、1分钟、5分钟、1小时、5小时、1天——用户可以按需调整这些精度。
PRECISION = [1, 5, 60, 300, 3600, 18000, 86400]         #A

def update_counter(conn, name, count=1, now=None):
    # 通过取得当前时间来判断应该对哪个时间片执行自增操作。
    now = now or time.time()
    # 为了保证之后的清理工作可以正确地执行，这里需要创建一个事务型流水线。
    pipe = conn.pipeline()
    # 为我们记录的每种精度都创建一个计数器。
    for prec in PRECISION:
        # 取得当前时间片的开始时间。
        pnow = int(now / prec) * prec
        # 创建负责存储计数信息的散列。
        hash = '%s:%s'%(prec, name)
        # 将计数器的引用信息添加到有序集合里面，
        # 并将其分值设置为0，以便在之后执行清理操作。
        pipe.zadd('known:', hash, 0)
        # 对给定名字和精度的计数器进行更新。
        pipe.hincrby('count:' + hash, pnow, count)
    pipe.execute()
# <end id="update_counter"/>


# 代码清单 5-4
# <start id="get_counter"/>
def get_counter(conn, name, precision):
    # 取得存储着计数器数据的键的名字。
    hash = '%s:%s'%(precision, name)
    # 从Redis里面取出计数器数据。
    data = conn.hgetall('count:' + hash)
    # 将计数器数据转换成指定的格式。
    to_return = []
    for key, value in data.iteritems():
        to_return.append((int(key), int(value)))
    # 对数据进行排序，把旧的数据样本排在前面。
    to_return.sort()
    return to_return

def clean_counters(conn):
    pipe = conn.pipeline(True)
    # 为了平等地处理更新频率各不相同的多个计数器，程序需要记录清理操作执行的次数。
    passes = 0
    # 持续地对计数器进行清理，直到退出为止。
    while not QUIT:
        # 记录清理操作开始执行的时间，用于计算清理操作执行的时长。
        start = time.time()
        # 渐进地遍历所有已知的计数器。
        index = 0
        while index < conn.zcard('known:'):
            # 取得被检查计数器的数据。
            hash = conn.zrange('known:', index, index)
            index += 1
            if not hash:
                break
            hash = hash[0]
            # 取得计数器的精度。
            prec = int(hash.partition(':')[0])
            # 因为清理程序每60秒钟就会循环一次，
            # 所以这里需要根据计数器的更新频率来判断是否真的有必要对计数器进行清理。
            bprec = int(prec // 60) or 1
            # 如果这个计数器在这次循环里不需要进行清理，
            # 那么检查下一个计数器。
            # （举个例子，如果清理程序只循环了三次，而计数器的更新频率为每5分钟一次，
            # 那么程序暂时还不需要对这个计数器进行清理。）
            if passes % bprec:
                continue

            hkey = 'count:' + hash
            # 根据给定的精度以及需要保留的样本数量，
            # 计算出我们需要保留什么时间之前的样本。
            cutoff = time.time() - SAMPLE_COUNT * prec
            # 获取样本的开始时间，并将其从字符串转换为整数。
            samples = map(int, conn.hkeys(hkey))
            # 计算出需要移除的样本数量。
            samples.sort()
            remove = bisect.bisect_right(samples, cutoff)

            # 按需移除计数样本。
            if remove:
                conn.hdel(hkey, *samples[:remove])
                # 这个散列可能已经被清空。
                if remove == len(samples):
                    try:
                        # 在尝试修改计数器散列之前，对其进行监视。
                        pipe.watch(hkey)
                        # 验证计数器散列是否为空，如果是的话，
                        # 那么从记录已知计数器的有序集合里面移除它。
                        if not pipe.hlen(hkey):
                            pipe.multi()
                            pipe.zrem('known:', hash)
                            pipe.execute()
                            # 在删除了一个计数器的情况下，
                            # 下次循环可以使用与本次循环相同的索引。
                            index -= 1
                        else:
                            # 计数器散列并不为空，
                            # 继续让它留在记录已有计数器的有序集合里面。
                            pipe.unwatch()
                    # 有其他程序向这个计算器散列添加了新的数据，
                    # 它已经不再是空的了，继续让它留在记录已知计数器的有序集合里面。
                    except redis.exceptions.WatchError:
                        pass

        # 为了让清理操作的执行频率与计数器更新的频率保持一致，
        # 对记录循环次数的变量以及记录执行时长的变量进行更新。
        passes += 1
        duration = min(int(time.time() - start) + 1, 60)
        # 如果这次循环未耗尽60秒钟，那么在余下的时间内进行休眠；
        # 如果60秒钟已经耗尽，那么休眠一秒钟以便稍作休息。
        time.sleep(max(60 - duration, 1))


def update_stats(conn, context, type, value, timeout=5):
    # 设置用于存储统计数据的键。
    destination = 'stats:%s:%s'%(context, type)
    # 像common_log()函数一样，
    # 处理当前这一个小时的数据和上一个小时的数据。
    start_key = destination + ':start'
    pipe = conn.pipeline(True)
    end = time.time() + timeout
    while time.time() < end:
        try:
            pipe.watch(start_key)
            now = datetime.utcnow().timetuple()
            hour_start = datetime(*now[:4]).isoformat()

            existing = pipe.get(start_key)
            pipe.multi()
            if existing and existing < hour_start:
                pipe.rename(destination, destination + ':last')
                pipe.rename(start_key, destination + ':pstart')
                pipe.set(start_key, hour_start)

            tkey1 = str(uuid.uuid4())
            tkey2 = str(uuid.uuid4())
            # 将值添加到临时键里面。
            pipe.zadd(tkey1, 'min', value)
            pipe.zadd(tkey2, 'max', value)
            # 使用合适聚合函数MIN和MAX，
            # 对存储统计数据的键和两个临时键进行并集计算。
            pipe.zunionstore(destination,
                [destination, tkey1], aggregate='min')
            pipe.zunionstore(destination,
                [destination, tkey2], aggregate='max')

            # 删除临时键。
            pipe.delete(tkey1, tkey2)
            # 对有序集合中的样本数量、值的和、值的平方之和三个成员进行更新。
            pipe.zincrby(destination, 'count')
            pipe.zincrby(destination, 'sum', value)
            pipe.zincrby(destination, 'sumsq', value*value)

            # 返回基本的计数信息，以便函数调用者在有需要时做进一步的处理。
            return pipe.execute()[-3:]
        except redis.exceptions.WatchError:
            # 如果新的一个小时已经开始，并且旧的数据已经被归档，那么进行重试。
            continue

def get_stats(conn, context, type):
    # 程序将从这个键里面取出统计数据。
    key = 'stats:%s:%s'%(context, type)
    # 获取基本的统计数据，并将它们都放到一个字典里面。
    data = dict(conn.zrange(key, 0, -1, withscores=True))
    # 计算平均值。
    data['average'] = data['sum'] / data['count']
    # 计算标准差的第一个步骤。
    numerator = data['sumsq'] - data['sum'] ** 2 / data['count']
    # 完成标准差的计算工作。
    data['stddev'] = (numerator / (data['count'] - 1 or 1)) ** .5
    return data

# 将这个Python生成器用作上下文管理器。
@contextlib.contextmanager
def access_time(conn, context):
    # 记录代码块执行前的时间。
    start = time.time()
    # 运行被包裹的代码块。
    yield

    # 计算代码块的执行时长。
    delta = time.time() - start
    # 更新这一上下文的统计数据。
    stats = update_stats(conn, context, 'AccessTime', delta)
    # 计算页面的平均访问时长。
    average = stats[1] / stats[0]

    pipe = conn.pipeline(True)
    # 将页面的平均访问时长添加到记录最慢访问时间的有序集合里面。
    pipe.zadd('slowest:AccessTime', context, average)
    # AccessTime有序集合只会保留最慢的100条记录。
    pipe.zremrangebyrank('slowest:AccessTime', 0, -101)
    pipe.execute()


# 这个视图（view）接受一个Redis连接以及一个生成内容的回调函数为参数。
def process_view(conn, callback):
    # 计算并记录访问时长的上下文管理器就是这样包围代码块的。
    with access_time(conn, urllib.request.path):
        # 当上下文管理器中的yield语句被执行时，这个语句就会被执行。
        return callback()

def ip_to_score(ip_address):
    score = 0
    for v in ip_address.split('.'):
        score = score * 256 + int(v, 10)
    return score


# 这个函数在执行时需要给定GeoLiteCity-Blocks.csv文件所在的位置。
def import_ips_to_redis(conn, filename):
    csv_file = csv.reader(open(filename, 'rb'))
    for count, row in enumerate(csv_file):
        # 按需将IP地址转换为分值。
        start_ip = row[0] if row else ''
        if 'i' in start_ip.lower():
            continue
        if '.' in start_ip:
            start_ip = ip_to_score(start_ip)
        elif start_ip.isdigit():
            start_ip = int(start_ip, 10)
        else:
            # 略过文件的第一行以及格式不正确的条目。
            continue

        # 构建唯一城市ID。
        city_id = row[2] + '_' + str(count)
        # 将城市ID及其对应的IP地址分值添加到有序集合里面。
        conn.zadd('ip2cityid:', city_id, start_ip)


# 这个函数在执行时需要给定GeoLiteCity-Location.csv文件所在的位置。
def import_cities_to_redis(conn, filename):
    for row in csv.reader(open(filename, 'rb')):
        if len(row) < 4 or not row[0].isdigit():
            continue
        row = [i.decode('latin-1') for i in row]
        # 准备好需要添加到散列里面的信息。
        city_id = row[0]
        country = row[1]
        region = row[2]
        city = row[3]
        # 将城市信息添加到Redis里面。
        conn.hset('cityid2city:', city_id,
            json.dumps([city, region, country]))

def find_city_by_ip(conn, ip_address):
    # 将IP地址转换为分值以便执行ZREVRANGEBYSCORE命令。
    if isinstance(ip_address, str):                        #A
        ip_address = ip_to_score(ip_address)               #A

    # 查找唯一城市ID。
    city_id = conn.zrevrangebyscore(                       #B
        'ip2cityid:', ip_address, 0, start=0, num=1)       #B

    if not city_id:
        return None

    # 将唯一城市ID转换为普通城市ID。
    city_id = city_id[0].partition('_')[0]                 #C
    # 从散列里面取出城市信息。
    return json.loads(conn.hget('cityid2city:', city_id))  #D

LAST_CHECKED = None
IS_UNDER_MAINTENANCE = False

def is_under_maintenance(conn):
    # 将两个变量设置为全局变量以便在之后对它们进行写入。
    global LAST_CHECKED, IS_UNDER_MAINTENANCE   #A

    # 距离上次检查是否已经超过1秒钟？
    if LAST_CHECKED < time.time() - 1:          #B
        # 更新最后检查时间。
        LAST_CHECKED = time.time()              #C
        # 检查系统是否正在进行维护。
        IS_UNDER_MAINTENANCE = bool(            #D
            conn.get('is-under-maintenance'))   #D

    # 返回一个布尔值，用于表示系统是否正在进行维护。
    return IS_UNDER_MAINTENANCE


def set_config(conn, type, component, config):
    conn.set(
        'config:%s:%s'%(type, component),
        json.dumps(config))


CONFIGS = {}
CHECKED = {}

def get_config(conn, type, component, wait=1):
    key = 'config:%s:%s'%(type, component)

    # 检查是否需要对这个组件的配置信息进行更新。
    if CHECKED.get(key) < time.time() - wait:
        # 有需要对配置进行更新，记录最后一次检查这个连接的时间。
        CHECKED[key] = time.time()
        # 取得Redis存储的组件配置。
        config = json.loads(conn.get(key) or '{}')
        # 将潜在的Unicode关键字参数转换为字符串关键字参数。
        config = dict((str(k), config[k]) for k in config)
        # 取得组件正在使用的配置。
        old_config = CONFIGS.get(key)

        # 如果两个配置并不相同……
        if config != old_config:
            # ……那么对组件的配置进行更新。
            CONFIGS[key] = config

    return CONFIGS.get(key)

REDIS_CONNECTIONS = {}

# 将应用组件的名字传递给装饰器。
def redis_connection(component, wait=1):                        #A
    # 因为函数每次被调用都需要获取这个配置键，所以我们干脆把它缓存起来。
    key = 'config:redis:' + component                           #B
    # 包装器接受一个函数作为参数，并使用另一个函数来包裹这个函数。
    def wrapper(function):                                      #C
        # 将被包裹函数里的一些有用的元数据复制到配置处理器。
        @functools.wraps(function)                              #D
        # 创建负责管理连接信息的函数。
        def call(*args, **kwargs):                              #E
            # 如果有旧配置存在，那么获取它。
            old_config = CONFIGS.get(key, object())             #F
            # 如果有新配置存在，那么获取它。
            _config = get_config(                               #G
                config_connection, 'redis', component, wait)    #G

            config = {}
            # 对配置进行处理并将其用于创建Redis连接。
            for k, v in _config.iteritems():                    #L
                config[k.encode('utf-8')] = v                   #L

            # 如果新旧配置并不相同，那么创建新的连接。
            if config != old_config:                            #H
                REDIS_CONNECTIONS[key] = redis.Redis(**config)  #H

            # 将Redis连接以及其他匹配的参数传递给被包裹函数，然后调用函数并返回执行结果。
            return function(                                    #I
                REDIS_CONNECTIONS.get(key), *args, **kwargs)    #I
        # 返回被包裹的函数。
        return call                                             #J
    # 返回用于包裹Redis函数的包装器。
    return wrapper

'''
# <start id="recent_log_decorator"/>
@redis_connection('logs')                   # redis_connection()装饰器非常容易使用。
def log_recent(conn, app, message):         # 这个函数的定义和之前展示的一样，没有发生任何变化。
    'the old log_recent() code'

log_recent('main', 'User 235 logged in')    # 我们再也不必在调用log_recent()函数时手动地向它传递日志服务器的连接了。
# <end id="recent_log_decorator"/>
'''

if __name__ == "__main__":
    print("main")
```



### Chapter 6

```python
import redis
import time
import uuid
import logging
import urllib.request
import csv
import json
import contextlib
import bisect
import math

def add_update_contact(conn, user, contact):
    ac_list = 'recent:' + user
    # 准备执行原子操作。
    pipeline = conn.pipeline(True)
    # 如果联系人已经存在，那么移除他。
    pipeline.lrem(ac_list, contact)
    # 将联系人推入到列表的最前端。
    pipeline.lpush(ac_list, contact)
    # 只保留列表里面的前100个联系人。
    pipeline.ltrim(ac_list, 0, 99)
    # 实际地执行以上操作。
    pipeline.execute()


def remove_contact(conn, user, contact):
    conn.lrem('recent:' + user, contact)


def fetch_autocomplete_list(conn, user, prefix):
    # 获取自动补完列表。
    candidates = conn.lrange('recent:' + user, 0, -1)
    matches = []
    # 检查每个候选联系人。
    for candidate in candidates:
        if candidate.lower().startswith(prefix):
            # 发现一个匹配的联系人。
            matches.append(candidate)
    # 返回所有匹配的联系人。
    return matches


valid_characters = '`abcdefghijklmnopqrstuvwxyz{'

def find_prefix_range(prefix):
    # 在字符列表中查找前缀字符所处的位置。
    posn = bisect.bisect_left(valid_characters, prefix[-1:])
    # 找到前驱字符。
    suffix = valid_characters[(posn or 1) - 1]
    # 返回范围。
    return prefix[:-1] + suffix + '{', prefix + '{'

def autocomplete_on_prefix(conn, guild, prefix):
    # 根据给定的前缀计算出查找范围的起点和终点。
    start, end = find_prefix_range(prefix)
    identifier = str(uuid.uuid4())
    start += identifier
    end += identifier
    zset_name = 'members:' + guild

    # 将范围的起始元素和结束元素添加到有序集合里面。
    conn.zadd(zset_name, start, 0, end, 0)
    pipeline = conn.pipeline(True)
    while 1:
        try:
            pipeline.watch(zset_name)
            # 找到两个被插入元素在有序集合中的排名。
            sindex = pipeline.zrank(zset_name, start)
            eindex = pipeline.zrank(zset_name, end)
            erange = min(sindex + 9, eindex - 2)
            pipeline.multi()
            # 获取范围内的值，然后删除之前插入的起始元素和结束元素。
            pipeline.zrem(zset_name, start, end)
            pipeline.zrange(zset_name, sindex, erange)
            items = pipeline.execute()[-1]
            break
        # 如果自动补完有序集合已经被其他客户端修改过了，那么进行重试。
        except redis.exceptions.WatchError:
            continue

    # 如果有其他自动补完操作正在执行，
    # 那么从获取到的元素里面移除起始元素和终结元素。
    return [item for item in items if '{' not in item]

def join_guild(conn, guild, user):
    conn.zadd('members:' + guild, user, 0)

def leave_guild(conn, guild, user):
    conn.zrem('members:' + guild, user)

'''
def list_item(conn, itemid, sellerid, price):
    #...
    # 监视卖家包裹发生的变动。
    pipe = conn.pipeline(True)
    pipe.watch("inv")
    # 确保被出售的物品仍然存在于卖家的包裹里面。
    if not pipe.sismember("inv", itemid):
        pipe.unwatch()
        return None

    # 将物品添加到市场里面。
    pipe.multi()
    pipe.zadd("market:", itemid, price)
    pipe.srem("inv", itemid)
    pipe.execute()
    return True

def purchase_item(conn, buyerid, itemid, sellerid, lprice):
    #...
    # 监视市场以及买家个人信息发生的变化。
    pipe.watch("market:", buyer)

    # 检查物品是否已经售出、物品的价格是否已经发生了变化，
    # 以及买家是否有足够的金钱来购买这件物品。
    price = pipe.zscore("market:", item)
    funds = int(pipe.hget(buyer, 'funds'))
    if price != lprice or price > funds:
        pipe.unwatch()
        return None

    # 将买家支付的货款转移给卖家，并将被卖出的物品转移给买家。
    pipe.multi()
    pipe.hincrby(seller, 'funds', int(price))
    pipe.hincrby(buyerid, 'funds', int(-price))
    pipe.sadd(inventory, itemid)
    pipe.zrem("market:", item)
    pipe.execute()
    return True
'''

# Redis 锁
def acquire_lock(conn, lockname, acquire_timeout=10):
    # 128位随机标识符。
    identifier = str(uuid.uuid4())

    end = time.time() + acquire_timeout
    while time.time() < end:
        # 尝试取得锁。
        if conn.setnx('lock:' + lockname, identifier):
            return identifier

        time.sleep(.001)

    return False

def release_lock(conn, lockname, identifier):
    pipe = conn.pipeline(True)
    lockname = 'lock:' + lockname

    while True:
        try:
            # 检查并确认进程还持有着锁。
            pipe.watch(lockname)
            if pipe.get(lockname) == identifier:
                # 释放锁。
                pipe.multi()
                pipe.delete(lockname)
                pipe.execute()
                return True

            pipe.unwatch()
            break

        # 有其他客户端修改了锁；重试。
        except redis.exceptions.WatchError:
            pass

    # 进程已经失去了锁。
    return False

def purchase_item_with_lock(conn, buyerid, itemid, sellerid):
    buyer = "users:%s" % buyerid
    seller = "users:%s" % sellerid
    item = "%s.%s" % (itemid, sellerid)
    inventory = "inventory:%s" % buyerid

    # 尝试获取锁。
    locked = acquire_lock(conn, 'market:')
    if not locked:
        return False

    pipe = conn.pipeline(True)
    try:
        # 检查物品是否已经售出，以及买家是否有足够的金钱来购买物品。
        pipe.zscore("market:", item)
        pipe.hget(buyer, 'funds')
        price, funds = pipe.execute()
        if price is None or price > funds:
            return None

        # 将买家支付的货款转移给卖家，并将售出的物品转移给买家。
        pipe.hincrby(seller, 'funds', int(price))
        pipe.hincrby(buyer, 'funds', int(-price))
        pipe.sadd(inventory, itemid)
        pipe.zrem("market:", item)
        pipe.execute()
        return True
    finally:
        # 释放锁。
        release_lock(conn, 'market:', locked)

#带有超时的锁
def acquire_lock_with_timeout(
    conn, lockname, acquire_timeout=10, lock_timeout=10):
    # 128位随机标识符。
    identifier = str(uuid.uuid4())
    lockname = 'lock:' + lockname
    # 确保传给EXPIRE的都是整数。
    lock_timeout = int(math.ceil(lock_timeout))

    end = time.time() + acquire_timeout
    while time.time() < end:
        # 获取锁并设置过期时间。
        if conn.setnx(lockname, identifier):
            conn.expire(lockname, lock_timeout)
            return identifier
        # 检查过期时间，并在有需要时对其进行更新。
        elif not conn.ttl(lockname):
            conn.expire(lockname, lock_timeout)

        time.sleep(.001)

    return False

def acquire_semaphore(conn, semname, limit, timeout=10):
    # 128位随机标识符。
    identifier = str(uuid.uuid4())
    now = time.time()

    pipeline = conn.pipeline(True)
    # 清理过期的信号量持有者。
    pipeline.zremrangebyscore(semname, '-inf', now - timeout)
    # 尝试获取信号量。
    pipeline.zadd(semname, identifier, now)
    # 检查是否成功取得了信号量。
    pipeline.zrank(semname, identifier)
    if pipeline.execute()[-1] < limit:
        return identifier

    # 获取信号量失败，删除之前添加的标识符。
    conn.zrem(semname, identifier)
    return None
# <end id="_1314_14473_8986"/>


# 代码清单 6-13
# <start id="_1314_14473_8990"/>
def release_semaphore(conn, semname, identifier):
    # 如果信号量已经被正确地释放，那么返回True；
    # 返回False则表示该信号量已经因为过期而被删除了。
    return conn.zrem(semname, identifier)

def acquire_fair_semaphore(conn, semname, limit, timeout=10):
    # 128位随机标识符。
    identifier = str(uuid.uuid4())
    czset = semname + ':owner'
    ctr = semname + ':counter'

    now = time.time()
    pipeline = conn.pipeline(True)
    # 删除超时的信号量。
    pipeline.zremrangebyscore(semname, '-inf', now - timeout)
    pipeline.zinterstore(czset, {czset: 1, semname: 0})

    # 对计数器执行自增操作，并获取操作执行之后的值。
    pipeline.incr(ctr)
    counter = pipeline.execute()[-1]

    # 尝试获取信号量。
    pipeline.zadd(semname, identifier, now)
    pipeline.zadd(czset, identifier, counter)

    # 通过检查排名来判断客户端是否取得了信号量。
    pipeline.zrank(czset, identifier)
    if pipeline.execute()[-1] < limit:
        # 客户端成功取得了信号量。
        return identifier

    # 客户端未能取得信号量，清理无用数据。
    pipeline.zrem(semname, identifier)
    pipeline.zrem(czset, identifier)
    pipeline.execute()
    return None

def release_fair_semaphore(conn, semname, identifier):
    pipeline = conn.pipeline(True)
    pipeline.zrem(semname, identifier)
    pipeline.zrem(semname + ':owner', identifier)
    # 返回True表示信号量已被正确地释放，
    # 返回False则表示想要释放的信号量已经因为超时而被删除了。
    return pipeline.execute()[0]

def refresh_fair_semaphore(conn, semname, identifier):
    # 更新客户端持有的信号量。
    if conn.zadd(semname, identifier, time.time()):
        # 告知调用者，客户端已经失去了信号量。
        release_fair_semaphore(conn, semname, identifier)
        return False
    # 客户端仍然持有信号量。
    return True

def acquire_semaphore_with_lock(conn, semname, limit, timeout=10):
    identifier = acquire_lock(conn, semname, acquire_timeout=.01)
    if identifier:
        try:
            return acquire_fair_semaphore(conn, semname, limit, timeout)
        finally:
            release_lock(conn, semname, identifier)


def send_sold_email_via_queue(conn, seller, item, price, buyer):
    # 准备好待发送邮件。
    data = {
        'seller_id': seller,
        'item_id': item,
        'price': price,
        'buyer_id': buyer,
        'time': time.time()
    }
    # 将待发送邮件推入到队列里面。
    conn.rpush('queue:email', json.dumps(data))
# <end id="_1314_14473_9056"/>


# 代码清单 6-19
# <start id="_1314_14473_9060"/>
def process_sold_email_queue(conn):
    while not QUIT:
        # 尝试获取一封待发送邮件。
        packed = conn.blpop(['queue:email'], 30)
        # 队列里面暂时还没有待发送邮件，重试。
        if not packed:
            continue

        # 从JSON对象中解码出邮件信息。
        to_send = json.loads(packed[1])
        try:
            # 使用预先编写好的邮件发送函数来发送邮件。
            fetch_data_and_send_sold_email(to_send)
        except EmailSendError as err:
            log_error("Failed to send sold email", err, to_send)
        else:
            log_success("Sent sold email", to_send)

def worker_watch_queue(conn, queue, callbacks):
    while not QUIT:
        # 尝试从队列里面取出一项待执行任务。
        packed = conn.blpop([queue], 30)
        # 队列为空，没有任务需要执行；重试。
        if not packed:
            continue

        # 解码任务信息。
        name, args = json.loads(packed[1])
        # 没有找到任务指定的回调函数，用日志记录错误并重试。
        if name not in callbacks:
            log_error("Unknown callback %s"%name)
            continue
        # 执行任务。
        callbacks[name](*args)

def worker_watch_queues(conn, queues, callbacks):   # 实现优先级特性要修改的第一行代码。
    while not QUIT:
        packed = conn.blpop(queues, 30)             # 实现优先级特性要修改的第二行代码。
        if not packed:
            continue

        name, args = json.loads(packed[1])
        if name not in callbacks:
            log_error("Unknown callback %s"%name)
            continue
        callbacks[name](*args)

def execute_later(conn, queue, name, args, delay=0):
    # 创建唯一标识符。
    identifier = str(uuid.uuid4())
    # 准备好需要入队的任务。
    item = json.dumps([identifier, queue, name, args])
    if delay > 0:
        # 延迟执行这个任务。
        conn.zadd('delayed:', item, time.time() + delay)
    else:
        # 立即执行这个任务。
        conn.rpush('queue:' + queue, item)
    # 返回标识符。
    return identifier

def poll_queue(conn):
    while not QUIT:
        # 获取队列中的第一个任务。
        item = conn.zrange('delayed:', 0, 0, withscores=True)
        # 队列没有包含任何任务，或者任务的执行时间未到。
        if not item or item[0][1] > time.time():
            time.sleep(.01)
            continue

        # 解码要被执行的任务，弄清楚它应该被推入到哪个任务队列里面。
        item = item[0][0]
        identifier, queue, function, args = json.loads(item)

        # 为了对任务进行移动，尝试获取锁。
        locked = acquire_lock(conn, identifier)
        # 获取锁失败，跳过后续步骤并重试。
        if not locked:
            continue

        # 将任务推入到适当的任务队列里面。
        if conn.zrem('delayed:', item):
            conn.rpush('queue:' + queue, item)

        # 释放锁。
        release_lock(conn, identifier, locked)


def create_chat(conn, sender, recipients, message, chat_id=None):
    # 获得新的群组ID。
    chat_id = chat_id or str(conn.incr('ids:chat:'))

    # 创建一个由用户和分值组成的字典，字典里面的信息将被添加到有序集合里面。
    recipients.append(sender)
    recipientsd = dict((r, 0) for r in recipients)

    pipeline = conn.pipeline(True)
    # 将所有参与群聊的用户添加到有序集合里面。
    pipeline.zadd('chat:' + chat_id, **recipientsd)
    # 初始化已读有序集合。
    for rec in recipients:
        pipeline.zadd('seen:' + rec, chat_id, 0)
    pipeline.execute()

    # 发送消息。
    return send_message(conn, chat_id, sender, message)


def send_message(conn, chat_id, sender, message):
    identifier = acquire_lock(conn, 'chat:' + chat_id)
    if not identifier:
        raise Exception("Couldn't get the lock")
    try:
        # 筹备待发送的消息。
        mid = conn.incr('ids:' + chat_id)
        ts = time.time()
        packed = json.dumps({
            'id': mid,
            'ts': ts,
            'sender': sender,
            'message': message,
        })

        # 将消息发送至群组。
        conn.zadd('msgs:' + chat_id, packed, mid)
    finally:
        release_lock(conn, 'chat:' + chat_id, identifier)
    return chat_id
# <end id="_1314_14473_9127"/>


# 代码清单 6-26
# <start id="_1314_14473_9132"/>
def fetch_pending_messages(conn, recipient):
    # 获取最后接收到的消息的ID。
    seen = conn.zrange('seen:' + recipient, 0, -1, withscores=True)

    pipeline = conn.pipeline(True)

    # 获取所有未读消息。
    for chat_id, seen_id in seen:
        pipeline.zrangebyscore(
            'msgs:' + chat_id, seen_id+1, 'inf')
    # 这些数据将被返回给函数调用者。
    chat_info = zip(seen, pipeline.execute())

    for i, ((chat_id, seen_id), messages) in enumerate(chat_info):
        if not messages:
            continue
        messages[:] = map(json.loads, messages)
        # 使用最新收到的消息来更新群组有序集合。
        seen_id = messages[-1]['id']
        conn.zadd('chat:' + chat_id, recipient, seen_id)

        # 找出那些所有人都已经阅读过的消息。
        min_id = conn.zrange(
            'chat:' + chat_id, 0, 0, withscores=True)

        # 更新已读消息有序集合。
        pipeline.zadd('seen:' + recipient, chat_id, seen_id)
        if min_id:
            # 清除那些已经被所有人阅读过的消息。
            pipeline.zremrangebyscore(
                'msgs:' + chat_id, 0, min_id[0][1])
        chat_info[i] = (chat_id, messages)
    pipeline.execute()

    return chat_info

def join_chat(conn, chat_id, user):
    # 取得最新群组消息的ID。
    message_id = int(conn.get('ids:' + chat_id))

    pipeline = conn.pipeline(True)
    # 将用户添加到群组成员列表里面。
    pipeline.zadd('chat:' + chat_id, user, message_id)
    # 将群组添加到用户的已读列表里面。
    pipeline.zadd('seen:' + user, chat_id, message_id)
    pipeline.execute()


def leave_chat(conn, chat_id, user):
    pipeline = conn.pipeline(True)
    # 从群组里面移除给定的用户。
    pipeline.zrem('chat:' + chat_id, user)
    pipeline.zrem('seen:' + user, chat_id)
    # 查看群组剩余成员的数量。
    pipeline.zcard('chat:' + chat_id)

    if not pipeline.execute()[-1]:
        # 删除群组。
        pipeline.delete('msgs:' + chat_id)
        pipeline.delete('ids:' + chat_id)
        pipeline.execute()
    else:
        # 查找那些已经被所有成员阅读过的消息。
        oldest = conn.zrange(
            'chat:' + chat_id, 0, 0, withscores=True)
        # 删除那些已经被所有成员阅读过的消息。
        conn.zremrangebyscore('msgs:' + chat_id, 0, oldest[0][1])

if __name__ == "__main__":
    print("main")
```



### Chapter 7

```python
import redis
import time
import uuid
import logging
import urllib.request
import csv
import json
import contextlib
import bisect
import math
import re

AVERAGE_PER_1K = {}

# 预先定义好从网上获取的停止词。
STOP_WORDS = set('''able about across after all almost also am among
an and any are as at be because been but by can cannot could dear did
do does either else ever every for from get got had has have he her
hers him his how however if in into is it its just least let like
likely may me might most must my neither no nor not of off often on
only or other our own rather said say says she should since so some
than that the their them then there these they this tis to too twas us
wants was we were what when where which while who whom why will with
would yet you your'''.split())

#  根据定义提取单词的正则表达式。
WORDS_RE = re.compile("[a-z']{2,}")

def tokenize(content):
    # 将文章包含的单词储存到 Python 集合里面。
    words = set()
    # 遍历文章包含的所有单词。
    for match in WORDS_RE.finditer(content.lower()):
        # 剔除所有位于单词前面或后面的单引号。
        word = match.group().strip("'")
        # 保留那些至少有两个字符长的单词。
        if len(word) >= 2:
            words.add(word)
    # 返回一个集合，集合里面包含了所有被保留并且不是停止词的单词。
    return words - STOP_WORDS

def index_document(conn, docid, content):
    # 对内容进行标记化处理，并取得处理产生的单词。
    words = tokenize(content)

    pipeline = conn.pipeline(True)
    # 将文章添加到正确的反向索引集合里面。
    for word in words:
        pipeline.sadd('idx:' + word, docid)
    # 计算一下，程序为这篇文章添加了多少个独一无二并且不是停止词的单词。
    return len(pipeline.execute())


def _set_common(conn, method, names, ttl=30, execute=True):
    # 创建一个新的临时标识符。
    id = str(uuid.uuid4())
    # 设置事务流水线，确保每个调用都能获得一致的执行结果。
    pipeline = conn.pipeline(True) if execute else conn
    # 给每个单词加上 'idx:' 前缀。
    names = ['idx:' + name for name in names]
    # 为将要执行的集合操作设置相应的参数。
    getattr(pipeline, method)('idx:' + id, *names)
    # 吩咐 Redis 在将来自动删除这个集合。
    pipeline.expire('idx:' + id, ttl)
    if execute:
        # 实际地执行操作。
        pipeline.execute()
    # 将结果集合的 ID 返回给调用者，以便做进一步的处理。
    return id

# 执行交集计算的辅助函数。
def intersect(conn, items, ttl=30, _execute=True):
    return _set_common(conn, 'sinterstore', items, ttl, _execute)

# 执行并集计算的辅助函数。
def union(conn, items, ttl=30, _execute=True):
    return _set_common(conn, 'sunionstore', items, ttl, _execute)

# 执行差集计算的辅助函数。
def difference(conn, items, ttl=30, _execute=True):
    return _set_common(conn, 'sdiffstore', items, ttl, _execute)

# 查找需要的单词、不需要的单词以及同义词的正则表达式。
QUERY_RE = re.compile("[+-]?[a-z']{2,}")

def parse(query):
    # 这个集合将用于储存不需要的单词。
    unwanted = set()
    # 这个列表将用于储存需要执行交集计算的单词。
    all = []
    # 这个集合将用于储存目前已发现的同义词。
    current = set()
    # 遍历搜索查询语句中的所有单词。
    for match in QUERY_RE.finditer(query.lower()):
        # 检查单词是否带有 + 号前缀或 - 号前缀。
        word = match.group()
        prefix = word[:1]
        if prefix in '+-':
            word = word[1:]
        else:
            prefix = None

        # 剔除所有位于单词前面或者后面的单引号，并略过所有停止词。
        word = word.strip("'")
        if len(word) < 2 or word in STOP_WORDS:
            continue

        # 如果这是一个不需要的单词，
        # 那么将它添加到储存不需要单词的集合里面。
        if prefix == '-':
            unwanted.add(word)
            continue

        # 如果在同义词集合非空的情况下，
        # 遇到了一个不带 + 号前缀的单词，
        # 那么创建一个新的同义词集合。
        if current and not prefix:
            all.append(list(current))
            current = set()
        current.add(word)

    # 将正在处理的单词添加到同义词集合里面。
    if current:
        all.append(list(current))

    # 把所有剩余的单词都放到最后的交集计算里面进行处理。
    return all, list(unwanted)

def parse_and_search(conn, query, ttl=30):
    # 对查询语句进行分析。
    all, unwanted = parse(query)
    # 如果查询语句只包含停止词，那么这次搜索没有任何结果。
    if not all:
        return None

    to_intersect = []
    # 遍历各个同义词列表。
    for syn in all:
        # 如果同义词列表包含的单词不止一个，那么执行并集计算。
        if len(syn) > 1:
            to_intersect.append(union(conn, syn, ttl=ttl))
        # 如果同义词列表只包含一个单词，那么直接使用这个单词。
        else:
            to_intersect.append(syn[0])

    # 如果单词（或者并集计算的结果）有不止一个，那么执行交集计算。
    if len(to_intersect) > 1:
        intersect_result = intersect(conn, to_intersect, ttl=ttl)
    # 如果单词（或者并集计算的结果）只有一个，那么将它用作交集计算的结果。
    else:
        intersect_result = to_intersect[0]

    # 如果用户给定了不需要的单词，
    # 那么从交集计算结果里面移除包含这些单词的文章，然后返回搜索结果。
    if unwanted:
        unwanted.insert(0, intersect_result)
        return difference(conn, unwanted, ttl=ttl)

    # 如果用户没有给定不需要的单词，那么直接返回交集计算的结果作为搜索的结果。
    return intersect_result
# 用户可以通过可选的参数来传入已有的搜索结果、指定搜索结果的排序方式，并对结果进行分页。
def search_and_sort(conn, query, id=None, ttl=300, sort="-updated",
                    start=0, num=20):
    # 决定基于文章的哪个属性进行排序，以及是进行升序排序还是降序排序。
    desc = sort.startswith('-')
    sort = sort.lstrip('-')
    by = "kb:doc:*->" + sort
    # 告知 Redis ，排序是以数值方式进行还是字母方式进行。
    alpha = sort not in ('updated', 'id', 'created')

    # 如果用户给定了已有的搜索结果，
    # 并且这个结果仍然存在的话，
    # 那么延长它的生存时间。
    if id and not conn.expire(id, ttl):
        id = None

    # 如果用户没有给定已有的搜索结果，
    # 或者给定的搜索结果已经过期，
    # 那么执行一次新的搜索操作。
    if not id:
        id = parse_and_search(conn, query, ttl=ttl)

    pipeline = conn.pipeline(True)
    # 获取结果集合的元素数量。
    pipeline.scard('idx:' + id)
    # 根据指定属性对结果进行排序，并且只获取用户指定的那一部分结果。
    pipeline.sort('idx:' + id, by=by, alpha=alpha,
        desc=desc, start=start, num=num)
    results = pipeline.execute()

    # 返回搜索结果包含的元素数量、搜索结果本身以及搜索结果的 ID ，
    # 其中搜索结果的 ID 可以用于在之后再次获取本次搜索的结果。
    return results[0], results[1], id

# 和之前一样，函数接受一个已有搜索结果的 ID 作为可选参数，
# 以便在结果仍然可用的情况下，对其进行分页。
def search_and_zsort(conn, query, id=None, ttl=300, update=1, vote=0,
                    start=0, num=20, desc=True):

    # 尝试更新已有搜索结果的生存时间。
    if id and not conn.expire(id, ttl):
        id = None

    # 如果传入的结果已经过期，
    # 或者这是函数第一次进行搜索，
    # 那么执行标准的集合搜索操作。
    if not id:
        id = parse_and_search(conn, query, ttl=ttl)

        scored_search = {
            # 函数在计算并集的时候也会用到传入的 ID 键，
            # 但这个键不会被用作排序权重（weight）。
            id: 0,
            # 对文章评分进行调整以平衡更新时间和投票数量。
            # 根据待排序数据的需要，投票数量可以被调整为 1 、10 、100 ，甚至更高。
            'sort:update': update,
            'sort:votes': vote
        }
        # 使用代码清单 7-7 定义的辅助函数执行交集计算。
        id = zintersect(conn, scored_search, ttl)

    pipeline = conn.pipeline(True)
    # 获取结果有序集合的大小。
    pipeline.zcard('idx:' + id)
    #  从搜索结果里面取出一页（page）。
    if desc:
        pipeline.zrevrange('idx:' + id, start, start + num - 1)
    else:
        pipeline.zrange('idx:' + id, start, start + num - 1)
    results = pipeline.execute()

    # 返回搜索结果，以及分页用的 ID 值。
    return results[0], results[1], id


# 代码清单 7-7
def _zset_common(conn, method, scores, ttl=30, **kw):
    # 创建一个新的临时标识符。
    id = str(uuid.uuid4())
    # 调用者可以通过传递参数来决定是否使用事务流水线。
    execute = kw.pop('_execute', True)
    # 设置事务流水线，保证每个单独的调用都有一致的结果。
    pipeline = conn.pipeline(True) if execute else conn
    # 为输入的键添加 ‘idx:’ 前缀。
    for key in scores.keys():
        scores['idx:' + key] = scores.pop(key)
    # 为将要被执行的操作设置好相应的参数。
    getattr(pipeline, method)('idx:' + id, scores, **kw)
    # 为计算结果有序集合设置过期时间。
    pipeline.expire('idx:' + id, ttl)
    # 除非调用者明确指示要延迟执行操作，否则实际地执行计算操作。
    if execute:
        pipeline.execute()
    # 将计算结果的 ID 返回给调用者，以便做进一步的处理。
    return id

# 对有序集合执行交集计算的辅助函数。
def zintersect(conn, items, ttl=30, **kw):
    return _zset_common(conn, 'zinterstore', dict(items), ttl, **kw)

# 对有序集合执行并集计算的辅助函数。
def zunion(conn, items, ttl=30, **kw):
    return _zset_common(conn, 'zunionstore', dict(items), ttl, **kw)

def string_to_score(string, ignore_case=False):
    # 用户可以通过参数来决定是否以大小写无关的方式建立前缀索引。
    if ignore_case:
        string = string.lower()

    # 将字符串的前 6 个字符转换为相应的数字值，
    # 比如把空字符转换为 0 、制表符（tab）转换为 9 、大写 A 转换为 65 ，
    # 诸如此类。
    pieces = map(ord, string[:6])
    # 为长度不足 6 个字符的字符串添加占位符，以此来表示这是一个短字符。
    while len(pieces) < 6:
        pieces.append(-1)

    score = 0
    # 对字符串进行转换得出的每个值都会被计算到分值里面，
    # 并且程序处理空字符的方式和处理占位符的方式并不相同。
    for piece in pieces:
        score = score * 257 + piece + 1

    # 通过多使用一个二进制位，
    # 程序可以表明字符串是否正好为 6 个字符长，
    # 这样它就可以正确地区分出 “robber” 和 “robbers” ，
    # 尽管这对于区分 “robbers” 和 “robbery” 并无帮助。
    return score * 2 + (len(string) > 6)

def to_char_map(set):
    out = {}
    for pos, val in enumerate(sorted(set)):
        out[val] = pos-1
    return out

LOWER = to_char_map(set([-1]) | set(range(ord('a'), ord('z')+1)))
ALPHA = to_char_map(set(LOWER) | set(range(ord('A'), ord('Z')+1)))
LOWER_NUMERIC = to_char_map(set(LOWER) | set(range(ord('0'), ord('9')+1)))
ALPHA_NUMERIC = to_char_map(set(LOWER_NUMERIC) | set(ALPHA))

def string_to_score_generic(string, mapping):
    length = int(52 / math.log(len(mapping), 2))    #A

    pieces = map(ord, string[:length])              #B
    while len(pieces) < length:                     #C
        pieces.append(-1)                           #C

    score = 0
    for piece in pieces:                            #D
        value = mapping[piece]                      #D
        score = score * len(mapping) + value + 1    #D

    return score * 2 + (len(string) > length)       #E



# <start id="zadd-string"/>
def zadd_string(conn, name, *args, **kwargs):
    pieces = list(args)                         # 为了进行之后的修改，
    for piece in kwargs.iteritems():            # 对传入的不同类型的参数进行合并（combine）
        pieces.extend(piece)                    #

    for i, v in enumerate(pieces):
        if i & 1:                               # 将字符串格式的分值转换为整数分值
            pieces[i] = string_to_score(v)      #

    return conn.zadd(name, *pieces)             # 调用已有的 ZADD 方法

def cpc_to_ecpm(views, clicks, cpc):
    return 1000. * cpc * clicks / views

def cpa_to_ecpm(views, actions, cpa):
    # 因为点击通过率是由点击次数除以展示次数计算出的，
    # 而动作的执行概率则是由动作执行次数除以点击次数计算出的，
    # 所以这两个概率相乘的结果等于动作执行次数除以展示次数。
    return 1000. * cpa * actions / views

TO_ECPM = {
    'cpc': cpc_to_ecpm,
    'cpa': cpa_to_ecpm,
    'cpm': lambda *args:args[-1],
}

def index_ad(conn, id, locations, content, type, value):
    # 设置流水线，使得程序可以在一次通信往返里面完成整个索引操作。
    pipeline = conn.pipeline(True)

    for location in locations:
        # 为了进行定向操作，把广告 ID 添加到所有相关的位置集合里面。
        pipeline.sadd('idx:req:'+location, id)

    words = tokenize(content)
    # 对广告包含的单词进行索引。
    for word in tokenize(content):
        pipeline.zadd('idx:' + word, id, 0)

    # 为了评估新广告的效果，
    # 程序会使用字典来储存广告每千次展示的平均点击次数或平均动作执行次数。
    rvalue = TO_ECPM[type](
        1000, AVERAGE_PER_1K.get(type, 1), value)
    # 记录这个广告的类型。
    pipeline.hset('type:', id, type)
    # 将广告的 eCPM 添加到一个记录了所有广告的 eCPM 的有序集合里面。
    pipeline.zadd('idx:ad:value:', id, rvalue)
    # 将广告的基本价格（base value）添加到一个记录了所有广告的基本价格的有序集合里面。
    pipeline.zadd('ad:base_value:', id, value)
    # 把能够对广告进行定向的单词全部记录起来。
    pipeline.sadd('terms:' + id, *list(words))
    pipeline.execute()

def target_ads(conn, locations, content):
    pipeline = conn.pipeline(True)
    # 根据用户传入的位置定向参数，找到所有位于该位置的广告，以及这些广告的 eCPM 。
    matched_ads, base_ecpm = match_location(pipeline, locations)
    # 基于匹配的内容计算附加值。
    words, targeted_ads = finish_scoring(
        pipeline, matched_ads, base_ecpm, content)

    # 获取一个 ID ，它可以用于汇报并记录这个被定向的广告。
    pipeline.incr('ads:served:')
    # 找到 eCPM 最高的广告，并获取这个广告的 ID 。
    pipeline.zrevrange('idx:' + targeted_ads, 0, 0)
    target_id, targeted_ad = pipeline.execute()[-2:]

    # 如果没有任何广告与目标位置相匹配，那么返回空值。
    if not targeted_ad:
        return None, None

    ad_id = targeted_ad[0]
    # 记录一系列定向操作的执行结果，作为学习用户行为的其中一个步骤。
    record_targeting_result(conn, target_id, ad_id, words)

    # 向调用者返回记录本次定向操作相关信息的 ID ，以及被选中的广告的 ID 。
    return target_id, ad_id

def match_location(pipe, locations):
    # 根据给定的位置，找出所有需要执行并集操作的集合键。
    required = ['req:' + loc for loc in locations]
    # 找出与指定地区相匹配的广告，并将它们储存到集合里面。
    matched_ads = union(pipe, required, ttl=300, _execute=False)
    # 找到储存着所有被匹配广告的集合，
    # 以及储存着所有被匹配广告的基本 eCPM 的有序集合，
    # 然后返回它们的 ID 。
    return matched_ads, zintersect(pipe,
        {matched_ads: 0, 'ad:value:': 1}, _execute=False)
# <end id="location_target"/>

def record_click(conn, target_id, ad_id, action=False):
    pipeline = conn.pipeline(True)
    click_key = 'clicks:%s'%ad_id

    match_key = 'terms:matched:%s'%target_id

    type = conn.hget('type:', ad_id)
    # 如果这是一个按动作计费的广告，
    # 并且被匹配的单词仍然存在，
    # 那么刷新这些单词的过期时间。
    if type == 'cpa':
        pipeline.expire(match_key, 900)
        if action:
            # 记录动作信息，而不是点击信息。
            click_key = 'actions:%s' % ad_id

    if action and type == 'cpa':
        # 根据广告的类型，维持一个全局的点击/动作计数器。
        pipeline.incr('type:%s:actions:' % type)
    else:
        pipeline.incr('type:%s:clicks:' % type)

    # 为广告以及所有被定向至该广告的单词记录下本次点击（或动作）。
    matched = list(conn.smembers(match_key))
    matched.append('')
    for word in matched:
        pipeline.zincrby(click_key, word)
    pipeline.execute()

    # 对广告中出现的所有单词的 eCPM 进行更新。
    update_cpms(conn, ad_id)

def finish_scoring(pipe, matched, base, content):
    bonus_ecpm = {}
    # 对内容进行标记化处理，以便与广告进行匹配。
    words = tokenize(content)
    for word in words:
        # 找出那些既位于定向位置之内，又拥有页面内容其中一个单词的广告。
        word_bonus = zintersect(
            pipe, {matched: 0, word: 1}, _execute=False)
        bonus_ecpm[word_bonus] = 1

    if bonus_ecpm:
        # 计算每个广告的最小 eCPM 附加值和最大 eCPM 附加值。
        minimum = zunion(
            pipe, bonus_ecpm, aggregate='MIN', _execute=False)
        maximum = zunion(
            pipe, bonus_ecpm, aggregate='MAX', _execute=False)

        # 将广告的基本价格、最小 eCPM 附加值的一半以及最大 eCPM 附加值的一半这三者相加起来。
        return words, zunion(
            pipe, {base:1, minimum:.5, maximum:.5}, _execute=False)
    # 如果页面内容中没有出现任何可匹配的单词，那么返回广告的基本 eCPM 。
    return words, base


def record_targeting_result(conn, target_id, ad_id, words):
    pipeline = conn.pipeline(True)

    # 找出内容与广告之间相匹配的那些单词。
    terms = conn.smembers('terms:' + ad_id)
    matched = list(words & terms)
    if matched:
        matched_key = 'terms:matched:%s' % target_id
        # 如果有相匹配的单词出现，那么把它们记录起来，并设置 15 分钟的生存时间。
        pipeline.sadd(matched_key, *matched)
        pipeline.expire(matched_key, 900)

    # 为每种类型的广告分别记录它们的展示次数。
    type = conn.hget('type:', ad_id)
    pipeline.incr('type:%s:views:' % type)
    # 对广告以及广告包含的单词的展示信息进行记录。
    for word in matched:
        pipeline.zincrby('views:%s' % ad_id, word)
    pipeline.zincrby('views:%s' % ad_id, '')

    # 广告每展示 100 次，就更新一次它的 eCPM 。
    if not pipeline.execute()[-1] % 100:
        update_cpms(conn, ad_id)

def update_cpms(conn, ad_id):
    pipeline = conn.pipeline(True)
    # 获取广告的类型和价格，以及广告包含的所有单词。
    pipeline.hget('type:', ad_id)
    pipeline.zscore('ad:base_value:', ad_id)
    pipeline.smembers('terms:' + ad_id)
    type, base_value, words = pipeline.execute()

    # 判断广告的 eCPM 应该基于点击次数进行计算还是基于动作执行次数进行计算。
    which = 'clicks'
    if type == 'cpa':
        which = 'actions'

    # 根据广告的类型，
    # 获取这类广告的展示次数和点击次数（或者动作执行次数）。
    pipeline.get('type:%s:views:' % type)
    pipeline.get('type:%s:%s' % (type, which))
    type_views, type_clicks = pipeline.execute()
    # 将广告的点击率或动作执行率重新写入到全局字典里面。
    AVERAGE_PER_1K[type] = (
        1000. * int(type_clicks or '1') / int(type_views or '1'))

    # 如果正在处理的是一个 CPM 广告，
    # 那么它的 eCPM 已经更新完毕，
    # 无需再做其他处理。
    if type == 'cpm':
        return

    view_key = 'views:%s' % ad_id
    click_key = '%s:%s' % (which, ad_id)

    to_ecpm = TO_ECPM[type]

    # 获取广告的展示次数，以及广告的点击次数（或者动作执行次数）。
    pipeline.zscore(view_key, '')
    pipeline.zscore(click_key, '')
    ad_views, ad_clicks = pipeline.execute()
    # 如果广告还没有被点击过，那么使用已有的 eCPM 。
    if (ad_clicks or 0) < 1:
        ad_ecpm = conn.zscore('idx:ad:value:', ad_id)
    else:
        # 计算广告的 eCPM 并更新它的价格。
        ad_ecpm = to_ecpm(ad_views or 1, ad_clicks or 0, base_value)
        pipeline.zadd('idx:ad:value:', ad_id, ad_ecpm)

    for word in words:
        # 获取单词的展示次数和点击次数（或者动作执行次数）。
        pipeline.zscore(view_key, word)
        pipeline.zscore(click_key, word)
        views, clicks = pipeline.execute()[-2:]

        # 如果广告还未被点击过，那么不对 eCPM 进行更新。
        if (clicks or 0) < 1:
            continue

        # 计算单词的 eCPM 。
        word_ecpm = to_ecpm(views or 1, clicks or 0, base_value)
        # 计算单词的附加值。
        bonus = word_ecpm - ad_ecpm
        # 将单词的附加值重新写入到为广告包含的每个单词分别记录附加值的有序集合里面。
        pipeline.zadd('idx:' + word, ad_id, bonus)
    pipeline.execute()

def add_job(conn, job_id, required_skills):
    # 把职位所需的技能全部添加到职位对应的集合里面。
    conn.sadd('job:' + job_id, *required_skills)

def is_qualified(conn, job_id, candidate_skills):
    temp = str(uuid.uuid4())
    pipeline = conn.pipeline(True)
    # 把求职者拥有的技能全部添加到一个临时集合里面，并设置过期时间。
    pipeline.sadd(temp, *candidate_skills)
    pipeline.expire(temp, 5)
    # 找出职位所需技能当中，求职者不具备的那些技能，并将它们记录到结果集合里面。
    pipeline.sdiff('job:' + job_id, temp)
    # 如果求职者具备职位所需的全部技能，那么返回 True 。
    return not pipeline.execute()[-1]

def index_job(conn, job_id, skills):
    pipeline = conn.pipeline(True)
    for skill in skills:
        # 将职位 ID 添加到相应的技能集合里面。
        pipeline.sadd('idx:skill:' + skill, job_id)
    # 将职位所需技能的数量添加到记录了所有职位所需技能数量的有序集合里面。
    pipeline.zadd('idx:jobs:req', job_id, len(set(skills)))
    pipeline.execute()

def find_jobs(conn, candidate_skills):
    # 设置好用于计算职位得分的字典。
    skills = {}
    for skill in set(candidate_skills):
        skills['skill:' + skill] = 1

    # 计算求职者对于每个职位的得分。
    job_scores = zunion(conn, skills)
    # 计算出求职者能够胜任以及不能够胜任的职位。
    final_result = zintersect(
        conn, {job_scores:-1, 'jobs:req':1})

    # 返回求职者能够胜任的那些职位。
    return conn.zrangebyscore('idx:' + final_result, 0, 0)

SKILL_LEVEL_LIMIT = 2

def index_job_levels(conn, job_id, skill_levels):
    total_skills = len(set(skill for skill, level in skill_levels))
    pipeline = conn.pipeline(True)
    for skill, level in skill_levels:
        level = min(level, SKILL_LEVEL_LIMIT)
        for wlevel in range(level, SKILL_LEVEL_LIMIT+1):
            pipeline.sadd('idx:skill:%s:%s'%(skill,wlevel), job_id)
    pipeline.zadd('idx:jobs:req', job_id, total_skills)
    pipeline.execute()

def search_job_levels(conn, skill_levels):
    skills = {}
    for skill, level in skill_levels:
        level = min(level, SKILL_LEVEL_LIMIT)
        for wlevel in range(level, SKILL_LEVEL_LIMIT+1):
            skills['skill:%s:%s'%(skill,wlevel)] = 1

    job_scores = zunion(conn, skills)
    final_result = zintersect(conn, {job_scores:-1, 'jobs:req':1})

    return conn.zrangebyscore('idx:' + final_result, 0, 0)


def index_job_years(conn, job_id, skill_years):
    total_skills = len(set(skill for skill, level in skill_years))
    pipeline = conn.pipeline(True)
    for skill, years in skill_years:
        pipeline.zadd(
            'idx:skill:%s:years'%skill, job_id, max(years, 0))
    pipeline.sadd('idx:jobs:all', job_id)
    pipeline.zadd('idx:jobs:req', job_id, total_skills)


def search_job_years(conn, skill_years):
    skill_years = dict(skill_years)
    pipeline = conn.pipeline(True)

    union = []
    for skill, years in skill_years.iteritems():
        sub_result = zintersect(pipeline,
            {'jobs:all':-years, 'skill:%s:years'%skill:1}, _execute=False)
        pipeline.zremrangebyscore('idx:' + sub_result, '(0', 'inf')
        union.append(
            zintersect(pipeline, {'jobs:all':1, sub_result:0}), _execute=False)

    job_scores = zunion(pipeline, dict((key, 1) for key in union), _execute=False)
    final_result = zintersect(pipeline, {job_scores:-1, 'jobs:req':1}, _execute=False)

    pipeline.zrange('idx:' + final_result, 0, 0)
    return pipeline.execute()[-1]

if __name__ == "__main__":
    print("main")
```



### Chapter 8

```python
import redis
import time
import uuid
import logging
import urllib.request
import csv
import json
import contextlib
import bisect
import math
import re
import threading
import functools

def acquire_lock_with_timeout(
    conn, lockname, acquire_timeout=10, lock_timeout=10):
    identifier = str(uuid.uuid4())                      #A
    lockname = 'lock:' + lockname
    lock_timeout = int(math.ceil(lock_timeout))         #D

    end = time.time() + acquire_timeout
    while time.time() < end:
        if conn.setnx(lockname, identifier):            #B
            conn.expire(lockname, lock_timeout)         #B
            return identifier
        elif not conn.ttl(lockname):                    #C
            conn.expire(lockname, lock_timeout)         #C

        time.sleep(.001)

    return False

def release_lock(conn, lockname, identifier):
    pipe = conn.pipeline(True)
    lockname = 'lock:' + lockname

    while True:
        try:
            pipe.watch(lockname)                  #A
            if pipe.get(lockname) == identifier:  #A
                pipe.multi()                      #B
                pipe.delete(lockname)             #B
                pipe.execute()                    #B
                return True                       #B

            pipe.unwatch()
            break

        except redis.exceptions.WatchError:       #C
            pass                                  #C

    return False                                  #D

CONFIGS = {}
CHECKED = {}

def get_config(conn, type, component, wait=1):
    key = 'config:%s:%s'%(type, component)

    if CHECKED.get(key) < time.time() - wait:           #A
        CHECKED[key] = time.time()                      #B
        config = json.loads(conn.get(key) or '{}')      #C
        old_config = CONFIGS.get(key)                   #D

        if config != old_config:                        #E
            CONFIGS[key] = config                       #F

    return CONFIGS.get(key)

REDIS_CONNECTIONS = {}

def redis_connection(component, wait=1):                        #A
    key = 'config:redis:' + component                           #B
    def wrapper(function):                                      #C
        @functools.wraps(function)                              #D
        def call(*args, **kwargs):                              #E
            old_config = CONFIGS.get(key, object())             #F
            _config = get_config(                               #G
                config_connection, 'redis', component, wait)    #G

            config = {}
            for k, v in _config.iteritems():                    #L
                config[k.encode('utf-8')] = v                   #L

            if config != old_config:                            #H
                REDIS_CONNECTIONS[key] = redis.Redis(**config)  #H

            return function(                                    #I
                REDIS_CONNECTIONS.get(key), *args, **kwargs)    #I
        return call                                             #J
    return wrapper                                              #K

def execute_later(conn, queue, name, args):
    # this is just for testing purposes
    assert conn is args[0]
    t = threading.Thread(target=globals()[name], args=tuple(args))
    t.setDaemon(1)
    t.start()

def create_user(conn, login, name):
    llogin = login.lower()
    # 使用第 6 章定义的加锁函数尝试对小写的用户名进行加锁。
    lock = acquire_lock_with_timeout(conn, 'user:' + llogin, 1)
    # 如果加锁不成功，那么说明给定的用户名已经被其他用户占用了。
    if not lock:
        return None

    # 程序使用了一个散列来储存小写的用户名以及用户 ID 之间的映射，
    # 如果给定的用户名已经被映射到了某个用户 ID ，
    # 那么程序就不会再将这个用户名分配给其他人。
    if conn.hget('users:', llogin):
        release_lock(conn, 'user:' + llogin, lock)
        return None

    # 每个用户都有一个独一无二的 ID ，
    # 这个 ID 是通过对计数器执行自增操作产生的。
    id = conn.incr('user:id:')
    pipeline = conn.pipeline(True)
    # 在散列里面将小写的用户名映射至用户 ID 。
    pipeline.hset('users:', llogin, id)
    # 将用户信息添加到用户对应的散列里面。
    pipeline.hmset('user:%s'%id, {
        'login': login,
        'id': id,
        'name': name,
        'followers': 0,
        'following': 0,
        'posts': 0,
        'signup': time.time(),
    })
    pipeline.execute()
    # 释放之前对用户名加的锁。
    release_lock(conn, 'user:' + llogin, lock)
    # 返回用户 ID 。
    return id

def create_status(conn, uid, message, **data):
    pipeline = conn.pipeline(True)
    # 根据用户 ID 获取用户的用户名。
    pipeline.hget('user:%s'%uid, 'login')
    # 为这条状态消息创建一个新的 ID 。
    pipeline.incr('status:id:')
    login, id = pipeline.execute()

    # 在发布状态消息之前，先检查用户的账号是否存在。
    if not login:
        return None

    # 准备并设置状态消息的各项信息。
    data.update({
        'message': message,
        'posted': time.time(),
        'id': id,
        'uid': uid,
        'login': login,
    })
    pipeline.hmset('status:%s'%id, data)
    # 更新用户的已发送状态消息数量。
    pipeline.hincrby('user:%s'%uid, 'posts')
    pipeline.execute()
    # 返回新创建的状态消息的 ID 。
    return id

# 函数接受三个可选参数，
# 它们分别用于指定函数要获取哪条时间线、要获取多少页时间线、以及每页要有多少条状态消息。
def get_status_messages(conn, uid, timeline='home:', page=1, count=30):
    # 获取时间线上面最新的状态消息的 ID 。
    statuses = conn.zrevrange(
        '%s%s'%(timeline, uid), (page-1)*count, page*count-1)

    pipeline = conn.pipeline(True)
    # 获取状态消息本身。
    for id in statuses:
        pipeline.hgetall('status:%s'%id)

    # 使用过滤器移除那些已经被删除了的状态消息。
    return filter(None, pipeline.execute())

HOME_TIMELINE_SIZE = 1000
def follow_user(conn, uid, other_uid):
    # 把正在关注有序集合以及关注者有序集合的键名缓存起来。
    fkey1 = 'following:%s'%uid
    fkey2 = 'followers:%s'%other_uid

    # 如果 uid 指定的用户已经关注了 other_uid 指定的用户，那么函数直接返回。
    if conn.zscore(fkey1, other_uid):
        return None

    now = time.time()

    pipeline = conn.pipeline(True)
    # 将两个用户的 ID 分别添加到相应的正在关注有序集合以及关注者有序集合里面。
    pipeline.zadd(fkey1, other_uid, now)
    pipeline.zadd(fkey2, uid, now)
    # 从被关注用户的个人时间线里面获取 HOME_TIMELINE_SIZE 条最新的状态消息。
    pipeline.zrevrange('profile:%s'%other_uid,
        0, HOME_TIMELINE_SIZE-1, withscores=True)
    following, followers, status_and_score = pipeline.execute()[-3:]

    # 修改两个用户的散列，更新他们各自的正在关注数量以及关注者数量。
    pipeline.hincrby('user:%s'%uid, 'following', int(following))
    pipeline.hincrby('user:%s'%other_uid, 'followers', int(followers))
    if status_and_score:
        # 对执行关注操作的用户的定制时间线进行更新，并保留时间线上面的最新 1000 条状态消息。
        pipeline.zadd('home:%s'%uid, **dict(status_and_score))
    pipeline.zremrangebyrank('home:%s'%uid, 0, -HOME_TIMELINE_SIZE-1)

    pipeline.execute()
    # 返回 True 表示关注操作已经成功执行。
    return True

def unfollow_user(conn, uid, other_uid):
    # 把正在关注有序集合以及关注者有序集合的键名缓存起来。
    fkey1 = 'following:%s'%uid
    fkey2 = 'followers:%s'%other_uid

    # 如果 uid 指定的用户并未关注 other_uid 指定的用户，那么函数直接返回。
    if not conn.zscore(fkey1, other_uid):
        return None

    pipeline = conn.pipeline(True)
    # 从正在关注有序集合以及关注者有序集合里面移除双方的用户 ID 。
    pipeline.zrem(fkey1, other_uid)
    pipeline.zrem(fkey2, uid)
    # 获取被取消关注的用户最近发布的 HOME_TIMELINE_SIZE 条状态消息。
    pipeline.zrevrange('profile:%s'%other_uid,
        0, HOME_TIMELINE_SIZE-1)
    following, followers, statuses = pipeline.execute()[-3:]

    # 对用户信息散列里面的正在关注数量以及关注者数量进行更新。
    pipeline.hincrby('user:%s'%uid, 'following', int(following))
    pipeline.hincrby('user:%s'%other_uid, 'followers', int(followers))
    if statuses:
        # 对执行取消关注操作的用户的定制时间线进行更新，
        # 移除被取消关注的用户发布的所有状态消息。
        pipeline.zrem('home:%s'%uid, *statuses)

    pipeline.execute()
    # 返回 True 表示取消关注操作执行成功。
    return True

REFILL_USERS_STEP = 50
def refill_timeline(conn, incoming, timeline, start=0):
    if not start and conn.zcard(timeline) >= 750:               # 如果时间线已经被填满了 3/4 或以上
        return                                                  # 那么不对它进行重新填充

    users = conn.zrangebyscore(incoming, start, 'inf',          # 获取一组用户，这些用户发布的消息将被用于填充时间线
        start=0, num=REFILL_USERS_STEP, withscores=True)        #

    pipeline = conn.pipeline(False)
    for uid, start in users:
        pipeline.zrevrange('profile:%s'%uid,                    # 从正在关注的人哪里获取最新的状态消息
            0, HOME_TIMELINE_SIZE-1, withscores=True)           #

    messages = []
    for results in pipeline.execute():
        messages.extend(results)                            # 将取得的所有状态消息放到一起

    messages.sort(key=lambda x:-x[1])                       # 根据发布时间对取得的所有状态消息进行排序，
    del messages[HOME_TIMELINE_SIZE:]                       # 并保留其中最新的 100 条状态消息

    pipeline = conn.pipeline(True)
    if messages:
        pipeline.zadd(timeline, **dict(messages))           # 将挑选出的状态消息添加到用户的主页时间线上面
    pipeline.zremrangebyrank(                               # 对时间线进行修剪，只保留最新的 100 条状态消息
        timeline, 0, -HOME_TIMELINE_SIZE-1)                 #
    pipeline.execute()

    if len(users) >= REFILL_USERS_STEP:
        execute_later(conn, 'default', 'refill_timeline',       # 如果还要其他用户的时间线需要进行重新填充，
            [conn, incoming, timeline, start])                  # 那么继续执行这个动作
# <end id="exercise-refilling-timelines"/>

def clean_timelines(conn, uid, status_id, start=0, on_lists=False):
    key = 'followers:%s'%uid            # 根据操作的处理进度，
    base = 'home:%s'                    # 选择对主页时间线还是对用户时间线进行操作
    if on_lists:                        #
        key = 'list:out:%s'%uid         #
        base = 'list:statuses:%s'       #
    followers = conn.zrangebyscore(key, start, 'inf',   # 从上次更新时的最后一个用户或者列表作为起点，
        start=0, num=POSTS_PER_PASS, withscores=True)   # 获取下一组用户或者列表（数量为 1000 个）

    pipeline = conn.pipeline(False)
    for follower, start in followers:                    # 从所有已获取的关注者的主页时间线上面，
        pipeline.zrem(base%follower, status_id)          # 移除指定的状态消息
    pipeline.execute()

    if len(followers) >= POSTS_PER_PASS:                    # 如果本次更新已经处理了至少 1000 个关注者，
        execute_later(conn, 'default', 'clean_timelines' ,  # 那么将后续的工作留到下次再执行
            [conn, uid, status_id, start, on_lists])        #

    elif not on_lists:
        execute_later(conn, 'default', 'clean_timelines',   # 如果针对列表的操作并未完成，那么对列表进行操作
            [conn, uid, status_id, 0, True])                # 如果操作只是对主页时间线执行的话，那么程序无需执行这一步

def follow_user_list(conn, uid, other_uid, list_id):
    fkey1 = 'list:in:%s'%list_id            # 把相关的键名缓存起来
    fkey2 = 'list:out:%s'%other_uid         #
    timeline = 'list:statuses:%s'%list_id   #

    if conn.zscore(fkey1, other_uid):   # 如果 other_uid 已经包含在列表里面，
        return None                     # 那么直接返回

    now = time.time()

    pipeline = conn.pipeline(True)
    pipeline.zadd(fkey1, other_uid, now)        # 将各个用户ID添加到相应的有序集合里面
    pipeline.zadd(fkey2, list_id, now)          #
    pipeline.zcard(fkey1)                       # 获取有序集合的大小
    pipeline.zrevrange('profile:%s'%other_uid,      # 从用户的个人时间线里面获取最新的状态消息
        0, HOME_TIMELINE_SIZE-1, withscores=True)   #
    following, status_and_score = pipeline.execute()[-2:]

    pipeline.hset('list:%s'%list_id, 'following', following)    # 对存储列表信息的散列进行更新，将列表的新大小记录到散列里面
    pipeline.zadd(timeline, **dict(status_and_score))           # 对列表的状态消息进行更新
    pipeline.zremrangebyrank(timeline, 0, -HOME_TIMELINE_SIZE-1)#

    pipeline.execute()
    return True                         # 返回 True 值，表示用户已经被添加到列表里面

def unfollow_user_list(conn, uid, other_uid, list_id):
    fkey1 = 'list:in:%s'%list_id            # 把相关的键名缓存起来
    fkey2 = 'list:out:%s'%other_uid         #
    timeline = 'list:statuses:%s'%list_id   #

    if not conn.zscore(fkey1, other_uid):   # 如果用户并未关注 other_uid ，
        return None                         # 那么直接返回

    pipeline = conn.pipeline(True)
    pipeline.zrem(fkey1, other_uid)                 # 从相应的有序集合里面移除各个用户ID
    pipeline.zrem(fkey2, list_id)                   #
    pipeline.zcard(fkey1)                           # 获取有序集合的大小
    pipeline.zrevrange('profile:%s'%other_uid,      # 从被取消关注的用户那里获取他最新发布的状态消息
        0, HOME_TIMELINE_SIZE-1)                    #
    following, statuses = pipeline.execute()[-2:]

    pipeline.hset('list:%s'%list_id, 'following', following)    # 对存储列表信息的散列进行更新，将列表的新大小记录到散列里面
    if statuses:
        pipeline.zrem(timeline, *statuses)                      # 从时间线里面移除被取消关注的用户所发布的状态消息
        refill_timeline(fkey1, timeline)                        # 重新填充时间线

    pipeline.execute()
    return True                         # 返回 True 值，表示用户已经被取消关注

def create_user_list(conn, uid, name):
    pipeline = conn.pipeline(True)
    pipeline.hget('user:%s'%uid, 'login')   # 获取创建列表的用户的用户名
    pipeline.incr('list:id:')               # 生成一个新的列表ID
    login, id = pipeline.execute()

    if not login:               # 如果用户不存在，那么直接返回
        return None             #

    now = time.time()

    pipeline = conn.pipeline(True)
    pipeline.zadd('lists:%s'%uid, **{id: now})  # 将新创建的列表添加到用户已经创建了的有序集合里面
    pipeline.hmset('list:%s'%id, {              # 创建记录列表信息的散列
        'name': name,                           #
        'id': id,                               #
        'uid': uid,                             #
        'login': login,                         #
        'following': 0,                         #
        'created': now,                         #
    })
    pipeline.execute()

    return id           # 返回新列表的ID

def post_status(conn, uid, message, **data):
    # 使用之前介绍过的函数来创建一条新的状态消息。
    id = create_status(conn, uid, message, **data)
    # 如果创建状态消息失败，那么直接返回。
    if not id:
        return None

    # 获取消息的发布时间。
    posted = conn.hget('status:%s'%id, 'posted')
    # 如果程序未能顺利地获取消息的发布时间，那么直接返回。
    if not posted:
        return None

    post = {str(id): float(posted)}
    # 将状态消息添加到用户的个人时间线里面。
    conn.zadd('profile:%s'%uid, **post)

    # 将状态消息推送给用户的关注者。
    syndicate_status(conn, uid, post)
    return id

# 函数每次被调用时，最多只会将状态消息发送给一千个关注者。
POSTS_PER_PASS = 1000
def syndicate_status(conn, uid, post, start=0):
    # 以上次被更新的最后一个关注者为起点，获取接下来的一千个关注者。
    followers = conn.zrangebyscore('followers:%s'%uid, start, 'inf',
        start=0, num=POSTS_PER_PASS, withscores=True)

    pipeline = conn.pipeline(False)
    # 在遍历关注者的同时，
    # 对 start 变量的值进行更新，
    # 这个变量可以在有需要的时候传递给下一个 syndicate_status() 调用。
    for follower, start in followers:
        # 将状态消息添加到所有被获取的关注者的定制时间线里面，
        # 并在有需要的时候对关注者的定制时间线进行修剪，
        # 防止它超过限定的最大长度。
        pipeline.zadd('home:%s'%follower, **post)
        pipeline.zremrangebyrank(
            'home:%s'%follower, 0, -HOME_TIMELINE_SIZE-1)
    pipeline.execute()

    # 如果需要更新的关注者数量超过一千人，
    # 那么在延迟任务里面继续执行剩余的更新操作。
    if len(followers) >= POSTS_PER_PASS:
        execute_later(conn, 'default', 'syndicate_status',
            [conn, uid, post, start])

def syndicate_status_list(conn, uid, post, start=0, on_lists=False):
    key = 'followers:%s'%uid            # 根据操作的处理进度（depending on how far along we are），
    base = 'home:%s'                    # 选择对主页时间线还是对用户时间线进行操作
    if on_lists:                        #
        key = 'list:out:%s'%uid         #
        base = 'list:statuses:%s'       #
    followers = conn.zrangebyscore(key, start, 'inf',   # 从上次更新时的最后一个用户或者列表作为起点，
        start=0, num=POSTS_PER_PASS, withscores=True)   # 获取下一组用户或者列表（数量为 1000 个）

    pipeline = conn.pipeline(False)
    for follower, start in followers:                   # 将状态消息添加到所有已获取关注者的主页时间线里面
        pipeline.zadd(base%follower, **post)            #
        pipeline.zremrangebyrank(                       #
            base%follower, 0, -HOME_TIMELINE_SIZE-1)    #
    pipeline.execute()

    if len(followers) >= POSTS_PER_PASS:                    # 如果已经对至少 1000 个用户进行了更新，
        execute_later(conn, 'default', 'syndicate_status',  # 那么将后续的更新操作留到下次再进行
            [conn, uid, post, start, on_lists])             #

    elif not on_lists:
        execute_later(conn, 'default', 'syndicate_status',  # 如果针对列表的操作并未完成，那么对列表进行操作
            [conn, uid, post, 0, True])                     # 如果操作只是对主页时间线执行的话，那么程序无需执行这一步


def delete_status(conn, uid, status_id):
    key = 'status:%s'%status_id
    # 对指定的状态消息进行加锁，防止两个程序同时删除同一条状态消息的情况出现。
    lock = acquire_lock_with_timeout(conn, key, 1)
    # 如果加锁失败，那么直接返回。
    if not lock:
        return None

    # 如果 uid 指定的用户并非状态消息的发布人，那么函数直接返回。
    if conn.hget(key, 'uid') != str(uid):
        release_lock(conn, key, lock)
        return None

    pipeline = conn.pipeline(True)
    # 删除指定的状态消息。
    pipeline.delete(key)
    # 从用户的个人时间线里面移除指定的状态消息 ID 。
    pipeline.zrem('profile:%s'%uid, status_id)
    # 从用户的定制时间线里面移除指定的状态消息 ID 。
    pipeline.zrem('home:%s'%uid, status_id)
    # 对储存着用户信息的散列进行更新，减少已发布状态消息的数量。
    pipeline.hincrby('user:%s'%uid, 'posts', -1)
    pipeline.execute()

    release_lock(conn, key, lock)
    return True

if __name__ == "__main__":
    print("main")
```



### Chapter 9

```python
import binascii

import redis
import time
import uuid
import logging
import urllib.request
import csv
import json
import contextlib
import bisect
import math
import re
import threading
import functools

# 短结构
from numpy import long

'''
list-max-ziplist-entries 512    # 列表结构使用压缩列表表示的限制条件。
list-max-ziplist-value 64       #

hash-max-ziplist-entries 512    # 散列结构使用压缩列表表示的限制条件
hash-max-ziplist-value 64       #（Redis 2.6 以前的版本会为散列结构使用不同的编码表示，并且选项的名字也与此不同）。

zset-max-ziplist-entries 128    # 有序集合使用压缩列表表示的限制条件。
zset-max-ziplist-value 64       #
'''

# 判断压缩结构
'''
conn.rpush('test', 'a', 'b', 'c', 'd')  # 首先将四个元素推入到列表。
conn.debug_object('test') 
conn.rpush('test', 65*'a') 
conn.debug_object('test')
conn.rpop('test')  
conn.debug_object('test')
'''

# 整数集合
'''
set-max-intset-entries 512      # 集合使用整数集合表示的限制条件。
conn.sadd('set-object', *range(500))  
conn.debug_object('set-object')
conn.sadd('set-object', *range(500, 1000))  
conn.debug_object('set-object')
'''

# 为了以不同的方式进行性能测试，函数需要对所有测试指标进行参数化处理。
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
    for p in range(passes):
        # 每个流水线操作都包含了 psize 次 RPOPLPUSH 命令调用。
        for pi in range(psize):
            # 每个 rpoplpush() 函数调用都会将列表最右端的元素弹出，
            # 并将它推入到同一个列表的左端。
            pipeline.rpoplpush(key, key)
        # 执行 psize 次 RPOPLPUSH 命令。
        pipeline.execute()

    # 计算每秒钟执行的 RPOPLPUSH 调用数量。
    return (passes * psize) / (time.time() - t or .001)
'''
long_ziplist_performance(conn, 'list', 1, 1000, 100)            # 当压缩列表编码的列表包含的节点数量不超过 1000 个时，
52093.558416505381                                              # Redis 每秒钟可以执行大约五万次操作。
>>> long_ziplist_performance(conn, 'list', 100, 1000, 100)      #
51501.154762768667                                              #
>>> long_ziplist_performance(conn, 'list', 1000, 1000, 100)     #
49732.490843316067                                              #
>>> long_ziplist_performance(conn, 'list', 5000, 1000, 100)     # 当压缩列表编码的列表包含的节点数量超过 5000 个时，
43424.056529592635                                              # 内存复制带来的消耗就会越来越大，
>>> long_ziplist_performance(conn, 'list', 10000, 1000, 100)    # 导致性能下降。
36727.062573334966                                              #
>>> long_ziplist_performance(conn, 'list', 50000, 1000, 100)    # 当压缩列表的节点数量达到 5000 个时，
16695.140684975777                                              # 性能出现明显下降。
>>> long_ziplist_performance(conn, 'list', 100000, 500, 100)    # 当节点数量达到十万个时，
553.10821080054586                                              # 压缩列表的性能低得根本没法用了。
'''


#分片
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

'''
# <start id="sharded-ip-lookup"/>
TOTAL_SIZE = 320000                                             # 把传递给分片函数的参数设置为全局常量，
SHARD_SIZE = 1024                                               # 确保每次传递的值总是相同的。

def import_cities_to_redis(conn, filename):
    for row in csv.reader(open(filename)):
        ...
        shard_hset(conn, 'cityid2city:', city_id,               # 为了对数据进行设置，用户需要传递 TOTAL_SIZE 参数和 SHARD_SIZE 参数。
            json.dumps([city, region, country]),                # 不过因为这个程序处理的 ID 都是数字，
            TOTAL_SIZE, SHARD_SIZE)                             # 所以 TOTAL_SIZE 实际上并没有被使用。

def find_city_by_ip(conn, ip_address):
    ...
    data = shard_hget(conn, 'cityid2city:', city_id,            # 程序在获取数据时，
        TOTAL_SIZE, SHARD_SIZE)                                 # 需要根据相同的 TOTAL_SIZE 参数和 SHARD_SIZE 参数查找被分片的键。
    return json.loads(data)
# <end id="sharded-ip-lookup"/>
'''


def shard_sadd(conn, base, member, total_elements, shard_size):
    shard = shard_key(base,
                      # 计算成员应该被储存到哪个分片集合里面；
                      # 因为成员并非连续 ID ，所以程序在计算成员所属的分片之前，会先将成员转换为字符串。
                      'x' + str(member), total_elements, shard_size)
    # 将成员储存到分片里面。
    return conn.sadd(shard, member)

# 为整数集合编码的集合预设一个典型的分片大小。
SHARD_SIZE = 512

def count_visit(conn, session_id):
    # 取得当天的日期，并生成唯一访客计数器的键。
    today = date.today()
    key = 'unique:%s' % today.isoformat()
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
        expected = conn.get('unique:%s' % yesterday)
        expected = int(expected or DAILY_EXPECTED)

        # 基于“明天的访客人数至少会比今天的访客人数多 50%”这一假设，
        # 给昨天的访客人数加上 50% ，然后向上舍入至下一个底数为 2 的幂。
        expected = 2 ** int(math.ceil(math.log(expected * 1.5, 2)))
        # 将计算出的预计访客人数写入到 Redis 里面，以便其他程序在有需要时使用。
        if not conn.setnx(exkey, expected):
            # 如果在我们之前，
            # 已经有其他客户端储存了当日的预计访客人数，
            # 那么直接使用已储存的数字。
            expected = conn.get(exkey)

            # 将当日的预计访客人数记录到本地副本里面，并将它返回给调用者。
    EXPECTED[key] = int(expected)
    return EXPECTED[key]
# 打包二进制

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
'''
Redis的键值最大能存储512M空间
'''

# 设置每个分片的大小。
USERS_PER_SHARD = 2 ** 20


def set_location(conn, user_id, country, state):
    # 取得用户所在位置的编码。
    code = get_code(country, state)

    # 查找分片 ID 以及用户在指定分片中的位置（position）。
    shard_id, position = divmod(user_id, USERS_PER_SHARD)
    # 计算用户数据的偏移量。
    offset = position * 2

    pipe = conn.pipeline(False)
    # 将用户的位置信息储存到分片后的位置表格里面。
    pipe.setrange('location:%s' % shard_id, offset, code)

    # 对记录目前已知最大用户 ID 的有序集合进行更新。
    tkey = str(uuid.uuid4())
    pipe.zadd(tkey, 'max', user_id)
    pipe.zunionstore('location:max',
                     [tkey, 'location:max'], aggregate='max')
    pipe.delete(tkey)

    pipe.execute()

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
    for shard_id in range(max_block + 1):
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
if __name__ == "__main__":
    print("main")
```



### Chapter 10

```python
import binascii

import redis
import time
import uuid
import logging
import urllib.request
import csv
import json
import contextlib
import bisect
import math
import re
import threading
import functools

# 短结构
from numpy import long

# 提高读性能
'''
主从服务设置
查看主从信息：INFO replication

主从从设置
查看主从从信息：INFO replication

：一般来说主从从架构中的从服务器只有读的权限
'''

# 扩展写性能和内存容量, 分片和不同的redis服务器处理
import binascii
from collections import defaultdict
from datetime import date
from decimal import Decimal
import functools
import json
from Queue import Empty, Queue
import threading
import time
import unittest
import uuid

import redis

CONFIGS = {}
CHECKED = {}


def get_config(conn, type, component, wait=1):
    key = 'config:%s:%s' % (type, component)

    if CHECKED.get(key) < time.time() - wait:  # A
        CHECKED[key] = time.time()  # B
        config = json.loads(conn.get(key) or '{}')  # C
        config = dict((str(k), config[k]) for k in config)
        old_config = CONFIGS.get(key)  # D

        if config != old_config:  # E
            CONFIGS[key] = config  # F

    return CONFIGS.get(key)


REDIS_CONNECTIONS = {}
config_connection = None


def redis_connection(component, wait=1):  # A
    key = 'config:redis:' + component  # B

    def wrapper(function):  # C
        @functools.wraps(function)  # D
        def call(*args, **kwargs):  # E
            old_config = CONFIGS.get(key, object())  # F
            _config = get_config(  # G
                config_connection, 'redis', component, wait)  # G

            config = {}
            for k, v in _config.iteritems():  # L
                config[k.encode('utf-8')] = v  # L

            if config != old_config:  # H
                REDIS_CONNECTIONS[key] = redis.Redis(**config)  # H

            return function(  # I
                REDIS_CONNECTIONS.get(key), *args, **kwargs)  # I

        return call  # J

    return wrapper  # K


def index_document(conn, docid, words, scores):
    pipeline = conn.pipeline(True)
    for word in words:  # I
        pipeline.sadd('idx:' + word, docid)  # I
    pipeline.hmset('kb:doc:%s' % docid, scores)
    return len(pipeline.execute())  # J


def parse_and_search(conn, query, ttl):
    id = str(uuid.uuid4())
    conn.sinterstore('idx:' + id,
                     ['idx:' + key for key in query])
    conn.expire('idx:' + id, ttl)
    return id


def search_and_sort(conn, query, id=None, ttl=300, sort="-updated",  # A
                    start=0, num=20):  # A
    desc = sort.startswith('-')  # B
    sort = sort.lstrip('-')  # B
    by = "kb:doc:*->" + sort  # B
    alpha = sort not in ('updated', 'id', 'created')  # I

    if id and not conn.expire(id, ttl):  # C
        id = None  # C

    if not id:  # D
        id = parse_and_search(conn, query, ttl=ttl)  # D

    pipeline = conn.pipeline(True)
    pipeline.scard('idx:' + id)  # E
    pipeline.sort('idx:' + id, by=by, alpha=alpha,  # F
                  desc=desc, start=start, num=num)  # F
    results = pipeline.execute()

    return results[0], results[1], id  # G


def zintersect(conn, keys, ttl):
    id = str(uuid.uuid4())
    conn.zinterstore('idx:' + id,
                     dict(('idx:' + k, v) for k, v in keys.iteritems()))
    conn.expire('idx:' + id, ttl)
    return id


def search_and_zsort(conn, query, id=None, ttl=300, update=1, vote=0,  # A
                     start=0, num=20, desc=True):  # A

    if id and not conn.expire(id, ttl):  # B
        id = None  # B

    if not id:  # C
        id = parse_and_search(conn, query, ttl=ttl)  # C

        scored_search = {  # D
            id: 0,  # D
            'sort:update': update,  # D
            'sort:votes': vote  # D
        }
        id = zintersect(conn, scored_search, ttl)  # E

    pipeline = conn.pipeline(True)
    pipeline.zcard('idx:' + id)  # F
    if desc:  # G
        pipeline.zrevrange('idx:' + id, start, start + num - 1)  # G
    else:  # G
        pipeline.zrange('idx:' + id, start, start + num - 1)  # G
    results = pipeline.execute()

    return results[0], results[1], id  # H


def execute_later(conn, queue, name, args):
    t = threading.Thread(target=globals()[name], args=tuple(args))
    t.setDaemon(1)
    t.start()


HOME_TIMELINE_SIZE = 1000
POSTS_PER_PASS = 1000


def shard_key(base, key, total_elements, shard_size):  # A
    if isinstance(key, (int, long)) or key.isdigit():  # B
        shard_id = int(str(key), 10) // shard_size  # C
    else:
        shards = 2 * total_elements // shard_size  # D
        shard_id = binascii.crc32(key) % shards  # E
    return "%s:%s" % (base, shard_id)  # F


def shard_sadd(conn, base, member, total_elements, shard_size):
    shard = shard_key(base,
                      'x' + str(member), total_elements, shard_size)  # A
    return conn.sadd(shard, member)  # B


SHARD_SIZE = 512
EXPECTED = defaultdict(lambda: 1000000)


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
    shard = shard_key(component, 'x' + str(key), shard_count, 2)
    # 返回连接。
    return get_redis_connection(shard, wait)


# <end id="get-sharded-connection"/>


def log_recent(conn, app, message):
    'the old log_recent() code'


log_recent = redis_connection('logs')(log_recent)  # 通过反复执行 3 次这行代码，可以达到和装饰器一样的效果


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


# <end id="shard-aware-decorator"/>


# 将 count_visit() 函数分片到 16 台机器上面执行，
# 执行所得的结果将被自动地分片到每台机器的多个数据库键上面。
@sharded_connection('unique', 16)
def count_visit(conn, session_id):
    today = date.today()
    key = 'unique:%s' % today.isoformat()
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


# 扩展复杂查询
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
# <end id="search-with-values"/>


# 代码清单 10-6
# <start id="search-on-shards"/>
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
# <end id="search-on-shards"/>

def get_values_thread(component, shard, wait, rqueue, *args, **kwargs):
    conn = get_redis_connection('%s:%s'%(component, shard), wait)
    count, results, id = search_get_values(conn, *args, **kwargs)
    rqueue.put((shard, count, results, id))

def get_shard_results_thread(component, shards, query, ids=None, ttl=300,
                  sort="-updated", start=0, num=20, wait=1, timeout=.5):

    ids = ids or shards * [None]
    rqueue = Queue()

    for shard in range(shards):
        t = threading.Thread(target=get_values_thread, args=(
            component, shard, wait, rqueue, query, ids[shard],
            ttl, sort, start, num))
        t.setDaemon(1)
        t.start()

    received = 0
    count = 0
    data = []
    deadline = time.time() + timeout
    while received < shards and time.time() < deadline:
        try:
            sh, c, r, i = rqueue.get(timeout=max(deadline-time.time(), .001))
        except Empty:
            break
        else:
            count += c
            data.extend(r)
            ids[sh] = i

    return count, data, ids


# 代码清单 10-7
# <start id="merge-sharded-results"/>
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

if __name__ == "__main__":
    print("main")
```

