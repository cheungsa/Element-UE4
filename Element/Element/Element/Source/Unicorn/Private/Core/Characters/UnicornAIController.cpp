// Copyright 2018 Team Unicorn All Rights Reserved

#include "UnicornAIController.h"
#include "UnicornCharacter.h"
#include "UnicornBBKeys.h"
#include "UnicornGameModeBase.h"
#include "UnicornAIManager.h"
#include "GameFramework/GameModeBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AUnicornAIController::AUnicornAIController(const FObjectInitializer& ObjectInitializer)
{
	VisionComponent = nullptr;
	VRAimLocationHeightMultiplier = 0.9f;
	TimeOnPathUntilRepath = 15.0f;
	MeleeRange = 160.0f;
	AttackTimoutTime = 10.0f;
}

void AUnicornAIController::GetActorEyesViewPoint(FVector& out_Location, FRotator& out_Rotation) const
{
	// Vision component override
	if (VisionComponent)
	{
		FTransform VisionTransform = VisionComponent->GetComponentTransform();
		out_Location = VisionTransform.GetLocation();
		out_Rotation = VisionTransform.Rotator();
		return;
	}
	AUnicornCharacter* UnicornCharacter = Cast<AUnicornCharacter>(GetPawn());

	// Default vision component from Unicorn Character
	if (UnicornCharacter)
	{
		USceneComponent* CharVisionComp = UnicornCharacter->GetVisionComponent();
		if (CharVisionComp)
		{
			FTransform VisionTransform = CharVisionComp->GetComponentTransform();
			out_Location = VisionTransform.GetLocation();
			out_Rotation = VisionTransform.Rotator();
			return;
		}
	}

	return Super::GetActorEyesViewPoint(out_Location, out_Rotation);
}

FVector AUnicornAIController::GetAimLocationOnActor(AActor* Actor) const
{
	// If the target is a player pawn, then get the aim position based on the camera and ZOffset
	AVRBaseCharacter* VRCharacter = Cast<AVRBaseCharacter>(Actor);
	if (VRCharacter)
	{
		// Get the camera location
		const UCameraComponent* Camera = VRCharacter->VRReplicatedCamera;
		if (Camera != nullptr)
		{
			// Get the camera's initial location
			FVector AimLoc = Camera->GetRelativeTransform().GetLocation();

			// Multiply the Z by the multiplier
			AimLoc.Z = AimLoc.Z * VRAimLocationHeightMultiplier;
			AimLoc = Camera->GetOwner()->GetTransform().TransformPosition(AimLoc);
			return AimLoc;
		}
	}
	// Otherwise, if its an actor, aim at the center of their bounding box
	if (Actor)
	{
		FBox ActorBoundingBox = Actor->GetComponentsBoundingBox();

		// Consider Child Actor components
		// This is a workaround for GetActorBounds not considering child actor components
		for (const UActorComponent* ActorComponent : Actor->GetComponents())
		{
			UChildActorComponent const* const ChildActorComponent = Cast<const UChildActorComponent>(ActorComponent);
			AActor const* const CurrChildActor = ChildActorComponent ? ChildActorComponent->GetChildActor() : nullptr;
			if (CurrChildActor)
			{
				ActorBoundingBox += CurrChildActor->GetComponentsBoundingBox();
			}
		}

		if (ActorBoundingBox.IsValid)
		{
			FVector BoundsOrigin = ActorBoundingBox.GetCenter();
			return BoundsOrigin;
		}
	}

	if (Actor)
	{
		return Actor->GetActorLocation();
	}
	else
	{
		return FVector::ZeroVector;
	}
}

void AUnicornAIController::SetGoalLocation(FVector GoalLocation)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsVector(UUnicornBBKeys::GoalLocation(), GoalLocation);
	}
}

void AUnicornAIController::SetIsPassive(bool bIsPassive)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(UUnicornBBKeys::IsPassive(),bIsPassive);
	}
}

void AUnicornAIController::SetTargetActor(AActor* TargetActor)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsObject(UUnicornBBKeys::TargetActor(), TargetActor);
	}
}

const FVector AUnicornAIController::GetGoalLocation()
{
	if (Blackboard)
	{
		return (Blackboard->GetValueAsVector(UUnicornBBKeys::GoalLocation()));
	}

	return FVector::ZeroVector;
}

AActor* AUnicornAIController::GetTargetActor() const
{
	if (Blackboard)
	{
		return (Cast<AActor>(Blackboard->GetValueAsObject(UUnicornBBKeys::TargetActor())));
	}

	return nullptr;
}

const bool AUnicornAIController::GetIsPassive()
{
	if (Blackboard)
	{
		return (Blackboard->GetValueAsBool(UUnicornBBKeys::IsPassive()));
	}

	return false;
}

AUnicornCharacter* AUnicornAIController::GetUnicornCharacter() const
{
	AUnicornCharacter* UnicornCharacter = Cast<AUnicornCharacter>(GetPawn());
	
	return UnicornCharacter;
}

const bool AUnicornAIController::IsWithinMeleeRange(AActor* Actor)
{
	if (Actor)
	{
		FVector TargetLocation = GetAimLocationOnActor(Actor);
		TargetLocation.Z = 0.0f;

		FVector CurrentLocation = GetPawn()->GetActorLocation();
		CurrentLocation.Z = 0.0f;

		FVector Distance = (TargetLocation - CurrentLocation);
		return (Distance.Size() <= MeleeRange);
	}
	return false;
}

AUnicornAIManager* AUnicornAIController::GetAIManager() const
{
	AUnicornGameModeBase* UnicornGameMode = Cast<AUnicornGameModeBase>(GetWorld()->GetAuthGameMode());
	if (UnicornGameMode)
	{
		return (UnicornGameMode->GetAIManager());
	}
	else
	{
		return nullptr;
	}
}

void AUnicornAIController::OnTargetTeleported_Implementation()
{

}

void AUnicornAIController::SetTargetRecentlyTeleported(bool bTargetRecentlyTeleported)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(UUnicornBBKeys::TargetRecentlyTeleported(), bTargetRecentlyTeleported);
	}
}

const bool AUnicornAIController::GetTargetRecentlyTeleported()
{
	if (Blackboard)
	{
		return Blackboard->GetValueAsBool(UUnicornBBKeys::TargetRecentlyTeleported());
	}
	else
	{
		return false;
	}
}

void AUnicornAIController::SetStunned(bool bStunned)
{
	if (Blackboard)
	{
		Blackboard->SetValueAsBool(UUnicornBBKeys::Stunned(), bStunned);
	}
}

const bool AUnicornAIController::GetStunned()
{
	if (Blackboard)
	{
		return Blackboard->GetValueAsBool(UUnicornBBKeys::Stunned());
	}
	else
	{
		return false;
	}
}