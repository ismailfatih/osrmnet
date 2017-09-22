// Copyright (c) 2016 Surya Suluh and contributors.  All rights reserved.
// Licensed under the MIT License.  See included LICENSE in the project root for license information.

#include "..\Stdafx.h"
#include "..\Utils.h"

#include "RouteResult.h"
#include "RouteParameters.h"

#include "osrm\json_container.hpp"

using namespace Osrmnet::RouteService;
using namespace osrm::util::json;

RouteResult^ RouteResult::FromJsonObject(const osrm::util::json::Object& jsonObject, RouteParameters^ routeParameters)
{
	/*
	{
		"waypoints": [ { ... } ],
		"routes": [ { ... } ],
		"code": "Ok"
	}
	*/

	auto result = gcnew RouteResult();

	// Process waypoint
	auto codeJson = jsonObject.values.at("code").get<String>().value;
	result->Code = Osrmnet::Utils::ConvertFromUtf8(codeJson);
	

	// Process Routes
	const auto &routesJson = jsonObject.values.at("routes").get<Array>().values;
	for (const auto &route : routesJson)
	{
		const auto &routeObj = route.get<osrm::util::json::Object>();
		result->Routes->Add(Route::FromJsonObject(routeObj, routeParameters->Steps, routeParameters->Annotations, routeParameters->Geometries, routeParameters->Overview));
	}

	// Code
	const auto &routeWayPoints = jsonObject.values.at("waypoints").get<Array>().values;
	for (const auto &wayPointJson : routeWayPoints)
	{
		const auto &wayPointObj = wayPointJson.get<osrm::util::json::Object>();
		result->WayPoints->Add(Waypoint::FromJsonObject(wayPointObj));
	}

	return result;
}