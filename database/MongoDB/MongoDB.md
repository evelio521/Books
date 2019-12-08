## Chapter 1

> 1. 内置对MapReduce的支持。
> 2. 对地理空间索引的支持。
> 3. 面向文档的数据库。一个文档可以表示非常复杂的层次关系。
> 4. 不具备连接和复杂的多项事务。
> 5. MongoDB自己管理服务集群，主动切换备份服务器，在集群加入新的结点等。

## Chapter 2

###文档

> 1. 文档的键值是有顺序的。
>
>    {"foo":2, "greeting":"hello world!"} 和 {"greeting":"hello world!","foo":2} 不同。
>
> 2. 键不能含有'\0', 他表示字符串结束。
>
> 3. . $ 有特殊意义，只有在特定环境。
>
> 4. MongoDB 区分大小写，也缺分类型。
>
> 5. _ 开头的键值是保留的。
>
> 6. 不能有重复的键。

### 集合

> 1. 集合就是一组文档，类似于关系数据库中的表。
> 2. 集合是无模式，一个集合可以任意存放。
> 3. 多个集合是为了对文档有序的管理。
> 4. 组织集合一般用"."来区分，两个集合实质上没有任何关系。

### 数据库

> 1. 一个MongoDB可以承载多个数据库，之间完全独立。
> 2. 每个数据库都有独立的权限控制，不同数据库存放不同的磁盘上。
> 3. 数据库名最终会变成文件系统里的文件。admin ，local和 config是保留的数据库。
> 4. 使用集合必须带上数据库名， "." 分割。

### 启动MongoDB

> 1. 默认箭筒27017端口。 ./mongid

### MongoDB Shell

> 1. ./mongo
> 2. \> use db
> 3. \> db

```
# 基本操作
post = {
    "title" : "My Blog Post",
    "content" : "Here's my blog post",
    "data": new Data() 
}

db.blog.insert(post)

db.blog.find()

db.blog.findOne()

post.comments = []

db.blog.update({"title": "My Blog Post"}, post)

db.blog.remove({"title": "My Blog Post"})

help

db.db.update

db.getCollection("version") # version是数据库类的一个属性，用这种方式

var collections = ["posts", "comments", "authors"]
for (i in collections) {
    doStuff(db.blog[collections[i]])
}
```

```
# 基本数据类型
1. null 
{"x" : null} 空值或不存在的值
2. 布尔
{"x": true} true or false
3. 32位整数
4. 64位整数
5. 64位浮点数
{"x" : 3.14}
6. 字符串
{"x" : "foobar"}
7. 符号
8. 对象id
{"x" : ObjectId()}
9. 日期
{"x" : new Date()}
10. 正则表达式
{"x" : /foobar/i}
11. 代码
{"x" : function() {/* ... */}}
12. 二进制数据
13. 最大值
14. 最小值
15. 未定义
{"x" : undefined}
16. 数组
{"x" : ["a", "b", "c"]}
17. 内嵌文档
{"x" : {"foo" : "bar"}}
```

> 1. shell 下都是双精度数，所以不要再shell下覆盖整个文档。
> 2. 每个文档都有一个 _id 是ObjectId

```
number = {
    "myInteger" : {
        "floatApprox" : 3
    }
}
db.num.insert(number)
db.num.findOne()
#doc.myInteger + 1
db.num.remove({"myInteger":"floatApprox"})

{"things" : ["pie", 3.14]}

{
    "name" : "John Doe",
    "address" : {
        "street" : "Park Street"
    }
}

```

## Chapter 3

> 1. 插入的文档不能超过4M
> 2. 不做数据校验，只是简单的插入数据
> 3. MongoDB 会预留一些空白文档来适应文档的大小，当文档数量不够使的情况下，会分配新的空间

