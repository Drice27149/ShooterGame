// Drice

#include "ShooterGame.h"
#include "Weapon/ShooterProjectile.h"
#include "Player/ShooterCharacter.h"
#include "Weapon/WeaponGun.h"

// Sets default values
AWeaponGun::AWeaponGun()
{
    CenterComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
    RootComponent = CenterComp;
    
    FirePointComp = CreateDefaultSubobject<USceneComponent>(TEXT("FirePointComp"));
    FirePointComp->SetupAttachment(RootComponent);
    
    bReplicates = true;
}

bool AWeaponGun::CanFire()
{
    if(BulletCount>0)
    {
        return true;
    }
    else{
        return false;
    }
}

void AWeaponGun::HandleFire()
{
    BulletCount--;
    if(ProjectileClass != NULL)
    {
        FRotator FireRotation = FirePointComp->GetComponentRotation();
        FVector FireLocation = FirePointComp->GetComponentLocation();
        FTransform SpawnTransform(FireRotation, FireLocation);
        AShooterProjectile* SpawnedProjectile = Cast<AShooterProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform));
        if(SpawnedProjectile != NULL)
        {
            SpawnedProjectile->OwnerPawn = OwnerPawn;
            UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
        }
    }
}

void AWeaponGun::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AWeaponGun, BulletCount);
}
