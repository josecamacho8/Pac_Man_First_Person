// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScorePickup.generated.h"

UCLASS()
class PACMAN1STPERSHORROR_API AScorePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScorePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* MeshComponent;

private:
	// Called when character static mesh overlaps pickup.
	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