```
db.foo.insert({"bar":"baz"}) # 插入一个文档
db.users.remove() # 删除所有文档
db.mailing.list.remove({"opt-out": true}) # 删除符合条件的文档
db.drop_collection("bar") # 删除速度最快

# 替换文档
var joe = db.users.findOne({"name" : "joe"});
joe.relationships = {"friends" : joe.friends, "enemies": joe.enemies};
joe.username = joe.name;
delete joe.friend;
db.users.upadte({"name": "joe"}, joe)

joe = db.people.findOne({"name" : "joe", "age" : 20});
joe.age++;
db.people.update({"name": "joe"}, joe) # 会失败，因为有多个，最好用_id查找

#使用修改器, 一定要用$ 开头的修改器，否则会把整个文档替换掉
db.analytics.update({"url" : "www.baidu.com"}, {"$inc" :{"pageviews" : 1}}) # _id 不能改变
db.analytics.find()

db.users.update({"_id": ObjectId("xxxxxxxxxxxxx")}, {"$set" :{"fb" : "eve"}})
db.users.find()
db.users.update({"_id": ObjectId("xxxxxxxxxxxxx")}, {"$set" :{"fb" : ["eve", "fvf"]}})
db.users.update({"_id": ObjectId("xxxxxxxxxxxxx")}, {"$unset" :{"fb" : 1}})

db.foo.insert({"count": "1"})
db.foo.update({}, {"$inc" : {"count" :1}})

#数组修改器
db.blog.posts.update({"title" : "tt"}， 
                     {$push} : {"comments" : {"name" : "joe", "email":"ee@126.com"}}})
db.papers.update({"authors cited": {"$ne" : "Richie"}}, {$push : {"authors": "Richie"}})

# 防止重复加入
db.users.update({"_id": ObjectId"xxxxx"}, {$addToSet : {"email" : "joe@gmail.com"}}) 

db.users.update({"_id": ObjectId"xxxxx"}, 
    {$addToSet : {"email" : {"$each": ["joe@gmail.com","joe@gmail.com","joe@gmail.com"]}}})
    
db.lists.insert({"todo": ["dishes", "dry"]})
db.lists.update({}, {"$pull" : {"todo" : "dry"}})

db.blog.update({"post" : post_id},
    {"$inc": {"comments.0.votes" : 1}})

db.blog.update({"comments.author" : "John"},
    {"$set": {"comments.$.author" : "Jim"}})

db.analytics.update({"url": "/blog"}, {"$inc": {"visit" : 1}}, true) # 第三个参数true 代表upsert

# save 是个shell命令
var x = db.foo.findOne()
x.num = 42
db.foo.save(x) # 等价于 db.foo.update({"_id": x._id}, x)

# 第三个参数true 代表upsert 第四个参数为true 代表更新所有匹配的文档
db.analytics.update({"url": "/blog"}, {"$inc": {"visit" : 1}}, true, true) 
db.runCommand({getLastError : 1})

ps = db.runCommand({"findAndModify" : "processess",
    "query" : {"status" : "READY"},
    "sort" : {"priority" : -1},
    "remmove" : true}).value
```



## Chapter 4

> 1. find查询中，不能使用变量，要通过其他方法实现
> 2. 不要用skip做分页，会影响性能，可以使用data做分页
> 3. 游标有超时功能，但可以设置不让其超时

