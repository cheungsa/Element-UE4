// Copyright 2018 Team Unicorn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VRAIController.h"
#include "UnicornAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNICORN_API AUnicornAIController : public AVRAIController
{
	GENERATED_BODY()
public:
	AUnicornAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Override for vision location and rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Vision")
	USceneComponent* VisionComponent;
	
	/** How high we expect a VR character's chest to be, based on the height of the VR camera component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float VRAimLocationHeightMultiplier;

	/** How long we should wait when we are on a path before we try to find a new path */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float TimeOnPathUntilRepath;

	/** How long we want to wait during an attack before abandoning it */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float AttackTimoutTime;

	/** How close counts as melee range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float MeleeRange;
		
	/** Override for the character's eyesight */
	virtual void GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;

	/** Gets the aim location on target actor */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI|Vision")
	FVector GetAimLocationOnActor(AActor* TargetActor) const;

	/** Sets the target actor for this AI Controller's Blackboard */
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard Keys")
	void SetTargetActor(AActor* TargetActor);

	/** Gets the target actor for this AI Controller's Blackboard */
	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	AActor* GetTargetActor() const;

	/** Sets the passive state for this AI Controller's Blackboard */
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard Keys")
	void SetIsPassive(bool bIsPassive);

	/** Gets Is Passive for this AI Controller's Blackboard */
	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	const bool GetIsPassive();

	/** Sets the goal location for this AI Controller's Blackboard */
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard Keys")
	void SetGoalLocation(FVector GoalLocation);

	/** Gets the goal location for this AI Controller's Blackboard */
	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	const FVector GetGoalLocation();

	/** Sets the target recently teleported key for this AI Controller's Blackboard */
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard Keys")
	void SetTargetRecentlyTeleported(bool bTargetRecentlyTeleported);

	/** Gets the target recently teleported key for this AI Controller's Blackboard */
	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	const bool GetTargetRecentlyTeleported();

	/** Sets the stunned key for this AI Controller's Blackboard */
	UFUNCTION(BlueprintCallable, Category = "AI|Blackboard Keys")
	void SetStunned(bool bStunned);

	/** Gets the stunned key for this AI Controller's Blackboard */
	UFUNCTION(BlueprintPure, Category = "AI|Blackboard Keys")
	const bool GetStunned();

	/** Gets the controlled Unicorn Character */
	UFUNCTION(BlueprintPure, Category = "AI")
	class AUnicornCharacter* GetUnicornCharacter() const;

	/** Gets the target actor for this AI Controller's Blackboard */
	UFUNCTION(BlueprintPure, Category = "AI|Combat")
	const bool IsWithinMeleeRange(AActor* Actor);

	/** Gets the AI Manager spawned by the Game Mode */
	UFUNCTION(BlueprintPure, Category = "AI")
	class AUnicornAIManager* GetAIManager() const;

	/** Overridable function to be called when player teleports */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void OnTargetTeleported();
};