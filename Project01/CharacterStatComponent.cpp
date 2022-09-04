// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"
#include "MyGameInstance.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	Level = 2;
}

void UCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);
}

void UCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance == nullptr) return;
	
	CurrentStatData = MyGameInstance->GetMyCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		CurrentHP = CurrentStatData->MaxHP;
	
		UE_LOG(LogTemp, Warning, TEXT("%d Level CurrentHP : %f"), Level, CurrentStatData->MaxHP);
	}

}

float UCharacterStatComponent::GetAttack()
{
	if (CurrentStatData == nullptr) return 0.0f;

	return CurrentStatData->Attack;
}

void UCharacterStatComponent::SetDamage(float NewDamage)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);

	if (CurrentHP <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentHP <= 0.0f, Dead!!!"));
		OnHPIsZero.Broadcast();
	}
}

// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

