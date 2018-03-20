#include "Pattern.h"
#include "PatternRow.h"
#include <cstring>

const int Pattern::maxRows;

Pattern::Pattern()
{
	rows = new PatternRow[maxRows];
}


Pattern::~Pattern()
{
	delete [] rows;
}


PatternRow& Pattern::getRow(int row)
{
	return rows[row];
}


void Pattern::clear()
{
	for (int i = 0 ; i < maxRows ; ++i)
		rows[i].clear();
}


int Pattern::getLastUsedRow() const
{
	int last = -1;

	for (int i = 0 ; i < maxRows ; ++i)
	{
		const PatternRow& row = rows[i];
		bool hasData = !row.getNote().isEmpty();

		for (int effectParam = 0 ; !hasData && effectParam < PatternRow::effectParams ; ++effectParam)
		{
			hasData |= !row.getEffect(effectParam).isEmpty();
		}

		if (hasData)
		{
			last = i;
		}
	}

	return last + 1;
}


int Pattern::getLastMacroUsed() const
{
	int last = -1;

	for (int i = 0 ; i < maxRows ; ++i)
	{
		const PatternRow& row = rows[i];
		if (row.getNote().effect == 'm')
		{
			last = row.getNote().getParamsAsByte();
		}

		for (int effectParam = 0 ; effectParam < PatternRow::effectParams ; ++effectParam)
		{
			if (row.getEffect(effectParam).effect == 'm')
			{
				last = row.getEffect(effectParam).getParamsAsByte();
			}
		}
	}

	return last;
}


bool Pattern::isEmpty() const
{
	return getLastUsedRow() == 0;
}


void Pattern::insertRow(int row, int flags)
{
	for (int i = maxRows - 1 ; i > row ; --i)
	{
		PatternRow& src = rows[i - 1];
		PatternRow& dest = rows[i];

		if (flags & PatternRow::FlagNote)
		{
			dest.getNote() = src.getNote();
		}

		if (flags & PatternRow::FlagEffect)
		{
			for (int effectParam = 0 ; effectParam < PatternRow::effectParams ; ++effectParam)
			{
				dest.getEffect(effectParam) = src.getEffect(effectParam);
			}
		}
	}

	rows[row].clear(flags);
}


void Pattern::deleteRow(int row, int flags)
{
	for (int i = row ; i < maxRows - 1 ; ++i)
	{
		PatternRow& src = rows[i + 1];
		PatternRow& dest = rows[i];

		if (flags & PatternRow::FlagNote)
		{
			dest.getNote() = src.getNote();
		}

		if (flags & PatternRow::FlagEffect)
		{
			for (int effectParam = 0 ; effectParam < PatternRow::effectParams ; ++effectParam)
			{
				dest.getEffect(effectParam) = src.getEffect(effectParam);
			}
		}
	}

	rows[maxRows - 1].clear(flags);
}
