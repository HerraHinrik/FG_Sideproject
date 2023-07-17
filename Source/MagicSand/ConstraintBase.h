// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "ConstraintBase.generated.h"


/**
 * A constraint represents a requirement that has to be met before an order to shoot goes through.
 * 
 * The order in which constraints are applied doesn't matter. To make a new constraint simply subclass
 * this class in either c++ or blueprints. The only functionality not yet exposed to blueprints is 
 * responding to gun components' tick function. This class is an "empty" constraint that always returns
 * true when evaluating. Use subclasses in game logic.
 * 
 * Functions you have to override:
 *		- Evaluate <-- returns true if requirement is met, false otherwise
 * 
 * Functions you can use if you need:
 *		- ProcessFire <-- upkeep that happens once per fire goes here (fires once per successful fire action)
 *		- OnReload <-- this code gets called whenever the gun component reloads
 *		- Tick <-- not supported for blueprint overriding right now
 */
UCLASS(Blueprintable)
class UConstraintBase : public UObject
{
	GENERATED_BODY()
	
public:
	UConstraintBase();

private:
	// Track The last frame number we were ticked.
	uint32 LastTickFrame = INDEX_NONE;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bUsesTick = false;

public:

	// Overrride these to implement logic
	// Evaluate is the most important -- it returns the boolean that determins if the weapon will fire
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Evaluate();
	virtual bool Evaluate_Implementation();

	// For when upkeep needs to happen when the weapon actually fires
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ProcessFire();
	virtual void ProcessFire_Implementation();

	// Responding to reload, leave blank if you don't care about reload
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnReload();
	virtual void OnReload_Implementation();

	virtual void ConstraintTick(float DeltaTime);

	bool CanTick();

};
