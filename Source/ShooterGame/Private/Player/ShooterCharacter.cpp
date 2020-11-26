// Drice

#include "Player/ShooterCharacter.h"
#include "ShooterGame.h"
#include "Weapon/Weapon.h"
#include "Weapon/WeaponGun.h"
#include "UI/ShooterHUD.h"
#include "Player/MyCharacterMovementComponent.h"
#include "Player/ShooterPlayerController.h"

AShooterCharacter::AShooterCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
    
    // Set up Default Weapon
    CurrentWeapon = NULL;
}

bool AShooterCharacter::IsRunning()
{
    return bRunning;
}

float AShooterCharacter::GetTurnDirection()
{
    return TurnDirection;
}
    
bool AShooterCharacter::IsViewMode()
{
    return !bUseControllerRotationYaw;
}

FRotator AShooterCharacter::GetAimOffset()
{
    const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

float AShooterCharacter::GetWalkSpeedMultiplier()
{
    return WalkSpeedMultiplier;
}
    
float AShooterCharacter::GetRunSpeedMultiplier()
{
    return RunSpeedMultiplier;
}
    
void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterCharacter::OnJumpStart);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::OnMoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::OnTurn);
    PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::OnLookUp);
    
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacter::OnStartFire);
}

void AShooterCharacter::OnMoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::OnMoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::OnStartFire()
{
    if(CurrentWeapon != NULL && CurrentWeapon->CanFire())
    {
        CurrentWeapon->StartFire();
    }
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AShooterCharacter, CurrentWeapon);
    DOREPLIFETIME(AShooterCharacter, bRunning);
    DOREPLIFETIME(AShooterCharacter, TurnDirection);
    DOREPLIFETIME(AShooterCharacter, LastHitInfo);
    DOREPLIFETIME_CONDITION(AShooterCharacter, PickUpWeapon, COND_OwnerOnly)
}

void AShooterCharacter::OnCrouchStart()
{
    Crouch();
}
    
void AShooterCharacter::OnCrouchEnd()
{
    UnCrouch();
}
    
void AShooterCharacter::OnJumpStart()
{
    Jump();
}

void AShooterCharacter::OnRunStart()
{
    bRunning = true;
    ServerSetRunning(true);
}
    
void AShooterCharacter::OnRunEnd()
{
    bRunning = false;
    ServerSetRunning(false);
}
    
void AShooterCharacter::OnViewModeStart()
{
    bUseControllerRotationYaw = false;
    ServerSetViewMode(true);
}

void AShooterCharacter::OnViewModeEnd()
{
    bUseControllerRotationYaw = true;
    ServerSetViewMode(false);
}

void AShooterCharacter::OnTurn(float Value)
{
    if ( (Controller != NULL) && (Value != 0.0f) )
    {
        AddControllerYawInput(Value);
    }
    ServerSetTurnDirection(Value);
}
    
void AShooterCharacter::OnLookUp(float Value)
{
    if ( (Controller != NULL) && (Value != 0.0f) )
    {
        AddControllerPitchInput(Value);
    }
}

void AShooterCharacter::OnDrop()
{
    ServerDropCurrentWeapon();
}
    
void AShooterCharacter::ServerSetRunning_Implementation(bool Value)
{
    bRunning = Value;
}
    
void AShooterCharacter::ServerSetViewMode_Implementation(bool Value)
{
    bUseControllerRotationYaw = !Value;
    MulticastSetViewMode(Value);
}

void AShooterCharacter::ServerSetTurnDirection_Implementation(float Value)
{
    TurnDirection = Value;
}

void AShooterCharacter::MulticastSetViewMode_Implementation(bool Value)
{
    bUseControllerRotationYaw = !Value;
}

