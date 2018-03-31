// Copyright 2018 Team Unicorn All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNICORN_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Max amount of resources. Infinite if 0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetMaxResource, Category = "Resources")
	float MaxResource;

	/** Current amount of resources. Infinite if 0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetCurrentResource, Category = "Resources")
	float CurrentResource;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Setter for the current resource count. Clamped between 0 and Max Resource */
	UFUNCTION(BlueprintCallable, Category = "Resources")
	void SetCurrentResource(float NewCurrentResource);

	/** Setter for Max Resource */
	UFUNCTION(BlueprintCallable, Category = "Resources")
	void SetMaxResource(float NewMaxResource);

	/** Function for gaining resources. Clamped between 0 and Max Resource */
	UFUNCTION(BlueprintCallable, Category = "Resources")
	void GainResource(float GainAmount);

	/** Function for losing resources. Clamped between 0 and Max Resource */
	UFUNCTION(BlueprintCallable, Category = "Resources")
	void LoseResource(float LossAmount, bool& bDepleted);

	/** Gets whether the current resource is depleted */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	const bool IsDepleted();

	/** Gets whether the current resource is equal to Max Resource */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	const bool IsAtMaxCapacity();

	/** Gets whether the current resource is equal to Max Resource */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Resources")
	const bool CanAfford(float Cost);

	const float GetCurrentResource();
	const float GetMaxResource();
};
