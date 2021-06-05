#include "mvThemeColorGroup.h"
#include "mvAppItem.h"

namespace Marvel {

	mvAppItemType mvThemeColorGroup::DecodeType(long encoded_constant)
	{ 
		return (mvAppItemType)(encoded_constant / 1000); 
	}

	size_t GetCacheColorCount(mvAppItem* item)
	{
		if (item->isEnabled())
			return item->getColorGroup().getCachedColors().size();
		else
			return item->getDisabledColorGroup().getCachedColors().size();
	}

	mvThemeColorGroup::mvThemeColorGroup(mvAppItem* parent)
		:
		m_parent(parent)
	{
	}

	bool mvThemeColorGroup::doesColorExists(long constant) const
	{
		for (const auto& color : m_cachedColors)
		{
			if (color.constant == constant)
				return true;
		}

		return false;
	}

	bool mvThemeColorGroup::addColor(mvThemeColor color)
	{
		if(doesColorExists(color.constant))
		{
			assert(false && "color already exists here");
			return false;
		}

		m_cachedColors.push_back(color);
		return true;
	}

	bool mvThemeColorGroup::addColor(const std::string& name, long constant, mvColor color)
	{
		if (doesColorExists(constant))
		{
			//assert(false && "color with this constant already exists");
			return false;
		}

		m_colors.push_back({name, constant, color, m_parent, false});
		return true;
	}

	std::vector<mvThemeColorGroup::mvThemeColor> mvThemeColorGroup::getColorsByType(mvAppItemType type) const
	{
		std::vector<mvThemeColorGroup::mvThemeColor> colors;

		for (const auto& color : m_colors)
		{
			if ((mvAppItemType)mvThemeColorGroup::DecodeType(color.constant) == type)
				colors.push_back(color);
		}

		return colors;
	}

	const std::vector<mvThemeColorGroup::mvThemeColor>& mvThemeColorGroup::getCachedColors() const
	{
		return m_cachedColors;
	}

	bool mvThemeColorGroup::isCacheValid() const
	{
		return !m_dirty;
	}

	void mvThemeColorGroup::invalidateCache()
	{
		m_dirty = true;
		m_cachedColors.clear();
	}

	void mvThemeColorGroup::setCacheValid()
	{
		m_dirty = false;
	}

}