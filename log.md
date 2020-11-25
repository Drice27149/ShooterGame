####2020.11.23~

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

