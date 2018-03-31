// Copyright 2018 Team Unicorn All Rights Reserved

#include "TeamComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"


// Sets default values for this component's properties
UTeamComponent::UTeamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Team = ETeam::TE_Player;
	EnemyTeam = ETeam::TE_PlayerEnemy;
}


// Called when the game starts
void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTeamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

const bool UTeamComponent::IsSameTeam(AActor* OtherActor)
{
	TArray<UTeamComponent*> TeamComps;
	OtherActor->GetComponents<UTeamComponent>(TeamComps);
	if (TeamComps.Num() > 0)
	{
		if (TeamComps[0])
		{
			return TeamComps[0]->Team == Team;
		}
	}

	return false;
}

const bool UTeamComponent::IsEnemyTeam(AActor* OtherActor)
{
	TArray<UTeamComponent*> TeamComps;
	OtherActor->GetComponents<UTeamComponent>(TeamComps);
	if (TeamComps.Num() > 0)
	{
		if (TeamComps[0])
		{
			return TeamComps[0]->Team == EnemyTeam;
		}
	}

	return false;
}
