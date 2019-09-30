// Fill out your copyright notice in the Description page of Project Settings.

#include "ControlCenter.h"
#include "TcpClientv.h"
#include "EditorFunctionLibrary.h"
#include "JsonUtilities.h"
#include "Json.h"
#if WITH_EDITORONLY_DATA
#include "Editor/UnrealEd/Public/Editor.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "MyBlueprintFunctionLibrary.h"
#include "IAssetTools.h"
#endif

class TcpClientv* UControlCenter::mtcp = nullptr;

UControlCenter::UControlCenter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{//UObject(const FObjectInitializer& ObjectInitializer)

}
bool UControlCenter::ConnectServer()
{
	if (mtcp == nullptr)
	{
		mtcp = new TcpClientv();
		isconnected = mtcp->Connecttoserver(192, 168, 1, 240, 8003);
		mtcp->OnTcpClientReceiveddata.AddDynamic(this, &UControlCenter::OnTcpResponse);
		if (isconnected)
		{

		}
#if WITH_EDITORONLY_DATA
		FEditorDelegates::EditorModeExit.AddStatic(&UControlCenter::clientexit);
#endif
		//UMyBlueprintFunctionLibrary::AddfunctiontoOnGameexitArray(&UTcpCommunicatorv1::clientexit);
		check(world);
		world->GetTimerManager().SetTimer(th, this, &UControlCenter::thwork, 0.5, true, 1);
	}
	return isconnected;
}
void UControlCenter::SplitString(FString str, TArray<FString>&strarray)
{//////?xxxxx?xxxxx?xxxxx...
	FString str1;
	FString str2;
	str.Split("?", &str1, &str2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	strarray.Insert(str2, 0);
	if (!str1.IsEmpty())
	{
		SplitString(str1, strarray);
	}
}
void UControlCenter::clientexit(FEdMode * mm)
{
	UControlCenter::mtcp->exitthread = true;
}
void UControlCenter::OnTcpResponse(const TArray<uint8>& p, const FString & str)
{
	FMessagePackage mp;
	FJsonObjectConverter::JsonObjectStringToUStruct<FMessagePackage>(str, &mp, 0, 0);
	if (mp.MT == MessageType::ASSIGNID)
	{
		command = mp;
	}
}
void UControlCenter::thwork()
{
	if (command.MT == MessageType::ASSIGNID)
	{
		UMyBlueprintFunctionLibrary::CLogtofile("command.MT == MessageType::ASSIGNID");
		command.MT = MessageType::EMPTY;
		FString message = command.PayLoad;
		TArray<FString> payloadmessage;
		SplitString(message, payloadmessage);
		//FName path = "/Game/VehicleBP/Maps/VehicleExampleMap";
		//FName path = FName(*payloadmessage[0]);
	    //int32 MyNewInt = FCString::Atoi(*payloadmessage[1]);
		//MyNewInt = 1;
		FName path = "";
		FName pathforassignid = "";
		findassetpath(path,pathforassignid);
		UMyBlueprintFunctionLibrary::CLogtofile("--------------------------");
		if (path.IsEqual(""))
		{

		}
		else
		{
			UMyBlueprintFunctionLibrary::CLogtofile(path.ToString());
			UEditorFunctionLibrary::AssignID(pathforassignid,1);
			FMessagePackage mp;
			mp.MT = MessageType::ASSIGNOK;
			mp.PayLoad = path.ToString();
			FString outstring;
			FJsonObjectConverter::UStructToJsonObjectString<FMessagePackage>(mp, outstring);
			mtcp->Send(outstring);
		}
	}

}
void UControlCenter::findassetpath(FName&path,FName&pathforassignid)
{
	UMyBlueprintFunctionLibrary::CLogtofile("Blueprint");
	findmeshpath(path, pathforassignid, "Blueprint");
	if (!path.IsEqual(""))//if Blueprint is exist just return Blueprint
	{
		return;
	}
	UMyBlueprintFunctionLibrary::CLogtofile("StaticMesh");
	findmeshpath(path, pathforassignid, "StaticMesh");
	if (!path.IsEqual(""))//if Blueprint not exist but StaticMesh is exist just return StaticMesh
	{
		return;
	}
	UMyBlueprintFunctionLibrary::CLogtofile("SkeletalMesh");
	findmeshpath(path, pathforassignid, "SkeletalMesh");
	if (!path.IsEqual(""))//if Blueprint and SkeletalMesh both not exist but SkeletalMesh is exist just return StaticMesh
	{
		FName subpath;
		//UMyBlueprintFunctionLibrary::CLogtofile("AnimSequence");
		//findmeshpath(subpath, "AnimSequence");
		//if (!subpath.IsEqual(""))
		//{
		//	FString strpath = path.ToString();
		//	strpath = strpath + "?" + subpath.ToString();
		//	path = *strpath;
		//	return;
		//}
	}
}
void UControlCenter::findmeshpath(FName & path, FName & pathforassignid,const FName& pclassname)
{
	FAssetRegistryModule* AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));	
	TArray<FAssetData> AssetData;
	FARFilter Filter = FARFilter();
	Filter.ClassNames.Add(pclassname);
	FString Directory = "";
	FString DirPrefix = Directory.IsEmpty() ? "/Game" : "/Game/";
	Filter.PackagePaths.Add(FName(*(DirPrefix + Directory)));
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	AssetRegistryModule->Get().GetAssets(Filter, AssetData);

	for (int i = 0; i < AssetData.Num(); i++)
	{
		if (!AssetData[i].AssetClass.IsEqual(pclassname))
		{
			AssetData.RemoveAt(i);
		}
	}
	int len = AssetData.Num();
	if (len == 0)
	{
		return;
	}
	pathforassignid = AssetData[0].PackageName;
	TArray<FStringFormatArg>FormatArray;
	FormatArray.Add(FStringFormatArg(AssetData[0].AssetClass.ToString()));
	FormatArray.Add(FStringFormatArg(AssetData[0].ObjectPath.ToString()));
	FString referencepath = FString::Format(TEXT("{0}'{1}'"), FormatArray);
	path = *referencepath;
}