```
db.users.find({"username": "joe", "age" : 27})
db.users.find({}, {"username": 1, "age" : 1})
db.users.find({}, {"username": 0, "age" : 0})
db.users.find({}, {"username": 0, "age" : 1})

db.users.find({ "age" : {"$gte: 18", "$lte" : 30})

date = new Data("01/01/2007")
db.users.find({"registered" : {"$lt" : start}})
db.users.find({ "age" : {"$ne": 19 })
db.raffle.find({"ticket_no" : {"$in" :[723, 888]}})
db.raffle.find({"ticket_no" : {"$nin" :[723, 888]}})

db.raffle.find({"$or" :[{"ticket_no" : {"$nin" :[723, 888]}},
    {"winner" : true}]})

db.users.find({"id_num" : {"$mod" : [5, 1]}})
db.users.find({"id_num" : {"$not" : {"$mod" : [5, 1]}}})
db.users.find({"age": {"$lt" : 30, "$gt" : 20}})

db.c.find()
db.c.find({"y" : null})
db.c.find({"z" : {"$in" :[null], "$exists" : true}}) # 只查找存在的文档

db.users.find({"name" : /joey?/i}) 

db.users.insert({"fruit" : ["a", "b"]})
db.users.find({"fruit" : "a"})
db.users.find({"fruit" : {"$all" : ["a", "b"]}}})
db.users.find({"fruit.1" : "b")
db.users.find({"fruit" : {"$size" : 3}}) # $size 不能同$gt一起用

# 解决$size 不能同$gt一起用
db.food.update({"$push" : {"fruit" : "strawberry"}, "$inc" : {"size" : 1}})
db.users.find({"size" : {"$gt" : 3}})

#返回前10条查询结果
db.blogs.post.findOne(criteria, {"comments" : {"$slice" : 10}})
#返回前后10条查询结果
db.blogs.post.findOne(criteria, {"comments" : {"$slice" : -10}})
#返回24~33个元素
db.blogs.post.findOne(criteria, {"comments" : {"$slice" : [23, 10]}})

#内嵌文档查询
db.people.find({"name.first" : "joe", "name.last" : "schmoe"})
db.blog.find({"comments" : {"$elemMatch" : {"author" : "joe", 
                                            "score" : {"$gte" : 5}}}})
                                            
# where
db.foo.insert({"apple" : 1, "banana" : 6, "peach" : 3})
db.foo.insert({"apple" : 1, "spinach" : 4, "watermelon" : 4})

db.foo.find({"$where": function() {
    for (var current in this) {
        for (var other in this) {
            if (current != other && this[current] == this[other]) {
                return true;
            }
        }
    }
    return false;
}})

db.foo.find({"$where" : "this.x + this.y == 10"})
db.foo.find({"$where" : "function () {return this.x + this.y == 10;}"})

#游标
for(i = 0; i < 100; i++) {
    db.c.insert({x : 1});
}
var cursor = db.c.find()

while (cursor.hasNext()) {
    obj = cursor.next();
    // ...
}

var cursor = db.people.find();
cursor.forEach(function(x) {
    printf(x.name);
});

var.cursor = db.foo.find().sort({"x" : 1}).limit(1).skip(10); // sort limit skip顺序无关

#随机选取一份文档
db.people.insert({"name": "joe", "random" : Math.random()})
db.people.insert({"name": "joe1", "random" : Math.random()})
db.people.insert({"name": "joe2", "random" : Math.random()})

var random = Math.random()
result = db.foo.findOne({"random" : {"$gt" : random}})

if (result == null) {
    result = db.foo.findOne({"random" : {"$gt" : random}})
}

db.people.ensureIndex({"profession" : 1, "state" : 1, "random" : 1})
```

## Chapter 5

> 1. 索引建立字段的顺序是有关的，{"date": 1, "username": -1} 和 {"username": -1, "date": 1}是不同的
> 2. system.indexes 包含每个索引详细信息
> 3. system.namespaces 包含每个索引的名字

```
db.people.ensureIndex({"date": 1, "username": 1})
db.people.ensureIndex({"date": 1, "username": -1}) # -1 反向或降序
db.blog.ensureIndex({"comments.date" : 1})
db.people.ensureIndex({"date": 1, "username": 1}, {"name": "alphabet"})
db.people.ensureIndex({"username": 1}, {"unique" : true, "dropDups": true})
db.people.ensureIndex({"username": 1, "date": 1}, {"unique" : true, "dropDups": true})

db.people.find().explain()
db.people.find({"age" : {$gt : 20, $lt : 30}}).explain()

db.c.find({"age": 14, "username" : /.*/}).hint({"username":1, "age" : 1}) # 强制使用某个类型索引

db.people.ensureIndex({"date": 1, "username": 1}, {"background" : true}) #后台建立索引
db.runCommand({"dropIndexes" : "foo", "index" : "alphabet"}) # 删除索引
db.runCommand({"dropIndexes" : "foo", "index" : "*"}) # 删除索引
```

