270 페이지

# MyCharacter.h
```
...
private:
    ...
    void AttackStartComboState();
    void AttackEndComboState();

private:
    ...
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    bool CanNextCombo;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    bool IsComboInputOn;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    int32 CurrentCombo;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    int32 MaxCombo;
    ...
...
```
# MyCharacter.cpp
```
...
AMyCharacter::AMyCharacter()
{
    ...
    MaxCombo = 4;
    AttackEndComboState();
}
...
void AMyCharacter::AttackStartComboState()
{
    CanNextCombo = true;
    IsComboInputOn = false;
    ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1);
    CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AMyCharacter::AttackEndComboState()
{
    IsComboInputOn = false;
    CanNextCombo = false;
    CurrentCombo = 0;
}
```
# MyAnimInstance.h
```
...
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
...
public:
    void JumpToAttackMontageSection(int32 NewSection);

public:
    FOnNextAttackCheckDelegate OnNextAttackCheck;
    FOnAttackHitCheckDelegate OnAttackHitCheck;
    
private:
    ...
    UFUNCTION()
    void AnimNotify_NextAttackCheck();
    
    FName GetAttackMontageSectionName(int32 Section);
    ...
...
```
# MyAnimInstance.cpp
```
...
void UMyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
    ABCHECK(Montage_IsPlaying(AttackMontage);
    Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UMyAnimInstance::AnimNotify_AttackHitCheck()
{
    OnAttackHitCheck.Broadcast();
}

void UMyAnimInstance::AnimNotify_NextAttackCheck()
{
    OnNextAttackCheck.Broadcast();
}

FName UMyAnimInstance::GetAttackMontageSectionName(int32 Section)
{
    ABCHECK(FMath::InWithinInclusive<int32>(Section, 1, 4), NAME_None);
    return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
```
