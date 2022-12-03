# MyCharacter.h
```
public:
...
    virtual void PostInitializeComponents() override;
    
private:
...
    void Attack();
    
    UFUNCTION()
    void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    bool IsAttacking;
```
# MyCharacter.cpp
```
...
#include "MyAnimInstance.h"
...

AMyCharacter::AMyCharacter()
{
    ...
    IsAttacking = false;
}

void AMyCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    auto AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
    ABCHECK(nullptr != AnimInstance);
    
    AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded));
...

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    ...
    PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
}

void AMyCharacter::Attack()
{
    if (IsAttacking) return;
    
    auto AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
    if (nullptr == AnimInstance) return;
    
    AnimInstance->PlayAttackMontage();
    IsAttacking = true;
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    ABCHECK(IsAttacking);
    IsAttacking = false;
}
``` 
# 공격 노티파이

![183912475-2d37f9d1-09eb-40ed-9a1a-1b2fb9ae35ec](https://user-images.githubusercontent.com/97510008/205428420-0b8814e0-0add-44b4-a6dc-8b8b1224da65.png)

## MyAnimInsatance.h
```
public:
    ...
    UFUNCTION()
    void AnimNotify_AttackHitCheck();
```
## MyAnimInstance.cpp
```
...
void UMyAnimInstance::AnimNotify_AttackHitCheck()
{
    ABLOG_S(Warning);
}
```
