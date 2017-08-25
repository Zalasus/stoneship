/*
 * GUIToolkit.h
 *
 *  Created on: 25 Jun 2017
 *      Author: zalasus
 */

#ifndef INCLUDE_GUI_GUITOOLKIT_H_
#define INCLUDE_GUI_GUITOOLKIT_H_


namespace Stoneship
{

    class Window;


    class GUIToolkit
    {
    public:

        GUIToolkit(Screen *screen);

        Window *createWindow(double originX, double originY, double sizeX, double sizeY);



    private:

        Screen mScreen;

        std::vector<Window*> mWindows;

    };

}


#endif /* INCLUDE_GUI_GUITOOLKIT_H_ */
