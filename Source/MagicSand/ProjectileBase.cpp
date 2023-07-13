// By Amos Johan Persson


#include "ProjectileBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	RawDamage = 0.0f;
	DamageMultiplier = 1.0f;
	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;


	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

}


void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}


void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Lifetime += DeltaTime;

	if (Lifetime >= LifeSpan) Destroy();
	
}

float AProjectileBase::CalculateDamage()
{
	return RawDamage * DamageMultiplier;
}


void AProjectileBase::SetRawDamage(float Amount)
{
	RawDamage = Amount;
}

void AProjectileBase::ApplyDamageModifier(float Amount)
{
	//Amount needs to be in decimal form for x% increased or negative for x% decreased
	DamageMultiplier += Amount;

	if (DamageMultiplier >= DamageMultiplierCap)
	{
		DamageMultiplier = DamageMultiplierCap;
	}
}


USphereComponent* AProjectileBase::GetCollisionComponent()
{
	return CollisionComp;
}

UProjectileMovementComponent* AProjectileBase::GetMovementComponent()
{
	return ProjectileMovement;
}

void AProjectileBase::RegisterOnHitComponent(UOnHitComponentBase* OnHitComponent)
{
	OnHitComponents.Add(OnHitComponent);
}

void AProjectileBase::PropagateHitToComponents(AActor* InActor, FVector HitLocation, FVector ImpactNormal)
{
	for (auto OnHitComponent : OnHitComponents)
	{
		OnHitComponent->ProcessOnHit(InActor, HitLocation, ImpactNormal);
	}
}