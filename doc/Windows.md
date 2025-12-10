# Windows

> `git clone 仓库地址`这一步如果慢可以下载[GitHub Desktop](https://github.com/apps/desktop), 在那里面clone, 速度会快很多.

## vcpkg

在你准备好的一个给vcpkg专用的空目录下

```PowerShell
git clone https://github.com/microsoft/vcpkg
```

```PowerShell
.\vcpkg\bootstrap-vcpkg.bat
```

然后在环境变量中的Path添加vcpkg路径的路径

## Visual Studio Build Tools

[ClickHereToDownload](https://aka.ms/vs/stable/vs_BuildTools.exe)

然后在工作负荷里面下载C++开发模块.

## SDL3

### Install

```PowerShell
vcpkg install sdl3 sdl3-image sdl3-ttf
```

### Upgrade

```PowerShell
vcpkg upgrade sdl3 sdl3-image sdl3-ttf
```

### Remove

```PowerShell
vcpkg remove sdl3 sdl3-image sdl3-ttf
```