```
# 地理空间索引
db.map.ensureIndex({"gps": "2d"})
db.map.ensureIndex({"gps": [0， 100]})
db.map.ensureIndex({"gps": ["x" : -30, "y" : 30]})
db.map.ensureIndex({"gps": {"latitude" : -180, "longitude": 180}})

db.map.find({"gps" : {"$near" :[40, -73]}}).limit(10) # 不会给出距离
db.runCommand({geoNear: "map", near : [40, -73], num : 10}); # 会给出距离

db.map.find({"gps": {"$within" : {"$box" : [[10, 20], [15, 30]]}}})
db.map.find({"gps": {"$within" : {"$center" : [[10, 20], 5]}}})

#复合地理空间索引
db.map.ensureIndex({"location": "2d", "desc" : 1})
db.map.find({"location" : {"$near" : [-70, 30]}, "desc" : "coffee"}).limit(1)
```



## Chapter 6

> 1.

```
db.foo.count()
db.foo.insert({"x" : 1})
db.foo.count()
db.foo.count({"x" : 1})

db.runCommand({"distinct" : "people", "key", "age"})

db.runCommand({"group" :{
    "ns" : "stocks",
    "key": "day",
    "initial" : {"time" : 0},
    "$reduce" : function(doc, prev) {
        if (doc.time > prev.time) {
            prev.price = doc.price;
            prev.time = doc.time;
        }
    },
    "condition" : {"day" : {"$gt" : "2010/09/30"}}
}})

db.runCommand({"group" :{...}}) # 返回文档总数和有多少个键值

db.runCommand({"group" :{
    "ns" : "posts",
    "key": "{"tags" : true}",
    "initial" : {"tags" : 0},
    "$reduce" : function(doc, prev) {
        for (i in doc.tags) {
            if (doc.tags[i] in prev.tags) {
                prev.tags[doc.tags[i]]++;
            } else {
                prev.tags[doc.tags[i]] = 1;
            }
        }
    },
    "finalize" : function(prev) {
        var mostPopular = 0;
        for (i in prev.tags) {
            if (prev.tags[i] > mostPopular) {
                prev.tag = i;
                mostPopular = prev.tags[i]； 
            }
        }
        delete prev.tags;
    }
}})

```

```shell
# mapreduce
>db.posts.insert({
   "post_text": "菜鸟教程，最全的技术文档。",
   "user_name": "mark",
   "status":"active"
})

>db.posts.insert({
   "post_text": "菜鸟教程，最全的技术文档。",
   "user_name": "mark",
   "status":"active"
})

>db.posts.insert({
   "post_text": "菜鸟教程，最全的技术文档。",
   "user_name": "mark",
   "status":"active"
})

>db.posts.insert({
   "post_text": "菜鸟教程，最全的技术文档。",
   "user_name": "mark",
   "status":"active"
})

>db.posts.insert({
   "post_text": "菜鸟教程，最全的技术文档。",
   "user_name": "mark",
   "status":"disabled"
})

>db.posts.insert({
   "post_text": "菜鸟教程，最全的技术文档。",
   "user_name": "runoob",
   "status":"disabled"
})

>db.posts.insert({
   "post_text": "菜鸟教程，最全的技术文档。",
   "user_name": "runoob",
   "status":"disabled"
})

>db.posts.insert({
   "post_text": "菜鸟教程，最全的技术文档。",
   "user_name": "runoob",
   "status":"active"
})

>db.posts.mapReduce( 
   function() { emit(this.user_name,1); }, 
   function(key, values) {return Array.sum(values)}, 
      {  
         query:{status:"active"},  
         out:"post_total" 
      }
).find()
```

