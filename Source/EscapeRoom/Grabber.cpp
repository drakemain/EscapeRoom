// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoom.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabbing system online!!"))
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerLocation;
	FRotator PlayerRotation;
	FVector GrabberReach;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	GrabberReach = PlayerRotation.Vector() * 100 + PlayerLocation;

	//UE_LOG(LogTemp, Warning, TEXT("%s | %s"), *PlayerLocation.ToString(), *PlayerRotation.ToString())
	DrawDebugLine(GetWorld(), PlayerLocation, GrabberReach, FColor(255, 0, 0));

	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(Hit, PlayerLocation, GrabberReach, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
	
	if (Hit.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("GRABBER HIT: %s"), *Hit.GetActor()->GetName())
	}
}