float AShooterCharacter::PlayCharacterMontage(UAnimMontage* AnimMontage, float InPlayRate)
{
	if (AnimMontage && GetMesh() && GetMesh()->AnimScriptInstance)
	{
		return GetMesh()->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void AShooterCharacter::OnStartReload()
{
    AWeaponGun* CurrentWeaponGun = Cast<AWeaponGun>(CurrentWeapon);
    if(CurrentWeaponGun)
    {
        CurrentWeaponGun->StartReload();
    }
}

void AShooterCharacter::ServerSetCurrentWeapon_Implementation(AWeapon* NewWeapon)
{
    AWeapon* LastWeapon = CurrentWeapon;
    CurrentWeapon = NewWeapon;
    //if new weapon is picked up, need to set it's owner
    if(NewWeapon){
        NewWeapon->SetOwnerCharacter(this);
        NewWeapon->SetOwner(this);
    }
    OnRep_CurrentWeapon(LastWeapon);
}

void AShooterCharacter::OnUnEquip()
{
    ServerSetCurrentWeapon(NULL);
}

int AShooterCharacter::GetCurrentWeaponType()
{
    if(CurrentWeapon)
    {
        return CurrentWeapon->GetWeaponTypeId();
    }
    else{
        return 0;
    }
}

void AShooterCharacter::OnRep_PickUpWeapon()
{
    //only display pick up option on character's owner client
    if(IsLocallyControlled())
    {
        AShooterPlayerController* MyShooterPlayerController = Cast<AShooterPlayerController>(GetController());
        AShooterHUD* MyShooterHUD = MyShooterPlayerController?(Cast<AShooterHUD>(MyShooterPlayerController->GetHUD())):NULL;
        if(MyShooterHUD){
            if(PickUpWeapon)
            {
                MyShooterHUD->OnPickUpWeaponChange(PickUpWeapon->GetWeaponName());
            }
            else
            {
                FString NoPickUp = FString::Printf(TEXT(""));
                MyShooterHUD->OnPickUpWeaponChange(NoPickUp);
            }
        }
    }
}

void AShooterCharacter::SetPickUpWeapon(AWeapon* NewPickUpWeapon)
{
    PickUpWeapon = NewPickUpWeapon;
}

void AShooterCharacter::OnPickUp()
{
    if(PickUpWeapon && !PickUpWeapon->HasOwner())
    {
        ServerSetCurrentWeapon(PickUpWeapon);
    }
}

void AShooterCharacter::OnRep_CurrentWeapon(AWeapon* LastWeapon)
{
    if(LastWeapon){
        //Unequip last weapon
        if(LastWeapon->HasOwner())
        {
            LastWeapon->OnUnEquip();
        }
        //drop last weapon
        else
        {
            LastWeapon->OnDrop();
        }
    }
    
    if(CurrentWeapon)
    {
        CurrentWeapon->OnEquip();
    }
}

void AShooterCharacter::ServerDropCurrentWeapon_Implementation()
{
    if(CurrentWeapon)
    {
        CurrentWeapon->SetOwnerCharacter(NULL);
        AWeapon* LastWeapon = CurrentWeapon;
        CurrentWeapon = NULL;
        OnRep_CurrentWeapon(LastWeapon);
    }
}

void AShooterCharacter::PlayHit(AActor* OtherActor, EHitType HitType, float HitDamage, FVector HitVector, FVector HitImpulse, FString HitBoneName)
{
    if(HitType==EHitType::NormalHit)
    {
        //debug only
        FireSound(HitBoneName);
        //0: back, 1: front, 2: left, 3: right
        int8 HitDirection = 0;
        FVector ForwardVector = GetActorForwardVector();
        FVector2D HitVector2D = FVector2D(HitVector.X, HitVector.Y);
        FVector2D ForwardVector2D = FVector2D(ForwardVector.X, ForwardVector.Y); 
        float DotProduct = FVector2D::DotProduct(HitVector2D, ForwardVector2D);
        //if positve, hit is from back
        if(FVector2D::DotProduct(HitVector2D, ForwardVector2D) > 0)
        {
            HitDirection = 0;
        }
        //otherwise from front
        else
        {
            bool IsLeft = HitBoneName.EndsWith(FString(TEXT("l")), ESearchCase::CaseSensitive);
            bool IsRight = HitBoneName.EndsWith(FString(TEXT("r")), ESearchCase::CaseSensitive);
            if(IsLeft^IsRight)
            {
                HitDirection = IsLeft?2:3;
            }
            else
            {
                HitDirection = 1;
            }
        }
        //ensure replication of all members arrive at the same time
        FTakeHitInfo NewHitInfo = LastHitInfo;
        NewHitInfo.HitDirection = HitDirection;
        NewHitInfo.HitCounter = NewHitInfo.HitCounter+1;
        LastHitInfo = NewHitInfo;
    }
    else
    {
        UCharacterMovementComponent* MyCharacterMovement = Cast<UCharacterMovementComponent>(GetCharacterMovement());
        if(MyCharacterMovement)
        {
            MyCharacterMovement->AddImpulse(HitImpulse, false);
        }
    }
}

void AShooterCharacter::OnRep_LastHitInfo()
{
    SimulateHit();
}

void AShooterCharacter::SimulateHit()
{
    PlayCharacterMontage(TakeHitMontage[LastHitInfo.HitDirection]);
}
