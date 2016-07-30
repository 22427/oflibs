#pragma once
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "vmath.h"

namespace vrpv {


/**
 * @brief The Screen class is basically an area in world space you want to
 * render to. This might be a powerwall or the side of a cave or a segment
 * in a segmented display.
 */
class Screen
{
	vec4 m_corners[3];
	mat4 m_wall_space;
	mat4 m_inv_wall_space;

public:

	enum CORNER
	{
		BL=0,
		BR,
		TL
	} ;

	vec4 corner(const int i) const;
	/**
	 * @brief Screen Constructor. Consumes three corners of the Screen area
	 * The last corner is implied.
	 * @param blc Bottom left corner.
	 * @param trc Top right corner.
	 * @param tlc Top left corner.
	 */
	Screen(const vec4 blc,const vec4 brc, const vec4& tlc);

	/**
	 * @brief calculate_projection_and_view Calculates a projection- and a view
	 * matrix for a given eye position.
	 * Note for stereo rendering you have to call this twice, once per eye.
	 * @param eye The postition of the eye.
	 * @param near The near distance.
	 * @param far The far distance.
	 * @param projection output where the projection matrix will be written to
	 * @param view output where the view matrix will be written to
	 */
	void calculate_projection_and_view(
			const vec4& eye,
			const float near,
			const float far,
			mat4& projection_matrix,
			mat4& view_matrix) const;
};

/**
 * @brief The ScreenArrangement class groups Screens together. This might be use
 * full for a cave or a multi-segment-display. Each Screen is accessible via
 * its name or its id, or you just store a pointer to the screen you are
 * interested in. In that case this class is only usefull for loading and
 * storing ScreenArrangements
 */
class ScreenArrangement
{
	std::vector<Screen> m_screens;
	std::map<std::string,Screen*> m_by_name;
public:
	/**
	 * @brief addScreen Adds a Screen to the arrangement.
	 * @param scrn The screen you wish to add (will be copied).
	 * @param name The name of the screen.
	 * @return a pointer to the interal screen, which can be accessed as long
	 * as this Object exists.
	 */
	Screen* addScreen(const Screen scrn, const std::string& name);

	/**
	 * @brief getScreen Provides access to a screen via its id. If there is
	 * no screen with this id a nullptr is returned.
	 * @param id The id of the screen.
	 * @return The Screen you asked for or nullptr if it does not exist.
	 */
	const Screen* getScreen(const int id)const;

	/**
	 * @brief getScreen Provides access to a screen via its name. If there is
	 * no screen with this name a nullptr is returned.
	 * @param name name of the screen.
	 * @return	The Screen you asked for or nullptr if it does not exist.
	 */
	const Screen* getScreen(const std::string& name);

	/**
	 * @brief count_screens Returns the number of screens in this
	 * ScreenArrangement
	 * @return #Screens
	 */
	unsigned int countScreens() const;

	/**
	 * @brief addScreens Adds screens from a file to this arrangement.
	 * The format of the file should be:
	 * <name0> (blx,bly,blz,blw) (brx,bry,brz,brw) (tlx,tly,tlz,tlw)
	 * <name1> (blx,bly,blz,blw) (brx,bry,brz,brw) (tlx,tly,tlz,tlw)
	 * # ...
	 * # a line starting with # is a comment line.
	 * @param path to the file.
	 * @return true if everything went well. false if there was a problem with
	 * the file.
	 */
	bool loadScreens(const std::string& path);

	/**
	 * @brief saveScreens saves this ScreenArrangement to the disk
	 * @param path where you want to store this.
	 * @return true if ecerything went well, false if there was a problem with
	 * the file.
	 */
	bool saveScreens(const std::string& path);
};
}
