// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReporter.h"

#include "EscapeGame.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPositionReporter::UPositionReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPositionReporter::BeginPlay()
{
	Super::BeginPlay();

	// stores name of object that the pointer is pointing to
	FString ObjectName = GetOwner()->GetName();

	// macro - log it's going to - warning - text that holds a string
	// prints name of object with %s
	UE_LOG(LogTemp, Warning, TEXT("Position report reporting for %s"), *ObjectName);
	
}


// Called every frame
void UPositionReporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