![image-20191208133630272](/Users/sunqiang/Library/Application Support/typora-user-images/image-20191208133630272.png)

```
db.runCommand({"mapreduce":"analytics", "map" : map, "reduce":reduce,
              "query" : {"date" : {"$gt" : week_go}}})
db.runCommand({"mapreduce":"analytics", "map" : map, "reduce":reduce,
              "query" : {"date" : {"$gt" : week_go}}, "limit" : 1000, "sort" : {"date" : -1}})
```



## Chapter 7

```shell
db.count.update({x : 1}, {"$inc" : {x : 1}}, false, true)
db.runCommand({getLastError : 1})
db.runCommand({"drop" : "test"});
db.listCommands()
# http:://localhost:28017/_commands

```

```shell
# 固定集合， 默认情况下没有索引
db.createCollection("my_collection", {capped : true, size : 10000, max :100})
db.runCommand({convertToCapped: "test", size: 10000})
db.my_collection.find().sort({"$natural" : -1})

```

```shell
# GridFS ： 存储大二进制文件
# 1. 利用已经建立的复制或分片机制，对于文件存储来说故障和扩展都很容易
# 2. 可以避免存储用户上传内容文件系统出现某些问题
# 3. 不产生磁盘碎片

echo "Hello World!!1" > foo.txt
./mongofiles put foo.txt
./mongofiles list
./mongofiles get foo.txt

```

```python
# python mongodb GrindFS
from pymongo import Connection
import gridfs
db = Connection().test
fs = gridfs.GrigFS(db)
file_id = fs.pus("hell", filename = "foo.txt")
fs.list
fs.get(file_id).read()
```

```shell
# 服务器脚本
db.eval("return 1;")
db.eval("function() {return 1;}")
db.eval("function(u) {return 1;}", [username])
db.eval("function(u,v) {return 1;}", [username, passwd])
#system.js用于存储JavaScript变量

db.system.js.insert({"_id" : "x", "value" : 1})
db.system.js.insert({"_id" : "y", "value" : 2})
db.system.js.insert({"_id" : "z", "value" : 3})
db.eval("return x + y +z;")

db.system.js.insert({"_id" : "log", "value" :
    function(msg, level) {
        var levels = ["DEBUG", "WARN", "ERROR", "FATAL"];
        level = level ? level : 0; 
        var now = new Date();
        print(now + " " + level[level] + msg)
    }
})

db.eval("x = 1; log('x is' + x); x = 2; log('x is greater than 1', 1);");

```

## Chapter 8

```shell
./mongodb --dbpath path --port 5585 --fork --logpath m.log --config ./config.conf
# [--dbpath path --port 5585 --fork --logpath m.log --config ./config.conf]- > mongodb.conf
db.shutdownServer()
# http://localhost:28017
# --nohttpinterface 关闭管理接口
db.runCommand({"serverStatus" : 1}) # 服务器状态
db.runCommand({"mongostat" : 1}) # 服务器重要信息
```

```shell
# 用户认证 admin中的用户是超级用户 system.user
use admin
db.addUser("root", "abcd")
use test
b.addUser("testUser", "abcd")
b.addUser("read_only", "abcd", true)
# 重启服务器 加入 --auth

use test
db.test.find() # error
db.auth("read_only", "abcd")
db.test.find() # ok

db.system.users.remove({"user" : "test_user"}) # 删除某个用户

mongod --bindip localhost # 只能在本地访问服务

```

```shell
#备份和恢复 
mongodump -d test -o backup
mongorestore -d test --drop backup/test

# fsync 和 锁
db.runCommand({"fsync" : 1, "lock" : 1}) # 锁定写操作进行备份
db.repaireDatabase() # 修复数据库，这是最后没有办法使用
```

## Chapter 9

