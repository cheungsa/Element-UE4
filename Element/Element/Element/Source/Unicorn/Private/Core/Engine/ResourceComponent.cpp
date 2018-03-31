// Copyright 2018 Team Unicorn All Rights Reserved

#include "ResourceComponent.h"


// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxResource = 100.0f;
	CurrentResource = MaxResource;
}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MaxResource < 0.0f)
	{
		MaxResource = 0.0f;
	}

	if (CurrentResource > MaxResource && MaxResource != 0.0f)
	{
		CurrentResource = MaxResource;
	}
	if (CurrentResource < 0)
	{
		CurrentResource = 0;
	}
	
}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResourceComponent::SetCurrentResource(float NewCurrentResource)
{
	if (MaxResource > 0.0f)
	{
		CurrentResource = FMath::Clamp(NewCurrentResource, 0.0f, MaxResource);
	}
	else
	{
		CurrentResource = NewCurrentResource;
	}
}

const float UResourceComponent::GetCurrentResource()
{
	return CurrentResource;
}

void UResourceComponent::SetMaxResource(float NewMaxResource)
{
	MaxResource = NewMaxResource;
	if (MaxResource <= 0.0f)
	{
		MaxResource = 0.0f;
	}
	else if (CurrentResource > MaxResource)
	{
		CurrentResource = MaxResource;
	}
}

const float UResourceComponent::GetMaxResource()
{
	return MaxResource;
}

void UResourceComponent::GainResource(float AmountGained)
{
	if (AmountGained <= 0.0f)
	{
		return;
	}
	else
	{
		SetCurrentResource(CurrentResource + AmountGained);
	}
}

void UResourceComponent::LoseResource(float AmountLost, bool& bDepleted)
{
	SetCurrentResource(CurrentResource - AmountLost);
	if (CurrentResource <= 0.0f)
	{
		bDepleted = true;
	}
	else
	{
		bDepleted = false;
	}
}

const bool UResourceComponent::IsDepleted()
{
	return (CurrentResource <= 0.0f);
}

const bool UResourceComponent::IsAtMaxCapacity()
{
	return (CurrentResource >= MaxResource && MaxResource > 0.0f);
}

const bool UResourceComponent::CanAfford(float Cost)
{
	return (Cost <= CurrentResource);
}