////////////////////////////////////////////////////////////////////
///
///   MorkParser.h - Mozilla Mork Format Parser/Reader 
///
///   Copyright (C) 2007 ScalingWeb.com
///   All rights reserved. 
/// 
///   Authors: Yuriy Soroka <ysoroka@scalingweb.com>
///	       Anton Fedoruk <afedoruk@scalingweb.com>
///
///
/// This program is free software; you can redistribute it and/or
/// modify it under the terms of the GNU General Public License
/// as published by the Free Software Foundation; either version 2
/// of the License, or (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this library; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
/// 
///////////////////////////////////////////////////////////////////


#ifndef __MorkParser_h__
#define __MorkParser_h__

#include <QMap>
#include <QByteArray>
class QString;

// Types

typedef QMap< int, std::string > MorkDict;
typedef QMap< int, int > MorkCells;					// ColumnId : ValueId
typedef QMap< int, MorkCells > MorkRowMap;			// Row id
typedef QMap< int, MorkRowMap > RowScopeMap;		// Row scope
typedef QMap< int, RowScopeMap > MorkTableMap;		// Table id
typedef QMap< int, MorkTableMap > TableScopeMap;	// Table Scope

// Mork header of supported format version
const char MorkMagicHeader[] = "// <!-- <mdb:mork:z v=\"1.4\"/> -->";

const char MorkDictColumnMeta[] = "<(a=c)>";

// Error codes
enum MorkErrors
{
	NoError = 0,
	FailedToOpen,
	UnsupportedVersion,
	DefectedFormat
};

// Mork term types
enum MorkTerm
{
	NoneTerm = 0,
	DictTerm,
	GroupTerm,
	TableTerm,
	RowTerm,
	CellTerm,
	CommentTerm,
	LiteralTerm
};


/// Class MorkParser

class MorkParser
{
public:

	MorkParser( int defaultScope = 0x80 );

	///
	/// Open and parse mork file

	bool open( const QString &path );

	///
	/// Return error status

	MorkErrors error();

	///
	/// Returns all tables of specified scope

	MorkTableMap *getTables( int tableScope );

	///
	/// Rerturns all rows under specified scope

	MorkRowMap *getRows( int rowScope, RowScopeMap *table );

	///
	/// Return value of specified value oid

	std::string &getValue( int oid );

	///
	/// Return value of specified column oid

	std::string &getColumn( int oid );

#ifndef QT_NO_DEBUG
	void debugWrite( const QString &path );
#endif // QT_NO_DEBUG


protected: // Members

	void initVars();

	bool isWhiteSpace( char c );
	char nextChar();

	void parseScopeId( const std::string &TextId, int *Id, int *Scope );
	void setCurrentRow( int TableScope, int TableId, int RowScope, int RowId );

	// Parse methods
	bool parse();
	bool parseDict();
	bool parseComment();
	bool parseCell();
	bool parseTable();
	bool parseMeta( char c );
	bool parseRow( int TableId, int TableScope );
	bool parseGroup();

protected: // Data

	// Columns in mork means value names
	MorkDict columns_;
	MorkDict values_;

	// All mork file data
	TableScopeMap mork_;
	MorkCells *currentCells_;

	// Error status of last operation
	MorkErrors error_;

	// All Mork data
	QByteArray morkData_;

	int morkPos_;
	int nextAddValueId_;
	int defaultScope_;

	// Indicates intity is being parsed
	enum { NPColumns, NPValues, NPRows } nowParsing_;

};

#endif // __MorkParser_h__

