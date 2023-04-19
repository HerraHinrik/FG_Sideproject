// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerBase.h"

USpawnerBase::USpawnerBase()
{

}

AActor* USpawnerBase::SpawnActor(
	UClass* InClass,
	const FVector* Location,
	const FRotator* Rotation,
	FActorSpawnParameters SpawnParams
)
{
	UWorld* const World = GetWorld();

	return World->SpawnActor(InClass, Location, Rotation, SpawnParams);
}

AActor* USpawnerBase::SpawnSingleProjectile(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters Params = FActorSpawnParameters();
	Params.SpawnCollisionHandlingOverride = SpawnActorCollisionHandlingMethod;

	return SpawnActor(ProjectileClass, &Location, &Rotation, Params);
}

TArray<AActor*> USpawnerBase::SpawnProjectiles(FVector Location, FRotator Rotation)
{
	TArray<AActor*> Projectiles = TArray<AActor*>();

	FVector AdjustedLoaction = Location + Rotation.RotateVector(MuzzleOffset);
	Projectiles.Add(SpawnSingleProjectile(AdjustedLoaction, Rotation));

	return Projectiles;
}