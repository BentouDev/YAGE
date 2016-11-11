//
// Created by bentoo on 10/19/16.
//

#ifndef GAME_PROPERTYMACRO_H
#define GAME_PROPERTYMACRO_H

#define YAGE_PROPERTY(field)\
	inline decltype(field) get##field() { return field; } const\
	inline void set##field(decltype(field) value) { field = value; }

#define YAGE_PROPERTY_NAMED(field, name)\
	inline decltype(field) get##name() { return field; } const\
	inline void set##name(decltype(field) value) { field = value; }

#endif //GAME_PROPERTYMACRO_H
