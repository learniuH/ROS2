# xacro语法

xacro 提供了可编程接口，类似于计算机语言，包括变量声明调用、函数声明与调用等语法实现。在使用 xacro 生成 urdf 时，根标签`robot`中必须包含命名空间声明：`xmlns:xacro="http://wiki.ros.org/xacro"`。

## 1.变量与算数运算

变量用于封装 URDF 中的一些字段，比如: PAI 值，小车的尺寸，轮子半径 ....，变量的基本使用语法包括变量定义、变量调用、变量运算等。

### 1.1 变量定义

语法格式

```xml
<xacro:property name="变量名" value="变量值" />
```

### 1.2 变量调用

语法格式

```xml
${变量名}
```

示例

```xml
<geometry>
    <cylinder radius="${wheel_radius}" length="${wheel_length}" />
</geometry>
```

### 1.3变量运算

语法格式

```xml
${数学表达式}
```

示例

```xml
<origin xyz="0 0 0" rpy="${PI / 2} 0 0" />
```

## 2.宏

类似于函数实现，提高代码服用率，优化代码结构，提高安全性。宏的基本使用语法包括宏的定义与调用。

### 2.1宏定义

语法格式：

```xml
<xacro:macro name="宏名称" params="参数列表（多参数之间使用空格分隔）">

    ......

    参数调用格式：${参数名}

</xacro:macro>
```

### 2.2宏调用

语法格式：

```xml
<xacro:宏名称 参数1=xxx 参数2=xxx />
```

示例：

```xml
<xacro:wheel_func wheel_name="left_wheel">
```


#### 3.文件包含

机器人由多部件组成，不同部件可能封装为单独的 xacro 文件，最后再将不同的文件集成，组合为完整机器人，可以使用文件包含实现。

语法格式：

```xml
<xacro include filename="其他xacro文件" />
```