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

AProjectileBase* USpawnerBase::SpawnSingleProjectile(FVector Location, FRotator Rotation, int32 RawDamageBoost, float DamagePercentageBoost)
{
	FActorSpawnParameters Params = FActorSpawnParameters();
	Params.SpawnCollisionHandlingOverride = SpawnActorCollisionHandlingMethod;
	
	AProjectileBase* Projectile = SpawnProjectile(ProjectileClass, &Location, &Rotation, Params);
	Projectile->AddToRawDamage(RawDamageBoost);
	Projectile->AddToDamageMultiplier(DamagePercentageBoost);
	Projectile->SetTeamNumber(TeamNumber);

	return Projectile;
}

TArray<AProjectileBase*> USpawnerBase::SpawnProjectiles_Implementation(FVector Location, FRotator Rotation, int32 RawDamageBoost, float DamagePercentageBoost)
{
	TArray<AProjectileBase*> Projectiles = TArray<AProjectileBase*>();

	FVector AdjustedLoaction = Location + Rotation.RotateVector(MuzzleOffset);
	Projectiles.Add(SpawnSingleProjectile(AdjustedLoaction, Rotation, RawDamageBoost, DamagePercentageBoost));

	return Projectiles;
}

void USpawnerBase::SetTeamNumber(int32 Team)
{
	TeamNumber = Team;
}
