# MyAnimInstance.h
```
public:
    UMyAnimInstance();
    
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
    float CurrentPawnSpeed;
```
# MyAnimInstance.cpp
```
UMyAnimInstance::UMyAnimInstance()
{
    CurrentPawnSpeed = 0.0f;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    auto Pawn = TryGetPawnOwner();
    if (::IsValid(Pawn))
    {
        CurrentPawnSpeed = Pawn->GetVelocity().Size();
    }
}
```
# 점프 구현
## MyAnimInstance.h
```
private:
...
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
    bool IsInAir;
```
## MyAnimInstance.cpp
```
UMyAnimInstance::UMyAnimInstance()
{
...
    IsInAir = false;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    auto Pawn = TryGetPawnOwner();
    if (::IsValid(Pawn))
    {
        CurrentPawnSpeed = Pawn->GetVelocity().Size();
		
        auto Character = Cast<ACharacter>(Pawn);
        if (Character)
        {
            IsInAir = Character->GetMovementComponent()->IsFalling();
        }
    }
}
```

## MyCharacter.cpp
```
AMyCharacter::AMyCharacter()
{
...
    GetCharacterMovement()->JumpZVelocity = 800.0f;
}

...

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
...
    PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);
...
}
```
