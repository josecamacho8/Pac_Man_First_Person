// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class PACMAN1STPERSHORROR_API ATile : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Call to initialize tileobjbounds variables after spawning
	void InitBounds(int TileWidth, int TileHeight);
private:
	// Bounds for tile coordinates
	int TileObjBoundsX;
	int TileObjBoundsY;

};
