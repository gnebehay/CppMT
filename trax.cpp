#include <trax.h>

#include "CMT.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ofstream;

using cmt::CMT;
using cv::cvtColor;
using cv::imread;

int main()
{


    trax_handle* trax;
    trax_configuration config;
    config.format_region = TRAX_REGION_RECTANGLE;
    config.format_image = TRAX_IMAGE_PATH;

    // Call trax_server_setup to initialize trax protocol
    trax = trax_server_setup_standard(config, NULL);

    ofstream ofile("/home/georg/traxlog");

    trax_image* img = NULL;
    trax_region* rect = NULL;

    CMT cmt;

    while(true)
    {

        Rect selection;

        trax_properties* prop = trax_properties_create();

        // The main idea of Trax interface is to leave the control to the master program
        // and just follow the instructions that the tracker gets. 
        // The main function for this is trax_wait that actually listens for commands.

        int tr = trax_server_wait(trax, &img, &rect, prop);


        // There are two important commands. The first one is TRAX_INITIALIZE that tells the
        // tracker how to initialize.
        if (tr == TRAX_INITIALIZE)
        {
            ofile << "TRAX_INITIALIZE" << endl;
            float x, y, width, height;
            trax_region_get_rectangle(rect, &x, &y, &width, &height);

            selection.x = x;
            selection.y = y;
            selection.width = width;
            selection.height = height;

            Mat im = imread(img->data);
            Mat im_gray;

            cvtColor(im, im_gray, CV_BGR2GRAY);
            cmt.initialize(im_gray, selection);

            // properties
            trax_server_reply(trax, rect, NULL);
        }
        // The second one is TRAX_FRAME that tells the tracker what to process next.
        else if (tr == TRAX_FRAME)
        {
            ofile << "TRAX_FRAME" << endl;
            // In trax mode images are read from the disk. The master program tells the
            // tracker where to get them.
            Mat im = imread(img->data);
            Mat im_gray;

            cvtColor(im, im_gray, CV_BGR2GRAY);
            cmt.processFrame(im_gray);
            Rect output = cmt.bb_rot.boundingRect();

            // At the end of single frame processing we send back the estimated
            // bounding box and wait for further instructions.

            trax_region* region = trax_region_create_rectangle(output.x, output.y, output.width, output.height);

            // Note that the tracker also has an option of sending additional data
            // back to the main program in a form of key-value pairs. We do not use
            // this option here, so this part is empty.
            trax_server_reply(trax, region, NULL);

            trax_region_release(&region);
        }
        // Any other command is either TRAX_QUIT or illegal, so we exit.
        else
        {
            trax_properties_release(&prop);
            break;
        }

        trax_properties_release(&prop);

        }

    // Call trax_cleanup to release potentially allocated resources 
    trax_cleanup(&trax);

    return 0;

}

