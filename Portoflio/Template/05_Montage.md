# MyAnimInstance.h
```
public:
    void PlayAttackMontage();

...

private:
...
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    UAnimMontage* AttackMontage;
```
# MyAnimInstance.cpp
```
UMyAnimInstance::UMyAnimInstance()
{
...
    static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Blueprints/KwangAttackMontage.KwangAttackMontage"));
    if (ATTACK_MONTAGE.Succeeded())
    {
        AttackMontage = ATTACK_MONTAGE.Object;
    }
}

...

void UMyAnimInstance::PlayAttackMontage()
{
    if (!Montage_IsPlaying(AttackMontage))
    {
        Montage_Play(AttackMontage, 1.0f);
    }
}
```
