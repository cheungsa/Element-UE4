// Copyright Sarah Cheung 2018.

#include "OpenDoor.h"
#include "EscapeGame.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	// Find default pawn in World
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	// Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f)); 
}

void UOpenDoor::CloseDoor()
{
	// Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f)); // 0.0f
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume in every frame
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		// If the ActorThatOpens is in the volume
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

