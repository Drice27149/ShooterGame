// Drice

#include "Player/ShooterCharacter.h"
#include "ShooterGame.h"
#include "Weapon/Weapon.h"
#include "Player/MyCharacterMovementComponent.h"

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
    
bool AShooterCharacter::IsJumping()
{
    return bJumping;
}
    
bool AShooterCharacter::IsCrouching()
{
    return bCrouching;
}
    
bool AShooterCharacter::IsTurning()
{
    return bTurning;
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
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShooterCharacter::OnJumpEnd);

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
        ServerStartFire();
    }
}

void AShooterCharacter::ServerStartFire_Implementation()
{
    CurrentWeapon->HandleFire();
    AllClientStartFire();
}

void AShooterCharacter::AllClientStartFire_Implementation()
{
    // play weapon animation
    CurrentWeapon->PlayWeaponFireAnimation();
    // play character animation
    PlayWeaponFireAnimation(CurrentWeapon);
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(AShooterCharacter, CurrentWeapon);
    DOREPLIFETIME(AShooterCharacter, bRunning);
    DOREPLIFETIME(AShooterCharacter, bJumping);
    DOREPLIFETIME(AShooterCharacter, bCrouching);
    DOREPLIFETIME(AShooterCharacter, bTurning);
}

void AShooterCharacter::OnRep_CurrentWeapon()
{
    UpdateWeaponMesh(CurrentWeapon);
    PlayWeaponChangedAnimation(CurrentWeapon);
}

void AShooterCharacter::OnEquipDefaultWeapon()
{
    ServerEquipDefaultWeapon();
}

void AShooterCharacter::ServerEquipDefaultWeapon_Implementation()
{
    if(DefaultWeaponClass != NULL)
    {
        FTransform SpawnTransform(GetActorRotation(), GetActorLocation());
        AWeapon* DefaultWeapon = Cast<AWeapon>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, DefaultWeaponClass, SpawnTransform));
        if(DefaultWeapon != NULL)
        {
            DefaultWeapon->OwnerPawn = Cast<APawn>(this);
            UGameplayStatics::FinishSpawningActor(DefaultWeapon, SpawnTransform);
        }
        CurrentWeapon = DefaultWeapon;
        // server won't call OnRep_CurrentWeapon automatically, need to call it by hand
        OnRep_CurrentWeapon();
    }
}

void AShooterCharacter::OnCrouchStart()
{
    bCrouching = true;
    ServerSetCrouching(true);
}
    
void AShooterCharacter::OnCrouchEnd()
{
    bCrouching = false;
    ServerSetCrouching(false);
}
    
void AShooterCharacter::OnJumpStart()
{
    bJumping = true;
    ServerSetJumping(true);
    Jump();
}
    
void AShooterCharacter::OnJumpEnd()
{
    bJumping = false;
    ServerSetJumping(false);
    StopJumping();
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
    
    if(Value < -MinTurnRate || Value > MinTurnRate)
    {
        bTurning = true;
        ServerSetTurning(true);
    }
    else
    {
        bTurning = false;
        ServerSetTurning(false);
    }
}
    
void AShooterCharacter::OnLookUp(float Value)
{
    if ( (Controller != NULL) && (Value != 0.0f) )
    {
        AddControllerPitchInput(Value);
    }
}
    
void AShooterCharacter::ServerSetRunning_Implementation(bool Value)
{
    bRunning = Value;
}
    
void AShooterCharacter::ServerSetJumping_Implementation(bool Value)
{
    bJumping = Value;
}

void AShooterCharacter::ServerSetCrouching_Implementation(bool Value)
{
    bCrouching = Value;
}
    
void AShooterCharacter::ServerSetViewMode_Implementation(bool Value)
{
    bUseControllerRotationYaw = !Value;
    AllClientSetViewMode(Value);
}

void AShooterCharacter::ServerSetTurning_Implementation(bool Value)
{
    bTurning = Value;
}

void AShooterCharacter::AllClientSetViewMode_Implementation(bool Value)
{
    bUseControllerRotationYaw = !Value;
}


