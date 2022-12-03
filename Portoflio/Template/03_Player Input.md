# MyCharacter.h
```
public:
 ...
    void MoveForward(float NewAxisValue);
    void MoveRight(float NewAxisValue);
    void Turn(float NewAxisValue);
    void LookUp(float NewAxisValue);
```
# MyCharacter.cpp
```
// Sets default values
AMyCharacter::AMyCharacter()
{
...
    // SetControlMode
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bInheritPitch = true;
    SpringArm->bInheritRoll = true;
    SpringArm->bInheritYaw = true;
	
    bUseControllerRotationYaw = false;
	
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AMyCharacter::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &AMyCharacter::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &AMyCharacter::Turn);
    PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &AMyCharacter::LookUp);
}

void AMyCharacter::MoveForward(float NewAxisValue)
{
    AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), (NewAxisValue * 1.0f));
}

void AMyCharacter::MoveRight(float NewAxisValue)
{
    AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), (NewAxisValue * 1.0f));
}

void AMyCharacter::Turn(float NewAxisValue)
{
    AddControllerYawInput(NewAxisValue);
}

void AMyCharacter::LookUp(float NewAxisValue)
{
    AddControllerPitchInput(NewAxisValue);
}
```
# Player Input

![183332114-67451f6b-3dfc-4570-85b2-1f509eaad6ba](https://user-images.githubusercontent.com/97510008/202838155-ccd58055-c1db-40b6-949a-14cde9d4d1d8.png)
