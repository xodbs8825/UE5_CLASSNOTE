![182603030-0e8968d5-5cf4-4ab2-9871-ae354264edc3](https://user-images.githubusercontent.com/97510008/205431696-812ad166-d11b-4797-9d80-1f711c5c4691.png)
![184578133-d76bf5a7-4657-4794-88ca-40227db4acab](https://user-images.githubusercontent.com/97510008/205431656-c3705e59-f778-42fd-86b1-6e43825ded99.png)
![184578151-fe049c78-84e7-4289-99e4-4bc4143b33f8](https://user-images.githubusercontent.com/97510008/205431660-51fda455-3e65-4f78-9f6e-b65502b63aac.png)

### DefaultEngine.ini
![182603137-a6a790a0-767a-4328-b86c-1b27f82ecdf0](https://user-images.githubusercontent.com/97510008/205432013-f005a072-5fae-41a7-bc58-e63a4c7caa27.png)
```
[/Script/Engine.CollisionProfile]
...
+DefaultChannelResponses=(Channel=ECC_GameTraceChannel1,Name="Attack",DefaultResponse=ECR_Ignore,bTraceType=True,bStaticObject=False)
...
```
Attack 의 트레이스 채널이 ECC_GameTraceChannel1 열거형 값을 사용하는 것 확인

### MyCharacter.h
```
...
private:
    void AttackCheck();
  
private:
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    float AttackRange;
    
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
    float AttackRadius;
```
### MyCharacter.cpp
```
...
#include "DrawDebugHelpers.h"

AMyCharacter::AMyCharacter()
{
    ...
    AttackRange = 200.0f;
    AttaclRadius = 50.0f;
}

...

void AMyCharacter::PostInitializeComponents()
{
    ...
    AnimInstance->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);
}

...

void AMyCharacter::AttackCheck()
{
    FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange ,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

    FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
    
    //Capsule 의 Z (Up)를 TraceVec 방향으로 회전 
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
    
    float DebugLifeTime = 5.0f;
    
    DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
        
    if (bResult)
    {
        if (HitResult.GetActor())
        {
            ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
        }
    }
}
```