```shell
# 主从复制
mkdir -p ~/dbs/master
./mongod --dbpath ~/dbs/master --port 10000 --master
mkdir -p ~/dbs/slave
./mongod --dbpath ~/dbs/slave --port 10001 --slave --source localhost:10000

# 添加及删除源
./mongod --slave --dbpath ~/dbs/slave --port 20718
use local
db.sources.insert({"host" : "localhost:20718"})
db.sources.remove({"host" : "localhost:20718"})

# 副本集
cat /etc/hostname
mkdir -p ~/dbs/node1 ~/dbs/node2

./mongod --dbpath ~/dbs/node1 --port 10001 --replSet blort/morton:10002
./mongod --dbpath ~/dbs/node2 --port 10002 --replSet blort/morton:10001
./mongod --dbpath ~/dbs/node3 --port 10003 --replSet blort/morton:10002, morton:10001

## 初始化副本集
./mongo morton:10001/admin
db.runCommand({"replSetInitiate" : {
    "_id" : "blort",
    "members" : [
        {
            "_id" : 1,
            "host" : "morton:10001"
        },
        {
            "_id" : 2,
            "host" : "morton:10002"
        }
    ]
}})


```

> mongodb副本集的定义
>
> 副本集（Replica Set）是一组MongoDB实例组成的集群，由一个主（Primary）服务器和多个备份（Secondary）服务器构成。通过Replication，将数据的更新由Primary推送到其他实例上，在一定的延迟之后，每个MongoDB实例维护相同的数据集副本。通过维护冗余的数据库副本，能够实现数据的异地备份，读写分离和自动故障转移
>
> 副本集角色
>
> 主节点（Primary）
> 接收所有的写请求，然后把修改同步到所有Secondary。一个Replica Set只能有一个Primary节点，当Primary挂掉后，其他Secondary或者Arbiter节点会重新选举出来一个主节点。
> 默认读请求也是发到Primary节点处理的，可以通过修改客户端连接配置以支持读取Secondary节点。
>
> 副本节点（Secondary）
> 与主节点保持同样的数据集。当主节点挂掉的时候，参与选主。
>
> 仲裁者（Arbiter）
> 不保有数据，不参与选主，只进行选主投票。使用Arbiter可以减轻数据存储的硬件需求，Arbiter几乎没什么大的硬件资源需求，但重要的一点是，在生产环境下它和其他数据节点不要部署在同一台机器
>
> **副本集中数据同步过程**：**Primary节点写入数据，Secondary通过读取Primary的oplog得到复制信息，开始复制数据并且将复制信息写入到自己的oplog**。如果某个操作失败，则备份节点停止从当前数据源复制数据。如果某个备份节点由于某些原因挂掉了，当重新启动后，就会自动从oplog的最后一个操作开始同步，同步完成后，将信息写入自己的oplog，由于复制操作是先复制数据，复制完成后再写入oplog，有可能相同的操作会同步两份，不过MongoDB在设计之初就考虑到这个问题，将oplog的同一个操作执行多次，与执行一次的效果是一样的。**简单的说就是：**
>
> 当Primary节点完成数据操作后，Secondary会做出一系列的动作保证数据的同步：
> 1：检查自己local库的oplog.rs集合找出最近的时间戳。
> 2：检查Primary节点local库oplog.rs集合，找出大于此时间戳的记录。
> 3：将找到的记录插入到自己的oplog.rs集合中，并执行这些操作。
>
> ​    副本集的同步和主从同步一样，都是异步同步的过程，不同的是副本集有个自动故障转移的功能。其原理是：slave端从primary端获取日志，然后在自己身上完全顺序的执行日志所记录的各种操作（该日志是不记录查询操作的），这个日志就是local数据 库中的oplog.rs表，默认在64位机器上这个表是比较大的，占磁盘大小的5%，oplog.rs的大小可以在启动参数中设 定：--oplogSize 1000,单位是M。
>
>    注意：在副本集的环境中，要是所有的Secondary都宕机了，只剩下Primary。最后Primary会变成Secondary，不能提供服务。

## Chapter 10

