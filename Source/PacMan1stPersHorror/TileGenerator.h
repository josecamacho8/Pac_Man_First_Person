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

	UPROPERTY(BlueprintReadOnly)
	int TileWidth;

	UPROPERTY(BlueprintReadOnly)
	int TileHeight;

	UPROPERTY(BlueprintReadOnly)
	int TotalTiles;
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
	Used by Blinkty to determine which tiles around this current tile are closest to target.
	Gets surrounding tiles (up, left, down, right), calculates distance from those to target
	and returns shortest distance tile. At point of tie, priority given in above mentioned order.
	*/
	UFUNCTION(BlueprintCallable)
	int GetClosestTileToTarget(FVector CurrentLocation, FVector TargetLocation, int PrevTile);


	/*
	Used by Pinky to determine chase tile. Pinky chases four tiles ahead of the current
	players direction. Calculate four tiles ahead of this tile system, based on using prev and 
	current player tiles to calculate direction.
	*/
	UFUNCTION(BlueprintCallable)
	int GetPinkyChaseTile(int PrevPlayerTile, int CurrentPlayerTile);

	/*
	Used by Inky to calculate chase tile. Inky takes Players current tile, Blinkies current tile, 
	and doubles the distance.
	*/
	UFUNCTION(BlueprintCallable)
	int GetInkyChaseTile(FVector BlinkyCurrentLocation, int PrevPlayerTile, int CurrentPlayerTile);

	/*
	Used  by Clyde to determine if within Clyde's radial distance of 8 tiles. Uses TileWidth to determine TileSize.
	*/
	UFUNCTION(BlueprintCallable)
	bool IsClydeWithinEightTiles(FVector ClydeLocation, FVector PlayerLocation);

	// Used to store layout of levels
	UPROPERTY(BlueprintReadOnly)
	TArray<int> TileLevelLayout;

private:
	UWorld* ThisWorld;


	enum TileType {EmptyFloor, GlassWall, ExternalWall, ScorePickupFloor, PowerupPickupFloor, TeleportWall};
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


	/*
	Used after world spawned. Need to live in world to create actors.
	*/
	void SpawnPickups();

	// Called by generator functon. Used to create physical representation of tile.
	void CreateTile(float X, float Y, float Z, int TileN, int TileTypeInt);

	/*
	Used by Inky to calculating 
	*/
	int GetOffsetTwoTilesFromPlayer(int PrevPlayerTile, int CurrentPlayerTile);
};
