// Fill out your copyright notice in the Description page of Project Settings.

#include "UnicornGameModeBase.h"
#include "UnicornAIManager.h"
#include "Runtime/Engine/Classes/Engine/World.h"


AUnicornGameModeBase::AUnicornGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void AUnicornGameModeBase::StartPlay()
{
	Super::StartPlay();

	AActor* SpawnedAIManager = GetWorld()->SpawnActor<AUnicornAIManager>(
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		FActorSpawnParameters());
	AIManager = Cast<AUnicornAIManager>(SpawnedAIManager);
}

AUnicornAIManager* AUnicornGameModeBase::GetAIManager() const
{
	return AIManager;
}

