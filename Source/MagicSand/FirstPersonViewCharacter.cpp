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
#include "MagicSandGameModeBase.h"

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

	//create initial stats component
	PlayerModifierComponent = CreateDefaultSubobject<UPlayerModifierComponent>(TEXT("PlayerModifierComp"));

	PlayerModifierComponent->OnApplyModifier.AddDynamic(this, &AFirstPersonViewCharacter::UpdateMovement);
	PlayerModifierComponent->OnRemoveModifier.AddDynamic(this, &AFirstPersonViewCharacter::UpdateMovement);

	PlayerModifierComponent->OnApplyModifier.AddDynamic(this, &AFirstPersonViewCharacter::Client_BroadcastStatChange);
	PlayerModifierComponent->OnRemoveModifier.AddDynamic(this, &AFirstPersonViewCharacter::Client_BroadcastStatChange);

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
	WeaponComponent->RegisterComponent();
	AddInstanceComponent(WeaponComponent);
	UE_LOG(LogTemp, Warning, TEXT("Weapon comp blueprint name: %s"), *WeaponComponent->GetName())

	WeaponComponent->InitializeGunComponent(PlayerModifierComponent);

	//Use BP for hand component
	HandComponent = NewObject<UHandComponent>(this, HandComponentBP);
	UE_LOG(LogTemp, Warning, TEXT("Hand comp blueprint name: %s"), *HandComponent->GetName())

	HandComponent->SetNetAddressable();
	HandComponent->ServerInitializeCardComponent();

	//Use BP for deck component
	DeckComponent = NewObject<UDeckComponentBase>(this, DeckComponentBP);
	UE_LOG(LogTemp, Warning, TEXT("Deck comp blueprint name: %s"), *DeckComponent->GetName())

	DeckComponent->SetNetAddressable();
	DeckComponent->ServerInitializeCardComponent();

	// For speed adjustment purposes
	InitialSpeed = GetCharacterMovement()->MaxWalkSpeed;
}


void AFirstPersonViewCharacter::UpdateMovement(UPlayerModifier* Modifier)
{
	auto Movement = GetCharacterMovement();
	auto StatComponent = GetPlayerModifierComponent();

	if (!IsValid(Movement) || !IsValid(StatComponent)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempting to update movement without required components."))
		return; 
	}

	float SpeedAdjustment = StatComponent->GetCurrentModifications().SpeedMultiplier;
	Movement->MaxWalkSpeed = InitialSpeed * (1 + SpeedAdjustment );
}


void AFirstPersonViewCharacter::RegisterUIEvents()
{
	PlayerModifierComponent->OnRemoveModifier.AddDynamic(this, &AFirstPersonViewCharacter::Client_BroadcastStatChange_Implementation);
	PlayerModifierComponent->OnApplyModifier.AddDynamic(this, &AFirstPersonViewCharacter::Client_BroadcastStatChange_Implementation);
}

void AFirstPersonViewCharacter::Destroyed()
{
	Super::Destroyed();

	// Example to bind to OnPlayerDied event in GameMode. 
	if (UWorld* World = GetWorld())
	{
		if (AMagicSandGameModeBase* GameMode = Cast<AMagicSandGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->GetOnPlayerDied().Broadcast(this);
		}
	}
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


void AFirstPersonViewCharacter::CallRestartPlayer()
{
	//Get a reference to the Pawn Controller.
	AController* CortollerRef = GetController();

	//Destroy the Player.
	Destroy();

	//Get the World and GameMode in the world to invoke its restart player function.
	if (UWorld* World = GetWorld())
	{
		if (AMagicSandGameModeBase* GameMode = Cast<AMagicSandGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(CortollerRef);
		}
	}
}

void AFirstPersonViewCharacter::LaunchFromPad(FVector Velocity)
{
	GetCharacterMovement()->StopActiveMovement();
	LaunchCharacter(Velocity, false, true);
}


void AFirstPersonViewCharacter::DrawCards(int Amount)
{
	DeckComponent->ServerDrawCards(Amount);
}


void AFirstPersonViewCharacter::Client_BroadcastStatChange_Implementation(UPlayerModifier* Modifier)
{
	UE_LOG(LogTemp, Warning, TEXT("Attempting stats changed broadcast."))
	OnStatsChanged.Broadcast();
}

bool AFirstPersonViewCharacter::Client_BroadcastStatChange_Validate(UPlayerModifier* Modifier)
{
	return true;
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
	HandComponent->ServerPlayCard(0, this);
}

void AFirstPersonViewCharacter::UseAbilityTwo(const FInputActionValue& Value)
{
	HandComponent->ServerPlayCard(1, this);
}

void AFirstPersonViewCharacter::UseAbilityThree(const FInputActionValue& Value)
{
	HandComponent->ServerPlayCard(2, this);
}

void AFirstPersonViewCharacter::UseAbilityFour(const FInputActionValue& Value)
{
	HandComponent->ServerPlayCard(3, this);
}

UGunComponent* AFirstPersonViewCharacter::GetWeaponComponenet()
{
	return WeaponComponent;
}

UPlayerModifierComponent* AFirstPersonViewCharacter::GetPlayerModifierComponent()
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