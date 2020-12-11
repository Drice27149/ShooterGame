**最新一次的作业在最下面**

## 10.31~11.6

* **演示视频**: [链接](https://pan.baidu.com/s/1ahcAQAQgudyXFP-kaagy7Q)，提取码: xd8w

* **本周内容**

  增加角色信息的显示, 得分的显示。

  增加基础操作的按钮。

## 11.7~11.20

* **演示视频**: [网盘链接](https://pan.baidu.com/s/13NPiuaEbLNYH2Ojg4WWK0A), 提取码: ncph 。**打包的安卓文件**: [网盘链接](https://pan.baidu.com/s/1iw7RXK0nRV886pYAw-zmHw) 提取码: dmgy。

* **本次内容**:
  
  *  #### 实现角色基础的移动功能, 包括站立, 跑, 跳动作的表现。
     * 这些实际上都是角色动画蓝图中动画状态机的内容。在状态机中分别定义好站立(Idle), 走动(Jog), 跑动(Run), 跳(Jump)几个状态, 再添上状态之间的转移条件(一般由角色的自身数据驱动)就可以了。
     * 在每个状态之中可以根据需求选择是重复播放动画或者是使用由参数驱动的Blend Space。比如说, 如果走动和跑动是分开两个状态, 那么各自的状态播放相应的动画即可; 如果是结合到一个状态的话也可以使用由速度驱动的Blend Space。 
     
  *  #### 实现角色的俯仰表现。
     * 参考官方文档中[瞄准偏移(Aim Offset)](https://docs.unrealengine.com/en-US/Engine/Animation/AnimHowTo/AimOffset/index.html)的内容即可。其大概的原理是把角色在指向不同方向上的动画混合到一个Blend Space中, 根据外部参数(Yaw 和 Pitch)驱动, 并叠加角色的上半身上。官方教程中创建的Aim Offset是2D的, 要实现俯仰只需1D的就足够了。
     
  *  #### 实现武器开火的表现, 要求开火过程中俯仰仍然可以保持正确。
     * 武器开火使用播放Montage进行表现, 使用layer blend per bone可以让Montage效果只有上半身, 这样就可以在移动中开火了。但单纯的使用上半身的Montage还不足够, 因为它会把Aim Offset的效果覆盖掉, 导致开火过程中武器偏离瞄准方向的问题。
     * 解决的方法是把动画蓝图中, 把Aim Offset结点放在动画播放的Slot之后, 这样Aim Offset既不会被覆盖, 而且动画也得到了Aim Offset的叠加效果, 开火过程中武器能够保持瞄准的方向。
     
  *  #### 实现一种动作表现, 类似近战或丢手雷, 要求在角色站立不动时是一个全身表现, 在角色移动时是上半身的表现, 下半身保持原来的移动状态。
     * 本人实现的是持剑的三连击效果。动画表现原理和武器开火基本一致, 都是Montage_play + layer blend per bone。
     * 稍有不同的是对站立不动和移动的情况要分开处理, 可以定义分别定义使用"FullBody"和"UpperBody" slot的两种Montage, 在触发攻击的事件中判断角色是否移动中并选择播放相应的动画; 也可以统一播放"FullBody"的动画, 然后在动画蓝图中使用layer blend per bone + Blend by bool/Blend by int进行选择(需要从角色蓝图中得知角色是否在移动)。
     
  *  #### 实现自由视角的功能。
     * 角色的方向是否与玩家控制器方向一致与 ```APawn::bUseControllerRotationYaw```有关, 若为false则不一致, 若为true则角色朝向始终与玩家控制器方向保持一致, 这个属性也可以在角色蓝图中通过use controller rotation yaw这个选项进行勾选。
     * 要实现按下alt键时的自由视角, 其实就是按下alt键后角色方向不再与玩家控制器方向保持一致, 即```APawn::bUseControllerRotationYaw = false```。 因此可以增加按下和松开alt键后的响应事件, 按下时设置其为false, 松开后设置其为true。蓝图或c++都可以, 本人的实现方法是在c++代码中增加了处理的逻辑。
     * 关于角度变化时角色头部对应移动的问题, 原理和之前的俯仰一样, 都是使用Aim Offset。 不同的地方是只有头部在动, 那么使用layer blend per bone把只把颈部以上的动画进行叠加即可。
     
  * #### 为了后续多人游戏的开发, 对上述的表现都实现了网络同步。
  
     * 同步的原理也并不复杂, 对于动画状态机, 只要把驱动状态机的变量(Speed, Direction, crouch, jump...)等同步了, 动画状态机的表现就自动同步了; 对于俯仰, 可以参考[官方工程ShooterGame](https://docs.unrealengine.com/en-US/Resources/SampleGames/ShooterGame/index.html)代码中的实现: 调用```APawn::GetBaseAimRotation()```可以得到一个内置同步的Pitch; 对于Montage的播放, 一个可行的方案是使用RPC, 即客户端调用Server方法, 服务器上再调用Netmulticast方法实现每个客户端同步播放。
  



## 11.21~11.27

* **演示视频**: [网盘链接](https://pan.baidu.com/s/1IoK7074rncVgkT_575BjDQ),  提取码: mfxj. **打包的安卓文件**: [链接](https://pan.baidu.com/s/1_SZWpLjT6kAt_l4vs-e_sg) 提取码: wfgb

* **本次内容**: 

  * **实现枪械的pick功能**。

    * 我实现的效果为靠近场景中的武器之后会弹出pick up窗口, 点击pick up可以把武器装备, 点击drop可以把当前装备的武器卸下。
    * 检测附近武器的原理是给武器注册```OnComponentBeginOverlap```和```OnComponentEndOverlap```事件,。
    * 捡起和卸下武器的原理是先在server端修改角色的武器数据, 然后在客户端把武器```attach```或者```detach```到角色的socket上面。

  * **射线枪械: 无物理弹道, 直线命中, 子弹打中角色不同部位有不同的命中反馈动画和音效**。

    * 使用```LineTraceSingleByChannel```, 起点为枪口, 终点为武器枪口延伸一段距离后得到的点。如果射线扫描命中目标, 则把击中结果```FHitResult```发送到服务器, 进行下一步操作。
    * 在服务器上判断```FhitResult```中击中的```Actor```是不是玩家角色, 如果是, 则通过```HitResult.BoneName```获得击中的骨骼名字, 根据骨骼名字播放不同的命中反馈动画和音效。
  
  * **抛体枪械: 抛体命中角色立刻爆炸, 命中场景会反弹减速, 减速到一定程度后爆炸。根据不同的物理材质而改变反弹的速度反馈。爆炸效果为把一定范围内的物件和角色炸飞**。 
  
    * 子弹开启```simulate physic```和```enable gravity```, 发射时通过```AddImpulse```到子弹的```RootComponent```获得速度, 子弹就可以进行抛体运动了。
    
    * 击中的碰撞检测注册```OnComponentHit```事件的回调函数, 在函数中判断击中的actor是否角色类型, 如果是则爆炸。
    
    * 不同的速度反馈可以通过新建```physic material```类并应用到物体上, 修改```physic material```的```Resititution```来改变速度反馈, 属性值越大速度的衰减越少。
    
    * 速度衰减的检测在子弹的每一个```Tick```中进行, 如果```GetVelocity().Size()```的结果过小则爆炸。
    
    * 炸飞效果实现方式有很多, 我使用的是给子弹增加一个```RadialForceComponent```组件, 在爆炸的时候调用```FireImulse```方法给爆炸的球型区域中的物体施加冲量, 达到炸飞的效果。 
    
      
    

## 11.28~12.11

* **演示视频**: [网盘链接](https://pan.baidu.com/s/169NMPem55QE6lg7mVVvPYQ), 提取码: 977k  **打包的安卓文件**: [上传中]

* **本次内容**:  多玩家对战

  *  一个玩家作为主机, 其他玩家加入游戏
     * 直接的方法是主机玩家用 ```?listen```命令行参数启动游戏, 其他玩家启动游戏时输入主机listen的ip地址加入游戏
     * 也可使使用```Create/Join/Destroy Session```的方法, 好处是不用输入命令行参数。
     * 由于之前都是按照```Dedicated Server```来设计游戏的同步, 所以在```listen server```模式下一些代码需要改动, 主要涉及使用```RepNotify```触发的同步表现(开火, 装弹), 因为```listen server```控制的玩家```RepNotify```不会被自动调用。
  *  玩家可以自由交战

     * 由于之前的内容都是按照多人游戏的模式写的, 因此实现自由交战比较容易。简而言之就是服务器处理交战的逻辑, 客户端同步交战的表现。
  *  死亡时有死亡表现, 死亡之后可以重生

     * 死亡表现使用了布娃娃效果, 实现方法是恰当设置mesh的碰撞参数并开启物理模拟。
     * 重生参考了官方ShooterGame工程, 在```PlayerController```中调用```ServerRestartPlayer()```

  *  游戏结束后进入结算界面, 包括击杀数, 死亡数

     * 击杀数和死亡数用同步变量记录在```PlayerState```中。

     * 同样参考了官方ShooterGame工程, 在击杀事件发生时, 把击杀者和被击杀者上传到```GameMode```类中, 由```GameMode```负责广播击杀事件并修改```PlayerState```中记录的击杀数和死亡数。
     * 游戏的时间在```GameMode```类中用一个计时器控制, 当游戏结束时广播通知所有客户端。客户端收到通知后, 显示游戏结束并从```GameState```中读取所有玩家的```PlayerState```, 再对击杀数和死亡数进行显示。

