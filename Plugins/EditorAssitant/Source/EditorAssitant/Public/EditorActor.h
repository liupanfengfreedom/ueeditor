// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EditorActor.generated.h"

UCLASS()
class EDITORASSITANT_API AEditorActor : public AActor
{
	GENERATED_BODY()
		UPROPERTY()
		class UControlCenter* mmcc;
#if WITH_EDITORONLY_DATA
		void OnObjectSelected(UObject* Object);
		void oneditormodtransitioned(class  FEdMode *);
#endif

public:	
	// Sets default values for this actor's properties
	AEditorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
