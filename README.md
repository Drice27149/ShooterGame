### 10.31~11.6

* 本周内容

  增加角色信息的显示, 得分的显示。

  增加基础操作的按钮。

* 演示视频

  [链接](https://pan.baidu.com/s/1ahcAQAQgudyXFP-kaagy7Q)，提取码: xd8w

* 遇到的问题

  * 由于之前写的太差, 把项目重新写了一遍, 浪费了很多时间。	
  * 枪支和人物的射击动画还没有实现, 目前看起来不像个游戏。

### 11.7~11.20

* **演示视频**: [todo], **打包的安卓文件**: [todo]。

* **本次内容**:

  *  实现角色基础的移动功能, 包括站立, 跑, 跳动作的表现。
    * 这些实际上都是角色动画蓝图中动画状态机的内容。在状态机中分别定义好站立(Idle), 走动(Jog), 跑动(Run), 跳(Jump)几个状态, 再添上状态之间的转移条件(一般由角色的自身数据驱动)就可以了。
    * 在每个状态之中可以根据需求选择是重复播放动画或者是使用由参数驱动的Blend Space。比如说, 如果走动和跑动是分开两个状态, 那么各自的状态播放相应的动画即可; 如果是结合到一个状态的话也可以使用由速度驱动的Blend Space。 
  *  实现角色的俯仰表现。
    * 参考官方文档中[瞄准偏移(Aim Offset)](https://docs.unrealengine.com/en-US/Engine/Animation/AnimHowTo/AimOffset/index.html)的内容即可。其大概的原理是把角色在指向不同方向上的动画混合到一个Blend Space中, 根据外部参数(Yaw 和 Pitch)驱动, 并叠加角色的上半身上。官方教程中创建的Aim Offset是2D的, 要实现俯仰只需1D的就足够了。
  *  实现武器开火的表现, 要求开火过程中俯仰仍然可以保持正确。
    * 武器开火使用播放Montage进行表现, 使用layer blend per bone可以让Montage效果只有上半身, 这样就可以在移动中开火了。但单纯的使用上半身的Montage还不足够, 因为它会把Aim Offset的效果覆盖掉, 导致开火过程中武器偏离瞄准方向的问题。
    * 解决的方法是把动画蓝图中, 把Aim Offset结点放在动画播放的Slot之后, 这样Aim Offset既不会被覆盖, 而且动画也得到了Aim Offset的叠加效果, 开火过程中武器能够保持瞄准的方向。
  *  实现一种动作表现, 类似近战或丢手雷, 要求在角色站立不动时是一个全身表现, 在角色移动时是上半身的表现, 下半身保持原来的移动状态。
    * 本人实现的是持剑的三连击效果。动画表现原理和武器开火基本一致, 都是Montage_play + layer blend per bone。
    * 稍有不同的是对站立不动和移动的情况要分开处理, 可以定义分别定义使用"FullBody"和"UpperBody" slot的两种Montage, 在触发攻击的事件中判断角色是否移动中并选择播放相应的动画; 也可以统一播放"FullBody"的动画, 然后在动画蓝图中使用layer blend per bone + Blend by bool/Blend by int进行选择(需要从角色蓝图中得知角色是否在移动)。
  *  实现自由视角的功能。
    * 角色的方向是否与玩家控制器方向一致与 ```APawn::bUseControllerRotationYaw```有关, 若为false则不一致, 若为true则角色朝向始终与玩家控制器方向保持一致, 这个属性也可以在角色蓝图中通过use controller rotation yaw这个选项进行勾选。
    * 要实现按下alt键时的自由视角, 其实就是按下alt键后角色方向不再与玩家控制器方向保持一致, 即```APawn::bUseControllerRotationYaw = false```。 因此可以增加按下和松开alt键后的响应事件, 按下时设置其为false, 松开后设置其为true。蓝图或c++都可以, 本人的实现方法是在c++代码中增加了处理的逻辑。
    * 关于角度变化时角色头部对应移动的问题, 原理和之前的俯仰一样, 都是使用Aim Offset。 不同的地方是只有头部在动, 那么使用layer blend per bone把只把颈部以上的动画进行叠加即可。
  * 为了后续多人游戏的开发, 对上述的表现都实现了网络同步。
    * 同步的原理也并不复杂, 对于动画状态机, 只要把驱动状态机的变量(Speed, Direction, crouch, jump...)等同步了, 动画状态机的表现就自动同步了; 对于俯仰, 可以参考[官方工程ShooterGame](https://docs.unrealengine.com/en-US/Resources/SampleGames/ShooterGame/index.html)代码中的实现: 调用```APawn::GetBaseAimRotation()```可以得到一个内置同步的Pitch; 对于Montage的播放, 一个可行的方案是使用RPC, 即客户端调用Server方法, 服务器上再调用Netmulticast方法实现每个客户端同步播放。

* **遇到的问题和解决方法**

  虽然主题是骨骼动画, 但这次遇到的问题大多出现在网络同步方面...

  * 问题:

    把武器attach到角色的mesh上之后, 进行俯仰时Server上的武器方向与本地的不一致, 导致了武器开火方向和瞄准方向不同的问题。

    解决方法:

    这个问题只会在dedicated server上出现, listen server不会。因此确定Pitch是已经正确同步的, 在[AnswerHub](https://answers.unrealengine.com/questions/49935/does-animmontage-play-on-a-dedicated-server-for-th.html)上找到的解释是dedicated server上默认是不在每个tick更新骨骼的, 进行俯仰时server上的人物骨骼没有更新, 附着在上面的武器方向自然也没有更新。需要把"Mesh Component Update Flag"选项设置为"Always Tick Pose and Refresh Bones", 改了之后问题解决。

  * 问题:

    在武器的类方法中调用server rpc无效。

    解决方法:

    查看文档后得知是ownership的问题。要在客户端的actor中调用server 的rpc函数, 必须满足的条件是这个actor在网络上的owner是客户端自己, 否则调用的请求会被忽略。在创建武器之后调用```AActor::setOwner```把owner设置为任意一个本客户端拥有的actor即可, 比如玩家的character或者playercontroller。