**记录遇到的问题和解决方法**

#### 2020.11.7~

虽然主题是骨骼动画, 但这次遇到的问题大多出现在网络同步方面...

* 问题:

  把武器attach到角色的mesh上之后, 进行俯仰时Server上的武器方向与本地的不一致, 导致了武器开火方向和瞄准方向不同的问题。

  解决方法:

  这个问题只会在dedicated server上出现, listen server不会。因此确定Pitch是已经正确同步的, 在[AnswerHub](https://answers.unrealengine.com/questions/49935/does-animmontage-play-on-a-dedicated-server-for-th.html)上找到的解释是dedicated server上默认是不在每个tick更新骨骼的, 进行俯仰时server上的人物骨骼没有更新, 附着在上面的武器方向自然也没有更新。需要把"Mesh Component Update Flag"选项设置为"Always Tick Pose and Refresh Bones", 改了之后问题解决。

* 问题:

  在武器的类方法中调用server rpc无效。

  解决方法:

  查看文档后得知是ownership的问题。要在客户端的actor中调用server 的rpc函数, 必须满足的条件是这个actor在网络上的owner是客户端自己, 否则调用的请求会被忽略。在创建武器之后调用```AActor::setOwner```把owner设置为任意一个本客户端拥有的actor即可, 比如玩家的character或者playercontroller。

#### 2020.11.23~

todo:

* change projectile collision type from block to overlap [gg]
* implement play hit system, character should react differently for different attack(location, rotation) [done]
* impulse projectile begin, derive or not? [not yet]
* problem about floating in the air: [gg]
  * when montage is playing, character float in the air
  * current solution is create a new slot in which foot don't play montage
* Think twice, code once.
* The arrive order of replicated variable is not fixed, and can't be relied on.

#### 2020.11.24~

todo:

* grende, fire radial impulse when explode.

log:

* [Simulate physic is not necessary for recieving radial force, the key is object type](https://answers.unrealengine.com/questions/517575/bug-radial-force-does-not-affect-actors-who-have-s.html)

* collision overlap detect + play hit seems like a goodone

  ```c++
  void AShooterCharacter::PlayHit(AActor* HitActor, EHitType HitType, float HitDamage, FVector HitForwardVector, FVector HitImpulse = FVector(0,0,0))
  ```

  Generally, three type of hit is applied:

  hit, heavy hit, block

  in each kind of hit, three things need to be determined: damage, impulse, animation

* overide take damage is not a good choice since this function's parameter is bad

#### 2020.11.25~

todo: 

* Implement a physic, hit with explosion and impulse projectile
  * use collision overlap to detect damaged actor
  * treat character and other actor differently
    * for character, input the impulse done
    * for give me the blueprint event and done

#### 2020.11.29~
* todo: 
  * health & death & respawn system
    * use ragdoll to implement death
    * physic animation(?) on projectile hit, seems like a good idea, will look into physic asset for more detail
  * will finish health & respawn first
  * then consider hit reaction & ragdoll
#### 2020.11.30~
* todo(priority from high to low): 
  * health &  death & ragdoll
    * health & ragdoll & death & respawn
      ```c++
      //Shooter
      
      UPROPERTY(Replicated)
      float Health
      
      void PlayHit(xxx, xxx){
          Health -= HitDamage;
      }
      
      void OnRep_lastHit(){
          if(Health>0){
           	SimulateHit();   
          }
          else{
           	SimulateDeath();
          }
      }
      
      //call back when LastHitInfo changed and not died
      void SimulateHit()
          
      //call back when LastHitInfo changed and died
      void SimulateDeath(){
          //ragdoll
          GetCharacterMovement->disable/distroy
         	GetMesh()->SetCollision(NoCollision)
          GetMesh()->SimulatePhysic(true)
          //death & respawn
      	Setlifespan(5.0f) // can be determined
         	PC->ReadyToRespawn(5.0f + 10.0f) // set timer and call ServerRestartPlayer() 
         	DetachFromController()
      }
      ```

    issues about above code:

    * collision is not disable when ragdoll
    * ragdoll is not replicated

  * physic hit reaction

  * time line based turn in place 

  * improve animation bp

* For spawning/destroying, you can read the [document](https://docs.unrealengine.com/en-US/Gameplay/HowTo/SpawnAndDestroyActors/index.html)

* notice: constructor -> editdefaultsonly -> beginplay

  * todo: figure out the order in detail

#### 2020.12.1~

* todo:(priority from high to low)

  * physic hit reaction
* timeline based turn in place
  
  * armless: fireball
  
  * gun: fireball + starter pack
  
  * sword: grey
    * boss choice: feng mao 
    * sword is abandoned for a while, currently focus on make tps good
  * fix bug about ragdoll collsion
* improve animation asset, use paragon
  * implement sword damage system
* improve level design
  * HUD, UI display (health, player name, kill notify...)
* package it with dedicated server
  
* This week's goal

  * main menu
  * animation that look good(turn in place, gun + armless)
  * HUD that look good(other's health & name display on top)
  * level that look good(basic room/landscape)
  * step by step

#### 2020.12.2

* physic hit reaction
  * timeline & curve in c++ is needed
  * fix bug about turn in place and ragdoll
* todo:(priority from high to low)
  * physic hit reaction
  * rebuild animation blueprint(change animation asset, fix foot IK)
  * head up display, health bar, player name display in game
  * main menu(necessary), multiple level(optional)
* today's goal
  * physic hit reaction(forgive me to use blueprint)
    * bug: at the beginning, rotation yaw != actor yaw 
    * workaround: if busy, then unable player input
  * rebuild animation blueprint