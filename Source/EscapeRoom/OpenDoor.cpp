// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoom.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	SetPositions();
}

// Set the open and close positions
void UOpenDoor::SetPositions()
{
	this->ClosedPosition = GetOwner()->GetActorRotation();
	this->OpenPosition = this->ClosedPosition;
	this->OpenPosition.Add(0.f, this->OpenAngle, 0.f);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->GetTriggerWeight() >= this->TriggerWeightMinimum)
	{
		Open();
		this->LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > this->CloseDelay)
		{
			Close();
		}
	}
}

void UOpenDoor::Open()
{
	if (GetOwner()->GetActorRotation() != this->OpenPosition)
	{
		GetOwner()->SetActorRelativeRotation(this->OpenPosition);
	}
}

void UOpenDoor::Close()
{
	if (GetOwner()->GetActorRotation() != this->ClosedPosition)
	{
		GetOwner()->SetActorRelativeRotation(this->ClosedPosition);
	}
}

float UOpenDoor::GetTriggerWeight()
{
	float TotalMass = 0.f;
	TArray<AActor*> ActorsOnTrigger;
	Trigger->GetOverlappingActors(ActorsOnTrigger);

	for (const auto& ActorOnTrigger : ActorsOnTrigger) {
		TotalMass += ActorOnTrigger->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s: %f"), *ActorOnTrigger->GetName(), ActorOnTrigger->FindComponentByClass<UPrimitiveComponent>()->GetMass())
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Mass: %f"), TotalMass)

	return TotalMass;
}
