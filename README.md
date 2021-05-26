# SnakeGame

## Request

Make sure you are running this program under Linux environment.

## Clone

```git
$ git clone git@github.com:shaowen0909/SnakeGame.git
```

## Build

```git
$ cd SnakeGame && mkdir build && cd build && cmake .. && make
$ ./SnakeGame
```

## Some problems

定时器更新蛇体频率较慢（比如：0.3s

当按下两个方向键且间隔小于0.3s的时候，看似可以正常移动，实则不是，这是因为Input函数更新了current_direction的值，但还未来得及打印蛇的行径时射又改变了一次方向，
导致current_direction被改变了两次，而Direct current_direction;却只被改变了一次。

比如蛇向下走，此时瞬间按出左,上键，看似蛇先会往左然后再往上，实则不是：
此时direct确实被改变了两次也就是先(-1, 0), (0, -1) （0.3s内发生的这一切
0.3s后 direct = (0, -1)，此时蛇头坐标加上direct的xy，就相当与蛇头后面的那个节点的位置，
所以自然地被判为撞击了自己的身体，结束游戏。

此问题已通过增加direction_buffer解决，按下的所有方向键都会被存入direction_buffer（一个deque，可以认为是一个FIFO的queue。
即蛇只从buffer中取方向（拿过之后pop），因此无论玩家按多块，蛇都会按照顺序行走。

