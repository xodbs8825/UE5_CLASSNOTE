// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "CharacterStatComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);


	// SetControllMode
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;	
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	//캐릭터가 Yaw 회전을 하지 않도록
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	CharacterStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("CHARACTERSTAT"));
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnim)
	{
		MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackHitCheck);
	}

	if (CharacterStat)
	{
		CharacterStat->OnHPIsZero.AddUObject(this, &AMyCharacter::OnHPIsZero);
		//CharacterStat->OnHPIsZero.AddLambda([this]()->void
		//	{
		//		MyAnim->SetAnimDeath();
		//		SetActorEnableCollision(false);
		//	});
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	
	//MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			TEXT("AMyCharacter::OnAttackMontageEnded"));
}

void AMyCharacter::OnHPIsZero()
{
	MyAnim->SetAnimDeath();
	SetActorEnableCollision(false);
}

void AMyCharacter::AttackHitCheck()
{
	float AttackRange = 200.0f;
	float AttackRaduis = 50.0f;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, 
			TEXT("AMyCharacter::AttackHitCheck"));

	//ECC_GameTraceChannel1
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(AttackRaduis),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRaduis;

	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;

	float DebugLifeTime = 5.0f;
	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRaduis,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#endif

	if (bResult)
	{
		if (HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());

			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			//FVector Dir = HitResult.GetActor()->GetActorLocation() - GetActorLocation();
			//FQuat LookAtRot = FRotationMatrix::MakeFromX(Dir).ToQuat();
			//SetActorRotation(LookAtRot);
		}
	}
}


float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	if (MyAnim)
	{
		CharacterStat->SetDamage(FinalDamage);
	}

	return FinalDamage;
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//MoveForward
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	//MoveRight
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	//Turn
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	//LookUp
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);



	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this,  &AMyCharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);

}


void AMyCharacter::MoveForward(float NewAxisValue)
{
	//GetActorForwardVector()
	//AddMovementInput(GetActorForwardVector(), NewAxisValue);

	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);

}

void AMyCharacter::MoveRight(float NewAxisValue)
{
	//
	//GetActorRightVector()
	//AddMovementInput(GetActorRightVector(), NewAxisValue);
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);

}


void AMyCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);

}

void AMyCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMyCharacter::Attack()
{
	// 애님인스턴스의 PlayAttackMontage
	MyAnim->PlayAttackMontage();


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Attack"));
}
