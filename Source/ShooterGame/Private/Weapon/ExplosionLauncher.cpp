// Drice


#include "Weapon/ExplosionLauncher.h"
#include "ShooterGame.h"
#include "Weapon/ExplosionProjectile.h"
#include "Player/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "UI/ShooterHUD.h"

AExplosionLauncher::AExplosionLauncher()
{
    FirePointComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp"));
    FirePointComp->SetupAttachment(RootComponent);
    
    bReplicates = true;
    SetReplicateMovement(true);
}

void AExplosionLauncher::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // local already simulated fire, so no need to replicated
    DOREPLIFETIME_CONDITION(AExplosionLauncher, BurstCounter, COND_SkipOwner);
}

void AExplosionLauncher::StartFire()
{
    FRotator FireRotation = FirePointComp->GetComponentRotation();
    FVector FireLocation = FirePointComp->GetComponentLocation();
    FTransform FireTransform(FireRotation, FireLocation);
    FVector FireVector = FirePointComp->GetForwardVector();
    
    // server doesn't always refesh bone, so fire transform need to be transfer to server
    ServerFireLauncher(FireTransform, FireVector);
    
    // simulate on local
    SimulateLauncherFire();
}

void AExplosionLauncher::ServerFireLauncher_Implementation(FTransform FireTransform, FVector FireVector)
{
    AmmoCount--;
    // call simulate funtion on remote client
    BurstCounter++;
    // fix listen server issue
    OnRep_BurstCounter();
    OnRep_AmmoCount();
    
    // spawn projectile on server, which will be repliated to client
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
            // add impulse to make it move
            FVector FireImpulse = FireVector * FireImpulseScale;
            USphereComponent* ExplosionPhysicComp = Cast<USphereComponent>(SpawnedProjectile->GetRootComponent());
            if(ExplosionPhysicComp)
            {
                ExplosionPhysicComp->AddImpulse(FireImpulse);
            }
        }
    }
}

void AExplosionLauncher::OnRep_BurstCounter()
{
    SimulateLauncherFire();
}

void AExplosionLauncher::SimulateLauncherFire()
{
    if(OwnerCharacter)
    {
        OwnerCharacter->PlayCharacterMontage(FireMontage_Character);
        PlayWeaponMontage(FireMontage_Weapon);
        
        if(OwnerCharacter->IsAiming())
        {
            // simulate camera shake on local player
            AShooterPlayerController* MyShooterPC = OwnerCharacter?Cast<AShooterPlayerController>(OwnerCharacter->GetController()):NULL;
            if(MyShooterPC && MyShooterPC->IsLocalController())
            {
                if(FireCameraShake)
                {
                    MyShooterPC->ClientPlayCameraShake(FireCameraShake, 1.0f);
                }
            }
        }
    }
}

bool AExplosionLauncher::CanFire()
{
    return AmmoCount > 0;
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