> **分片组件**
>
> 分片集群由分片、mongos路由器和配置服务器组成。
>
> 分片
>
> MongoDB分片集群将数据分布在一个或多个分片上。每个分片部署成一个MongoDB副本集，该副本集保存了集群整体数据的一部分。因为每个分片都是一个副本集，所以他们拥有自己的复制机制，能够自动进行故障转移。你可以直接连接单个分片，就像连接单独的副本集一样。但是，如果连接的副本集是分片集群的一部分，那么只能看到部分数据。
>
> mongos路由器
>
> 如果每个分片都包含部分集群数据，那么还需要一个接口连接整个集群。这就是mongos。mongos进程是一个路由器，将所有的读写请求指引到合适的分片上。如此一来，mongos为客户端提供了一个合理的系统视图。
>
> mongos进程是轻量级且非持久化的。它们通常运行与与应用服务器相同的机器上，确保对任意分片的请求只经过一次网络跳转。换言之，应用程序连接本地的mongos，而mongos管理了指向单独分片的连接。
>
> 配置服务器
>
> 如果mongs进程是非持久化的，那么必须有地方能持久保存集群的公认状态；这就是配置服务器的工作，其中持久化了分片集群的元数据，改数据包括：每个数据库，集合和特定范围数据的位置；一份变更记录，保存了数据在分片之间进行迁移的历史信息。配置服务器中保存的元数据是某些特定功能和集群维护是的重中之重。举例来说，每次有mongos进程启动，它都会从配置服务器中获取一份元数据的副本。没有这些数据，就无法获得一致的分片集群视图。该数据的重要性对配置服务器的设计和部署也有影响。

```shell
#MongoDB分片集群搭建

#环境
Centos 7.5 1804
MongoDB 4.0.1

shard分片主机：
    shard1: IP:192.168.1.1
    shard2: IP:192.168.1.2
    shard2: IP:192.168.1.3
    #三台主机分别启动三个mongod实例：
        mongod1: 端口: 27017
        mongod2: 端口: 27018
        mongod2: 端口: 27019

configsrv主机：
    IP:192.168.1.4
        mongod1: 端口: 27019
        mongod2: 端口: 37018
        mongod2: 端口: 47019

Route主机：
    192.168.1.5
        mongods: 端口: 27017
        
# 启动配置服务器
mkdir -p ~/dbs/config ~/dbs/config1 ~/dbs/config2
./mongod --dbpath ~/dbs/config --port 20000
./mongod --dbpath ~/dbs/config1 --port 20001
./mongod --dbpath ~/dbs/config2 --port 20002

#启动Routes主机
./mongos --port 30000 --configdb localhost:20000,localhost:20001,localhost:20002

#添加片
mkdir -p ~/dbs/shared1 ~/dbs/shared2 ~/dbs/shared3
./mongod --dbpath ~/dbs/shared1 --port 10000
./mongod --dbpath ~/dbs/shared2 --port 10001
./mongod --dbpath ~/dbs/shared3 --port 10002

./mongo localhsot:30000/admin
db.runCommand({addshard : "localhost:10000", allowLocal: true})
db.runCommand({addshard : "localhost:10001", allowLocal: true})
db.runCommand({addshard : "localhost:10002", allowLocal: true})

# 打开分片功能
db.runCommand({"enablesharding": "foo"})
db.runCommand({"sharedcollection" : "foo.bar", "key": {"_id" : 1}})
```

```shell
#管理分片
#启动mongos
use config
db.shards.find()
db.databases.find()
db.chunks.find()
db.printShardingStatus()
db.runCommand({"removeshard": "localhost:10000"})
db.runCommand({"moveprimary": "test", "to" : "localhost:10001"})

```



> 1. local数据库主要存储副本集的元数据。
> 2. admin数据库则主要存储MongoDB的用户、角色等信息。
> 3. 使用`mongo`进入mongo shell后，会发现默认进入的test数据库。
> 4. db 代表当前数据库。
> 5. config 表示sessions配置信息