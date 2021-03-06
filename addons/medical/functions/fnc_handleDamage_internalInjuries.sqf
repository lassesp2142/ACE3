/*
 * Author: Glowbal
 * Handling of the internal injuries upon the handleDamage eventhandler.
 *
 * Arguments:
 * 0: Unit That Was Hit <OBJECT>
 * 1: Name Of Hit Selection <STRING>
 * 2: Amount Of Damage <NUMBER>
 * 3: Shooter or source of the damage <OBJECT>
 * 4: Type of the damage done <STRING>
 *
 * Return Value:
 * None <NIL>
 *
 * Public: No
 */

#include "script_component.hpp"

private ["_unit", "_selectionName", "_amountOfDamage", "_sourceOfDamage", "_typeOfDamage", "_bodyPartn"];
_unit = _this select 0;
_selectionName = _this select 1;
_amountOfDamage = _this select 2;
_sourceOfDamage = _this select 3;
_typeOfDamage = _this select 4;
_bodyPartn = [_selectionName] call FUNC(selectionNameToNumber);

// TODO implement internal injuries
