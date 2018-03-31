// Copyright 2018 Team Unicorn All Rights Reserved

#include "UnicornCharacter.h"
#include "ResourceComponent.h"


// Sets default values
AUnicornCharacter::AUnicornCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize components
	TeamComponent = CreateDefaultSubobject<UTeamComponent>("TeamComponent");
	AddOwnedComponent(TeamComponent);
	SetTeam(ETeam::TE_PlayerEnemy);
	SetEnemyTeam(ETeam::TE_Player);

	HealthComponent = CreateDefaultSubobject<UResourceComponent>("HealthComponent");
	AddOwnedComponent(HealthComponent);
}

// Called when the game starts or when spawned
void AUnicornCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnicornCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnicornCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

USceneComponent* AUnicornCharacter::GetVisionComponent_Implementation() const
{
	return RootComponent;
}

void AUnicornCharacter::SetTeam(ETeam NewTeam)
{
	Team = NewTeam;
	TeamComponent->Team = NewTeam;
}

void AUnicornCharacter::SetEnemyTeam(ETeam NewTeam)
{
	EnemyTeam = NewTeam;
	TeamComponent->EnemyTeam = NewTeam;
}

bool AUnicornCharacter::LoseHealth_Implementation(float HealthLost, bool& bDied)
{
	if (!bDead)
	{
		bool bJustDied;
		HealthComponent->LoseResource(HealthLost, bJustDied);
		bDied = bJustDied;
		if (bJustDied)
		{
			OnDeath();
		}
		return true;
	}
	else
	{
		bDied = false;
		return false;
	}
}

void AUnicornCharacter::OnDeath_Implementation()
{
	bDead = true;
	return;
}

bool AUnicornCharacter::GainHealth_Implementation(float HealthGained)
{
	HealthComponent->GainResource(HealthGained);
	return true;
}


