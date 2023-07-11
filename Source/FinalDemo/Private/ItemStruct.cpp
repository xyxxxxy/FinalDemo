// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStruct.h"


FItemStruct::FItemStruct(FName Name /*= "0" */)
{
	ID=Name;
}

FName FItemStruct::GetID() const
{
	return ID;
}

FItemPropertyStruct::FItemPropertyStruct()
{
	ItemName="0";
	Mesh=nullptr;
}


