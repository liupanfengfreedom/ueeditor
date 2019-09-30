// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorActor.h"
#include "Engine.h"
#include "ControlCenter.h"
#if WITH_EDITORONLY_DATA
#include "Editor/UnrealEd/Public/Editor.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#endif

// Sets default values

AEditorActor::AEditorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#if WITH_EDITORONLY_DATA
	USelection::SelectObjectEvent.AddUObject(this, &AEditorActor::OnObjectSelected);
	//FEditorDelegates::FOnEditorModeTransitioned.AddUObject(this, &AEditorActor::oneditormodtransitioned);
	FEditorDelegates::EditorModeEnter.AddUObject(this,&AEditorActor::oneditormodtransitioned);

#endif
}
#if WITH_EDITORONLY_DATA
void AEditorActor::OnObjectSelected(UObject* Object)
{
	if (Object == this)
	{
		if (mmcc)
		{
			FName pathname;
			FName pathnameforassignid;
			mmcc->findassetpath(pathname,pathnameforassignid);
			return;
		}
		mmcc = NewObject<UControlCenter>();
		mmcc->world = GetWorld();
		mmcc->ConnectServer();
		// DO STUFF
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnObjectSelected!"));
	}
	else if (!IsSelected())
	{
		// UNDO STUFF
	}
}
void AEditorActor::oneditormodtransitioned(FEdMode *  m)
{

	if (GetWorld() != nullptr)
	{
		if (mmcc)
		{
			return;
		}
		mmcc = NewObject<UControlCenter>();
		mmcc->world = GetWorld();
		mmcc->ConnectServer();
	}
}
#endif
// Called when the game starts or when spawned
void AEditorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEditorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

