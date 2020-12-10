// Drice

#include "Weapon/WeaponGun.h"
#include "ShooterGame.h"
#include "ShooterGameType.h"
#include "Weapon/ShooterProjectile.h"
#include "Player/ShooterCharacter.h"
#include "Player/ShooterPlayerController.h"


// Sets default values
AWeaponGun::AWeaponGun()
{
    FirePointComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp"));
    FirePointComp->SetupAttachment(RootComponent);
    
    bReplicates = true;
    SetReplicateMovement(true);
    
    BulletCount = MaxBulletCount;
    BurstCounter = 0;
} 

bool AWeaponGun::CanFire()
{
    return AmmoCount > 0;
}

int AWeaponGun::GetWeaponTypeId()
{
    return 1;
}

void AWeaponGun::StartFire()
{
    FVector StartTrace = FirePointComp->GetComponentLocation();
    FVector EndTrace = FirePointComp->GetForwardVector()*FireRange + StartTrace;
    FHitResult Hit;
    FCollisionQueryParams CollisionParams;
    
    if(GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_WorldDynamic, CollisionParams)){
        ServerNotifyHit(Hit);
        
        SimulateWeaponHit(Hit.Location);
    }
    
    ServerFireWeapon();
    SimulateFireWeapon();
}

void AWeaponGun::ServerFireWeapon_Implementation()
{
    AmmoCount--;
    //call simulate function on remote client
    BurstCounter++;
    
    // fix listen server issue
    OnRep_BurstCounter();
}

void AWeaponGun::SimulateFireWeapon()
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


void AWeaponGun::OnRep_BurstCounter()
{
    SimulateFireWeapon();
}

void AWeaponGun::ServerNotifyHit_Implementation(FHitResult HitResult)
{
    AShooterCharacter* HitCharacter = Cast<AShooterCharacter>(HitResult.GetActor());
    if(HitCharacter){
        HitCharacter->PlayHit(OwnerCharacter, EHitType::NormalHit, WeaponGunDamage, 
        FirePointComp->GetForwardVector(), FirePointComp->GetForwardVector(), HitResult.BoneName);
    }
    
    // update HitNotify for remote client to simulate
    FHitNotify NewNotify;
    NewNotify.HitLocation = HitResult.Location;
    NewNotify.HitCounter = HitNotify.HitCounter + 1;
    HitNotify = NewNotify;
    
    // fix listen server issue
    OnRep_HitNotify();
}

void AWeaponGun::OnRep_HitNotify()
{
    SimulateWeaponHit(HitNotify.HitLocation);
}

void AWeaponGun::SimulateWeaponHit(FVector HitLocation)
{
    if(HitEffect != NULL)
    {
        UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, HitLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
    }
}

void AWeaponGun::HandleFiring(bool bfromReplication)
{
    //server
    if(GetLocalRole() == ROLE_Authority && !bfromReplication)
    {
        //spawn and fire projectile
        BulletCount--;
        if(ProjectileClass != NULL)
        {
            FRotator FireRotation = FirePointComp->GetComponentRotation();
            FVector FireLocation = FirePointComp->GetComponentLocation();
            FTransform SpawnTransform(FireRotation, FireLocation);
            AShooterProjectile* SpawnedProjectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
            if(SpawnedProjectile != NULL)
            {
                SpawnedProjectile->OwnerCharacter = OwnerCharacter;
                SpawnedProjectile->SetInstigator(OwnerCharacter);
                UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
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


void AWeaponGun::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // only owner need to know bullet count
    DOREPLIFETIME_CONDITION(AWeaponGun, BulletCount, COND_OwnerOnly);
    
    // skip owner because weapon fire was already simulated on owner
    DOREPLIFETIME_CONDITION(AWeaponGun, BurstCounter, COND_SkipOwner);
    DOREPLIFETIME_CONDITION(AWeaponGun, HitNotify, COND_SkipOwner);
}

void AWeaponGun::StartReload()
{
    ServerStartReload();
}

void AWeaponGun::HandleReload(bool bfromReplication)
{
    //server
    if(GetLocalRole() == ROLE_Authority && !bfromReplication)
    {
        BulletCount = MaxBulletCount;
    }
    //client
    if(GetLocalRole() != ROLE_Authority)
    {
        if(OwnerCharacter)
        {
            OwnerCharacter->PlayCharacterMontage(PawnReloadAnimation);
            PlayWeaponMontage(WeaponReloadAnimation);
        } 
    }
}

void AWeaponGun::ServerStartReload_Implementation()
{
    HandleReload(false);
    MulticastStartReload();
}

void AWeaponGun::MulticastStartReload_Implementation()
{
    HandleReload(true);
}
