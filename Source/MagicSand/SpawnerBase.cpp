// By Amos Johan Persson


#include "SpawnerBase.h"

USpawnerBase::USpawnerBase()
{

}

AProjectileBase* USpawnerBase::SpawnProjectile(
	TSubclassOf<AProjectileBase> InClass,
	const FVector* Location,
	const FRotator* Rotation,
	FActorSpawnParameters SpawnParams
)
{
	UWorld* const World = GetWorld();
	AActor* Spawned = World->SpawnActor(InClass, Location, Rotation, SpawnParams);
	AProjectileBase* Projectile = (AProjectileBase*)Spawned;

	return Projectile;
}

AProjectileBase* USpawnerBase::SpawnSingleProjectile(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters Params = FActorSpawnParameters();
	Params.SpawnCollisionHandlingOverride = SpawnActorCollisionHandlingMethod;

	return SpawnProjectile(ProjectileClass, &Location, &Rotation, Params);
}

TArray<AProjectileBase*> USpawnerBase::SpawnProjectiles_Implementation(FVector Location, FRotator Rotation)
{
	TArray<AProjectileBase*> Projectiles = TArray<AProjectileBase*>();

	FVector AdjustedLoaction = Location + Rotation.RotateVector(MuzzleOffset);
	Projectiles.Add(SpawnSingleProjectile(AdjustedLoaction, Rotation));

	return Projectiles;
}