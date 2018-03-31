// Copyright 2018 Team Unicorn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	TE_Player	UMETA(DisplayName = "Player"),
	TE_PlayerEnemy	UMETA(DisplayName = "PlayerEnemy")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNICORN_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeamComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** The team of this component and its owning actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	ETeam Team;

	/** The enemy team of this component and its owning actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	ETeam EnemyTeam;

	/** Gets whether the other actor has the same team as this component. Returns false if other actor does not implement a team component*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Team")
	const bool IsSameTeam(AActor* OtherActor);

	/** Gets whether the other actor has the team designated as the enemy of this component. Returns false if other actor does not implement a team component*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Team")
	const bool IsEnemyTeam(AActor* OtherActor);

};
