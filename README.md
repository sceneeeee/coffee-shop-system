# coffee-shop-system

一个基于 **C 语言** 实现的命令行咖啡店点单与管理系统。  
项目支持 **经理端** 和 **顾客端** 两种使用角色，包含饮品管理、顾客管理、点单、充值、订单历史查看、销售统计和数据持久化等功能。

仓库中：

- `complete/`：最终版本，推荐运行与展示
- `preview/`：较早版本（偏中文、未完全优化）

---

## 项目特点

- C 语言实现，适合基础数据结构与模块化编程练习
- 命令行交互，逻辑清晰
- 区分经理与顾客两类角色
- 支持饮品定制
- 支持订单历史与销售统计
- 支持二进制文件持久化保存数据
- 使用 `Makefile` 一键编译

---

## 项目结构

```text
coffee-shop-system/
├── README.md
├── complete/
│   ├── Makefile
│   ├── coffee_shop.h
│   ├── main.c
│   ├── manager.c
│   ├── customer.c
│   ├── data.c
│   └── util.c
└── preview/
````

---

## 功能模块说明

### 1. 主程序入口 `main.c`

主菜单提供三个入口：

1. Manager login
2. Customer self-service
3. Exit

系统启动时会：

* 初始化默认饮品（如果当前没有饮品数据）
* 加载历史数据文件
* 循环显示主菜单，直到用户选择退出

默认经理密码为：

```text
manager123
```

---

### 2. 公共头文件 `coffee_shop.h`

该文件定义了整个系统的核心常量、结构体和函数声明。

#### 常量

* `MAX_CUSTOMERS = 100`
* `MAX_DRINKS = 20`
* `MAX_ORDERS = 50`
* `MAX_NAME = 50`
* `MAX_PHONE = 15`

#### 主要结构体

##### `Drink`

表示饮品信息：

* `id`
* `name`
* `basePrice`
* `available`

##### `DrinkOptions`

表示饮品定制选项：

* `beanType`
* `milkType`
* `isHot`
* `extraShot`
* `sugars`

##### `Order`

表示单笔订单：

* `orderId`
* `drinkId`
* `options`
* `totalCost`
* `orderTime`

##### `Customer`

表示顾客信息：

* `accountNumber`
* `name`
* `phone`
* `balance`
* `orderHistory`
* `orderCount`
* `active`

---

## 角色与功能

## 1. 经理端功能

经理登录后可以进入管理菜单，主要功能包括：

### 饮品管理

* Add Drink
* Edit Drink
* Delete Drink
* View All Drinks
* Set Option Prices

支持对饮品进行新增、编辑、停用和查看。

#### 饮品删除机制

删除饮品并不是彻底从数组中移除，而是将其 `available` 标记为 `0`，从而在菜单中隐藏。

---

### 顾客管理

* Add Customer
* Edit Customer
* Delete Customer
* View All Customers

支持新增顾客、修改顾客信息、停用顾客账户以及查看顾客列表。

#### 顾客账号生成规则

新顾客账号自动从 `1001` 开始递增生成。

#### 顾客删除机制

删除顾客也不是物理删除，而是将其 `active` 标记为 `0`，表示账户停用。

---

### 销售统计

经理可以查看系统销售统计信息，包括：

* Total Orders
* Total Revenue
* Average Order Amount
* Drink Sales Ranking

这些统计是通过遍历所有顾客的订单历史汇总得到的。

---

### 系统设置

经理还可以修改经理密码。
修改时需要：

1. 输入当前密码
2. 输入新密码
3. 再次确认新密码

新密码要求至少 6 个字符。

---

## 2. 顾客端功能

顾客登录后可以使用以下功能：

* Place an order
* View order history
* Recharge
* Log out

---

### 顾客登录

顾客通过 **账号号** 登录，不需要单独密码。
只有处于激活状态的账户才可登录。

---

### 充值

顾客可以向账户余额充值。
系统会校验金额必须大于 0。

---

### 下单

顾客下单流程如下：

1. 查看饮品菜单
2. 输入饮品 ID
3. 选择定制选项
4. 系统计算总价
5. 检查余额是否充足
6. 用户确认下单
7. 保存订单并扣除余额

如果余额不足，系统会提示缺少的金额。

---

### 订单定制项

每杯饮品支持以下定制内容：

#### Coffee Bean Type

* Arabica
* Robusta
* Blend

#### Milk Type

* Whole Milk
* Skim Milk
* Oat Milk
* Almond Milk
* None

#### Temperature

* Hot
* Iced

#### Extra Shot

* Yes / No

#### Sugar

* 0 到 5 份

---

### 订单历史

顾客可以查看自己历史订单，系统会显示：

* 订单号
* 饮品名称
* 咖啡豆类型
* 奶类型
* 冷热
* 是否加 shot
* 糖份数
* 金额
* 下单时间

---

## 价格计算规则

订单总价由以下部分组成：

```text
总价 = 饮品基础价格 + extra shot 价格 + 糖份数 × sugarPrice
```

其中：

* `extraShotPrice` 默认值：`0.50`
* `sugarPrice` 默认值：`0.10`

经理可以在后台修改这两个价格。

---

## 默认饮品

如果系统第一次运行、尚未加载到饮品数据，会自动初始化以下默认菜单：

* Espresso — $2.50
* Latte — $3.50
* Cappuccino — $3.50
* Mochaccino — $4.00
* Americano — $2.80

---

## 数据持久化

系统使用二进制文件保存数据：

* `customers.dat`
  保存顾客信息和订单历史

* `drinks.dat`
  保存饮品列表、额外价格、糖价格、自增订单号和经理密码

程序启动时会尝试自动加载这些文件；退出或执行关键修改操作时会自动保存。

---

## 编译方法

进入最终版目录：

```bash
cd complete
```

使用 `make` 编译：

```bash
make
```

编译成功后会生成可执行文件：

```bash
coffee_shop
```

---

## 运行方法

在 `complete/` 目录下运行：

```bash
./coffee_shop
```

---

## 清理编译文件

```bash
make clean
```

---

## 编译环境

Makefile 使用：

* `gcc`（默认）
* `clang`（macOS 下自动切换）
* C99 标准
* `-Wall` 编译警告选项

---

## 代码模块说明

### `main.c`

负责：

* 全局变量定义
* 程序入口
* 主菜单显示
* 一级菜单分发

### `manager.c`

负责：

* 经理登录
* 饮品管理
* 顾客管理
* 销售统计
* 修改经理密码

### `customer.c`

负责：

* 顾客菜单
* 下单
* 充值
* 查看订单历史
* 饮品定制

### `data.c`

负责：

* 默认饮品初始化
* 数据保存
* 数据加载

### `util.c`

负责：

* 输入缓冲区清理
* 按 ID 查找饮品
* 按账号查找顾客
* 计算订单金额

### `coffee_shop.h`

负责：

* 宏定义
* 结构体定义
* 全局变量声明
* 函数声明

---

## 适合学习的内容

这个项目适合练习：

* C 语言模块化编程
* 头文件与多源文件组织
* 结构体设计
* 数组管理数据
* 命令行菜单系统
* 文件读写（二进制持久化）
* 简单业务系统建模

---

## 已知特点与局限

当前实现整体已经比较完整，但也有一些可以继续优化的地方：

* 顾客登录只靠账号号，没有顾客密码机制
* 删除采用“停用”方式，而不是彻底删除
* 数据存储使用本地二进制文件，不适合多人并发
* 饮品和顾客容量使用固定数组上限
* 销售统计逻辑较基础，没有时间维度筛选
* 界面是纯命令行，没有图形化界面

---

## 默认经理密码

```text
manager123
```

首次运行后建议在经理菜单中及时修改。

