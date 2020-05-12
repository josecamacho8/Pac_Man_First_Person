// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "TileGenerator.generated.h"

UCLASS()
class PACMAN1STPERSHORROR_API ATileGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Tile Coordinate Origin X/Y
	int TileCoordinatesOriginX;
	int TileCoordinatesOriginY;
	int TileCoordinatesOriginZ;

	// Tile Count X/Y
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TileCountX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TileCountY;

	// Tile Width/Height
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TileWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TileHeight;

	/*
	Main function to be called by actors traversing tile system.

	Parameters:
		-	int CoordinateX: X coordinate location of actor 
		-	int CoordinateY: Y coordinate location of actor

	Returns: int
		-	[0, TotalTiles): Index of tile location 
		-	TotalTiles: If coordinates lay outside of known tiles
	*/
	int GetTileFromCoordinates(int CoordinateX, int CoordinateY);

private:
	// Main function. Generates tiles based on height and width.
	virtual void GenerateTilesForLevel();

	// Object array for ATiles.
	TArray<ATile*> TileArr;

	// LevelTileHeight x LevelTileWidth
	int TotalTiles;

};
