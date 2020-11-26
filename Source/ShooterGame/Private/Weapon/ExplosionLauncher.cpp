// Drice


#include "Weapon/ExplosionLauncher.h"
#include "ShooterGame.h"
#include "Weapon/ExplosionProjectile.h"
#include "Player/ShooterCharacter.h"

AExplosionLauncher::AExplosionLauncher()
{
    FirePointComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp"));
    FirePointComp->SetupAttachment(RootComponent);
    
    bReplicates = true;
    SetReplicateMovement(true);
}

bool AExplosionLauncher::CanFire()
{
    return true;
}

//todo: change it to EWeaponType: int8
int AExplosionLauncher::GetWeaponTypeId()
{
    return 3;
}
    
void AExplosionLauncher::HandleFiring(bool bfromReplication)
{
    //server
    if(GetLocalRole() == ROLE_Authority && !bfromReplication)
    {
        //spawn and fire projectile
        if(ProjectileClass != NULL)
        {
            FRotator FireRotation = FirePointComp->GetComponentRotation();
            FVector FireLocation = FirePointComp->GetComponentLocation();
            FTransform SpawnTransform(FireRotation, FireLocation);
            AExplosionProjectile* SpawnedProjectile = Cast<AExplosionProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
            if(SpawnedProjectile != NULL)
            {
                SpawnedProjectile->SetInstigator(OwnerCharacter);
                UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
                //add impulse to physic projectile
                FVector FireImpulse = (FireRotation.Vector()) * FireImpulseScale;
                USphereComponent* ExplosionPhysicComp = Cast<USphereComponent>(SpawnedProjectile->GetRootComponent());
                if(ExplosionPhysicComp)
                {
                    ExplosionPhysicComp->AddImpulse(FireImpulse);
                }
            }
        }
    }
    //client
    if(GetLocalRole() != ROLE_Authority)
    {
        if(OwnerCharacter){
            OwnerCharacter->PlayCharacterMontage(FireMontage_Character);
            PlayWeaponMontage(FireMontage_Weapon);
        }
    }
}
