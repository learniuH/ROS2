# URDF工具

在 ROS2 中，提供了一些URDF文件相关的工具，比如：

- `check_urdf` 命令可以检查复杂的 urdf 文件是否存在语法问题；

- `urdf_to_graphviz` 命令可以查看 urdf 模型结构，显示不同 link 的层级关系。

```bash
$ sudo apt install liburdfdom-tools
```

## 1.check 语法检查
```bash
$ check_urdf demo05_exercise.urdf

robot name is: Lbot
---------- Successfully Parsed XML ---------------
root Link: base_footprint has 1 child(ren)
    child(1):  base_link
        child(1):  left_back_wheel
        child(2):  left_front_wheel
        child(3):  right_back_wheel
        child(4):  right_front_wheel
```

## 2.urdf_to_graphviz 结构查看
```base
$ urdf_to_graphviz demo05_exercise.urdf
```

当前目录下，将生成urdf中robot名称命令的.pdf和.gv文件，打开pdf文件会以树形结构显示link与joint的关系。

**注意：**该工具以前名为`urdf_to_graphiz`现建议使用`urdf_to_graphviz`替代