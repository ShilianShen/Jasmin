# Menu

## Intro

为Jasmin的子模块

## 功能

* 静态图片/文本展示
* 可选, 频繁从toml文件重新渲染
* 按钮事件
* 界面跳转

非频繁重载: $init \rightarrow load \rightarrow (renew \rightarrow draw)_n \rightarrow kill$.

频繁重载: $init \rightarrow load \rightarrow (renew \rightarrow draw\rightarrow kill \rightarrow load)_n \rightarrow kill$.

## 结构

$$
\text{basic}\to
[
\text{trig.h}
\to\text{elem}
\to\text{page}
\to\text{menu}
]_{\text{struct}}

\to
\left[\begin{matrix}
\text{draw}\\
\text{init}\\
\text{kill}\\
\text{load}\\
\text{renew}\\
\end{matrix}\right]_{\text{func}}
\to\text{trig.c}, \text{menu}
$$
