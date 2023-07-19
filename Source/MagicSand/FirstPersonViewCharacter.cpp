// By Amos Johan Persson

#include "FirstPersonViewCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AFirstPersonViewCharacter::AFirstPersonViewCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//create initial gun component
	WeaponComponent = CreateDefaultSubobject<UGunComponent>(TEXT("WeaponComp"));

	//create initial hand component
	HandComponent = CreateDefaultSubobject<UHandComponent>(TEXT("HandComp"));

	//create initial deck component
	DeckComponent = CreateDefaultSubobject<UDeckComponentBase>(TEXT("DeckComp"));

	//create initial gun component
	PlayerModifierComponent = CreateDefaultSubobject<UPlayerModifierComponent>(TEXT("PlayerModifierComp"));

}

// Called when the game starts or when spawned
void AFirstPersonViewCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//Use BP for weapon component
	WeaponComponent = NewObject<UGunComponent>(this, GunComponentBP);
	UE_LOG(LogTemp, Warning, TEXT("Weapon comp blueprint name: %s"), *WeaponComponent->GetName())

	WeaponComponent->InitializeGunComponent();

	//Use BP for hand component
	HandComponent = NewObject<UHandComponent>(this, HandComponentBP);
	UE_LOG(LogTemp, Warning, TEXT("Hand comp blueprint name: %s"), *HandComponent->GetName())

	HandComponent->InitializeCardComponent();

	//Use BP for deck component
	DeckComponent = NewObject<UDeckComponentBase>(this, DeckComponentBP);
	UE_LOG(LogTemp, Warning, TEXT("Deck comp blueprint name: %s"), *DeckComponent->GetName())

	DeckComponent->InitializeCardComponent();
}

// Called to bind functionality to input
void AFirstPersonViewCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings -- red marks are an intellisense bug
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::JumpEvent);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::Look);

		//Shoot
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::Fire);

		//Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::Reload);

		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::Dash);

		//Swap weapon
		EnhancedInputComponent->BindAction(SwapAction, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::SwapWeapon);

		//Ability one
		EnhancedInputComponent->BindAction(AbilityActionOne, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::UseAbilityOne);

		//Ability two
		EnhancedInputComponent->BindAction(AbilityActionTwo, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::UseAbilityTwo);

		//Ability three
		EnhancedInputComponent->BindAction(AbilityActionThree, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::UseAbilityThree);

		//Ability four
		EnhancedInputComponent->BindAction(AbilityActionFour, ETriggerEvent::Triggered, this, &AFirstPersonViewCharacter::UseAbilityFour);
	}
}



//void AFirstPersonViewCharacter::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(AFirstPersonViewCharacter, WeaponComponent);
//}

void AFirstPersonViewCharacter::LaunchFromPad(FVector Velocity)
{
	GetCharacterMovement()->StopActiveMovement();
	LaunchCharacter(Velocity, false, true);
}

void AFirstPersonViewCharacter::JumpEvent_Implementation(const FInputActionValue& Value)
{
	TSet<AActor*> Hits;
	GetCapsuleComponent()->GetOverlappingActors(Hits);

	for (auto Actor : Hits)
	{
		if (Actor->ActorHasTag("JumpPad"))
		{
			LaunchFromPad(FVector(0, 0, 1500));
			break;
		}
	}

	Jump();
}

void AFirstPersonViewCharacter::Move_Implementation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFirstPersonViewCharacter::Look_Implementation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFirstPersonViewCharacter::Dash_Implementation(const FInputActionValue& Value)
{
	
}

void AFirstPersonViewCharacter::Fire(const FInputActionValue& Value)
{
	if (!IsValid(WeaponComponent)) return;
	WeaponComponent->Fire();
}

void AFirstPersonViewCharacter::Reload(const FInputActionValue& Value)
{
	if (!IsValid(WeaponComponent)) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("No Weapon Component, not reloading"))
		return; 
	}

	UE_LOG(LogTemp, Warning, TEXT("Performing reload"))
	WeaponComponent->Reload();
}

void AFirstPersonViewCharacter::SwapWeapon(const FInputActionValue& Value)
{
	if (!IsValid(WeaponComponent)) return;

	WeaponComponent->ToggleLoadout();
}


void AFirstPersonViewCharacter::UseAbilityOne(const FInputActionValue& Value)
{
	WeaponComponent->ApplyModifier(AbilityModifierOne);
}

void AFirstPersonViewCharacter::UseAbilityTwo(const FInputActionValue& Value)
{
	WeaponComponent->ApplyModifier(AbilityModifierTwo);
}

void AFirstPersonViewCharacter::UseAbilityThree(const FInputActionValue& Value)
{
	WeaponComponent->ApplyModifier(AbilityModifierThree);
}

void AFirstPersonViewCharacter::UseAbilityFour(const FInputActionValue& Value)
{
	WeaponComponent->ApplyModifier(AbilityModifierFour);
}


UGunComponent* AFirstPersonViewCharacter::GetWeaponComponenet()
{
	return WeaponComponent;
}

UPlayerModifierComponent* AFirstPersonViewCharacter::GetPlayerModifierComponenet()
{
	return PlayerModifierComponent;
}

UHandComponent* AFirstPersonViewCharacter::GetHandComponenet()
{
	return HandComponent;
}

UDeckComponentBase* AFirstPersonViewCharacter::GetDeckComponenet()
{
	return DeckComponent;
}