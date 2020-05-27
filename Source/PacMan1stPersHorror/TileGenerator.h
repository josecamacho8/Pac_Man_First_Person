// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScorePickup.h"
#include "TileGenerator.generated.h"

UCLASS()
class PACMAN1STPERSHORROR_API ATileGenerator : public AActor
{
	GENERATED_BODY()
	
	// TODO: create inner tile class within this class
	// TODO: support mirroring

public:	
	// Sets default values for this actor's properties
	ATileGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Tile System Constraints
	int TileCountX;
	int TileCountY;
	int TotalTiles;
	int TileWidth;
	int TileHeight;

	/*
	Called by actors traversing tile system. 

	Parameters:
		-	int CoordinateX: X coordinate location of actor 
		-	int CoordinateY: Y coordinate location of actor

	Returns: int
		-	[0, n): Index of tile location 
		-	-1: If coordinates lay outside of known tiles
	*/
	UFUNCTION(BlueprintCallable)
	int GetTileFromCoordinates(FVector Location);

	/*
	Returns FVector located at center of tile[TileIndex]
	*/
	UFUNCTION(BlueprintCallable)
	FVector GetCoordinatesFromTile(int TileIndex);

	/*
	Used by AI to determine which tiles around this current tile are closest to target.
	Gets surrounding tiles (up, left, down, right), calculates distance from those to target
	and returns shortest distance tile. At point of tie, priority given in above mentioned order.
	*/
	UFUNCTION(BlueprintCallable)
	int GetClosestTileToTarget(FVector CurrentLocation, FVector TargetLocation, int PrevTile);

private:
	UWorld* ThisWorld;

	enum TileType {EmptyFloor, GlassWall, ExternalWall, ScorePickupFloor, TeleportWall};
	class Tile {
		public:
			UStaticMeshComponent* MeshComponent;
			TileType Type;
	};

	// Used to determine if specific tile is Glass/ExternalWall
	bool IsWall(Tile* ATile);

	// Used to cache bounds of entire system. No idea if it improves performance. Research(?)
	float TileXLowerBound, TileXUpperBound;
	float TileYLowerBound, TileYUpperBound;

	// Used as common parent for all scenes.
	USceneComponent* RootScene;

	// Object array for ATiles.
	TArray<Tile*> TileArr;

	// Object array for ScorePickups.
	TArray<AScorePickup*> ScorePickupArr;

	/*
	Called at consturction of TileGenerator class. Fills TileArr based off
	TileCounts/TileWidth/TileHeight.
	*/
	void GenerateTilesForLevel();

	// Called by generator functon. Used to create physical representation of tile.
	void CreateTile(float X, float Y, float Z, int TileN, int TileTypeInt);
};
